#include "Scene.h"
#include "Image.h"
#include "TextureMaterial.h"
#include "Mesh.h"
#include "Blob.h"

#include <chrono>
#include <iostream>

Color3 ray_cast(const Ray& r, const Scene& world, int limit)
{
    if (limit <= 0) return Color3(0, 0, 0);

    hit_record record;

    if (world.intersects(r, 0.001f, infinity, record))
    {
        Ray ray_out;
        Color3 color;
        double diffuse;
        double specular;
        if (record.textureMaterial->getTextureAt(r, record, color, ray_out, diffuse, specular))
        {
            for (auto light : world.getLights())
            {
                auto light_direction = light->getDirection(record.p);
                auto light_distance = light->getDistance(record.p);
                auto light_color = light->getColor();

                Ray shadow_ray(record.p, light_direction);
                hit_record shadow_record;

                if (world.intersects(shadow_ray, 0.001f, light_distance, shadow_record))
                {
                    continue;
                }

                auto light_intensity = Dot(record.normal, light_direction);

                if (light_intensity > 0)
                {
                    color += light_color * diffuse * light_intensity;
                }

                auto reflected = reflect(-light_direction, record.normal);
                auto specular_intensity = Dot(reflected, light_direction);

                if (specular_intensity > 0)
                {
                    color += light_color * specular * pow(specular_intensity, 4);
                }
            }

            return color * ray_cast(ray_out, world, limit - 1);
        }
        else
        {
            return Color3(0, 0, 0);
        }
    }

    Vector3 unit_direction = Normalize(r.direction());
    auto t = 0.5 * (unit_direction.getY() + 1.0);
    return (1.0 - t) * Color3(1.0, 1.0, 1.0) + t * Color3(0.5, 0.7, 1.0);
}

Pixel processImageColor(Color3& pixel_color, int samples_per_pixel)
{
    auto r = pixel_color.getX();
    auto g = pixel_color.getY();
    auto b = pixel_color.getZ();

    auto scale = 1.0f / samples_per_pixel;

    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);

    int ir = static_cast<int>(256 * Clamp(r, 0.0, 0.999));
    int ig = static_cast<int>(256 * Clamp(g, 0.0, 0.999));
    int ib = static_cast<int>(256 * Clamp(b, 0.0, 0.999));

    Pixel pixel(ir, ig, ib);
    return pixel;
}

int main(int argc, char** argv)
{
    auto start = std::chrono::high_resolution_clock::now();

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <output file.ppm>" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    Scene world(objects, lights);

    auto material_ground = std::make_shared<UniformTexture>(Color3(0.8, 0.8, 0.0), 0.5f, 0.5f);
    auto material_blue = std::make_shared<UniformTexture>(Color3(0.2, 0.4, 0.9), 0.5f, 0.5f);
    //auto material_left = std::make_shared<MetalTexture>(Color3(1.0, 0.8, 0.8), 1.0);
    //auto material_right = std::make_shared<MirrorTexture>(Color3(1.0, 0.0, 0.0));

    int m_e = 4;
    int m_d = 1;

    std::shared_ptr<Sphere> sphere1 = std::make_shared<Sphere>(Point3(0, 0, 0), 0.10, material_blue);
    std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(Point3(m_e, 0, 0), 0.10, material_blue);
    std::shared_ptr<Sphere> sphere3 = std::make_shared<Sphere>(Point3(0, m_e, 0), 0.10, material_blue);
    std::shared_ptr<Sphere> sphere4 = std::make_shared<Sphere>(Point3(0, 0, m_e), 0.10, material_blue);
    std::shared_ptr<Sphere> sphere5 = std::make_shared<Sphere>(Point3(m_e, m_e, 0), 0.10, material_blue);
    std::shared_ptr<Sphere> sphere6 = std::make_shared<Sphere>(Point3(m_e, 0, m_e), 0.10, material_blue);
    std::shared_ptr<Sphere> sphere7 = std::make_shared<Sphere>(Point3(0, m_e, m_e), 0.10, material_blue);
    std::shared_ptr<Sphere> sphere8 = std::make_shared<Sphere>(Point3(m_e, m_e, m_e), 0.10, material_blue);

    world.addObject(sphere1);
    world.addObject(sphere2);
    world.addObject(sphere3);
    world.addObject(sphere4);
    world.addObject(sphere5);
    world.addObject(sphere6);
    world.addObject(sphere7);
    world.addObject(sphere8);

    Blob blob(Point3(0, 0, 0), m_e, m_d, 0.9, material_ground);
    Mesh mesh = blob.marchCubes();
    std::cout << "Mesh size: " << mesh.getMesh().size() << std::endl;
    world.addObject(std::make_shared<Mesh>(mesh));

    world.addLight(std::make_shared<DirectionalLight>(Point3(1, 4, 10), Color3(1, 1, 1), 1.0f));

    const int samples_per_pixel = 100;
    int max_depth = 50;

    int width = 400;
    int height = 360;
    Image image(width, height);

    for (int j = image.getHeight() - 1; j >= 0; j--)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image.getWidth(); i++)
        {
            Color3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                double u = double(i + RandomDouble()) / (image.getWidth() - 1);
                double v = double(j + RandomDouble()) / (image.getHeight() - 1);
                Ray ray = world.getCamera().getRay(u, v);
                pixel_color += ray_cast(ray, world, max_depth);
            }

            Pixel pixel = processImageColor(pixel_color, samples_per_pixel);
            image.setPixel(i, j, pixel);
        }
    }

    objects.clear();
    lights.clear();

    image.toPPM(argv[1]);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " seconds" << std::endl;

    return EXIT_SUCCESS;
}