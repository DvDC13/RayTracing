#include <vector>
#include "Object.h"
#include "Light.h"
#include "Camera.h"
#include "Utils.h"

#include <memory>

class Scene : public Object
{
public:
    Scene(const std::vector<std::shared_ptr<Object>>& objects, const std::vector<std::shared_ptr<Light>>& lights)
        : m_objects(objects), m_lights(lights)
    {
        m_camera = Camera(Point3(-1, 1, 1), Point3(0, 0, 0), Vector3(0, 1, 0), 90, 16.0 / 9.0);
    }

    void addObject(const std::shared_ptr<Object>& object) { m_objects.emplace_back(object); }
    void addLight(const std::shared_ptr<Light>& light) { m_lights.emplace_back(light); }
    void clearObjects() { m_objects.clear(); }
    void clearLights() { m_lights.clear(); }

    inline const std::vector<std::shared_ptr<Object>>& getObjects() const { return m_objects; }
    inline const std::vector<std::shared_ptr<Light>>& getLights() const { return m_lights; }
    inline const Camera& getCamera() const { return m_camera; }

    virtual bool intersects(const Ray& ray, double t_min, double t_max, hit_record& record) const override
    {
        hit_record tmp_record;
        bool hit = false;
        double closest_so_far = t_max;

        for (const auto& object : m_objects)
        {
            if (object->intersects(ray, t_min, closest_so_far, tmp_record))
            {
                hit = true;
                closest_so_far = tmp_record.t;
                record = tmp_record;
            }
        }

        return hit;
    }

    // Useless but need to override
    virtual Vector3 normalAt(const Point3& point, const Ray& ray, hit_record& record) const override
    {
        return Vector3(0, 0, 0);
    }

private:
    std::vector<std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<Light>> m_lights;
    Camera m_camera;
};