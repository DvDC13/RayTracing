#pragma once

#include "Vector.h"

struct hit_record;

class TextureMaterial
{
public:
    virtual ~TextureMaterial() = default;

    virtual bool getTextureAt(const Ray& ray_in, const hit_record& record, Color3& color, Ray& ray_out, double& diffuse, double& specular) const = 0;
};

class UniformTexture : public TextureMaterial
{
public:
    UniformTexture(const Color3& color, double diffuse, double specular)
        : m_color(color), m_diffuse(diffuse), m_specular(specular) 
        {}

    virtual bool getTextureAt(const Ray& ray_in, const hit_record& record, Color3& color, Ray& ray_out, double& diffuse, double& specular) const override
    {
        auto dispersed_direction = record.normal + RandomInUnitSphereVector();
        if (dispersed_direction.nearZero()) dispersed_direction = record.normal;
        ray_out = Ray(record.p, dispersed_direction);
        color = m_color;
        diffuse = m_diffuse;
        specular = m_specular;
        return true;
    }

    inline const Color3& getColor() const { return m_color; }
    inline double getDiffuse() const { return m_diffuse; }
    inline double getSpecular() const { return m_specular; }

private:
    Color3 m_color;
    double m_diffuse;
    double m_specular;
};

class MirrorTexture : public TextureMaterial
{
public:
    MirrorTexture(const Color3& color)
        : m_color(color) 
        {}

    virtual bool getTextureAt(const Ray& ray_in, const hit_record& record, Color3& color, Ray& ray_out, double& diffuse, double& specular) const override
    {
        Vector3 reflected = reflect(Normalize(ray_in.direction()), record.normal);
        ray_out = Ray(record.p, reflected);
        color = m_color;
        return true;
    }
private:
    Color3 m_color;
};


class MetalTexture : public TextureMaterial
{
public:
    MetalTexture(const Color3& color, double fuzz)
        : m_color(color), m_fuzz(fuzz < 1 ? fuzz : 1) 
        {}
    
    virtual bool getTextureAt(const Ray& ray_in, const hit_record& record, Color3& color, Ray& ray_out, double& diffuse, double& specular) const override
    {
        Vector3 reflected = reflect(Normalize(ray_in.direction()), record.normal);
        ray_out = Ray(record.p, reflected + m_fuzz * RandomInUnitSphereVector());
        color = m_color;
        return (Dot(ray_out.direction(), record.normal) > 0);
    }

private:
    Color3 m_color;
    double m_fuzz;
};
