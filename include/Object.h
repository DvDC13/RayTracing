#pragma once

#include "Vector.h"
#include "Ray.h"

class TextureMaterial;

struct hit_record
{
    Point3 p;
    Vector3 normal;
    double t;
    bool front_face;

    inline void set_face_normal(const Ray& ray, const Vector3& outward_normal)
    {
        front_face = Dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }

    std::shared_ptr<TextureMaterial> textureMaterial;
};

class Object
{
public:
    Object() = default;

    virtual ~Object() = default;

    virtual bool intersects(const Ray& ray, double t_min, double t_max, hit_record& record) const = 0;

    virtual Vector3 normalAt(const Point3& point, const Ray& ray, hit_record& record) const = 0;
};

class Sphere : public Object
{
public:
    Sphere(const Point3& center, double radius, std::shared_ptr<TextureMaterial> material)
        : m_center(center)
        , m_radius(radius)
        , m_textureMaterial(material)
    {}

    virtual bool intersects(const Ray& ray, double t_min, double t_max, hit_record& record) const override
    {
        Vector3 oc = ray.origin() - m_center;
        double a = ray.direction().LengthSquared();
        double half_b = Dot(oc, ray.direction());
        double c = oc.LengthSquared() - m_radius * m_radius;
        double discriminant = half_b * half_b - a * c;
        if (discriminant < 0)
            return false;

        double sqrtd = sqrt(discriminant);
        double root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root)
        {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        record.t = root;
        record.p = ray.at(record.t);
        record.normal = normalAt(record.p, ray, record);
        record.textureMaterial = m_textureMaterial;

        return true;
    }

    virtual Vector3 normalAt(const Point3& point, const Ray& ray, hit_record& record) const override
    {
        Vector3 outward_normal = Normalize(point - m_center);
        record.set_face_normal(ray, outward_normal);
        return record.normal;
    }

private:
    Point3 m_center;
    double m_radius;
    std::shared_ptr<TextureMaterial> m_textureMaterial;
};

class Triangle : public Object
{
public:
    Triangle(Point3 p0, Point3 p1, Point3 p2, std::shared_ptr<TextureMaterial> material)
        : m_p0(p0), m_p1(p1), m_p2(p2)
        {}
    
    virtual bool intersects(const Ray& ray, double t_min, double t_max, hit_record& record) const override
    {
        double D = -Dot(record.normal, m_p1);
        double t = -(Dot(record.normal, ray.origin()) + D) / Dot(record.normal, ray.direction());
        Vector3 Phit = ray.at(t);
        record.t = t;
        record.p = Phit;
        record.normal = normalAt(record.p, ray, record);
        record.textureMaterial = m_textureMaterial;

        Vector3 edge0 = m_p1 - m_p0;
        Vector3 edge1 = m_p2 - m_p1;
        Vector3 edge2 = m_p0 - m_p2;
        Vector3 c0 = Phit - m_p0;
        Vector3 c1 = Phit - m_p1;
        Vector3 c2 = Phit - m_p2;

        return Dot(record.normal, Cross(edge0, c0)) > 0 && Dot(record.normal, Cross(edge1, c1)) > 0
                && Dot(record.normal, Cross(edge2, c2)) > 0;
    }

    virtual Vector3 normalAt(const Point3& point, const Ray& ray, hit_record& record) const override
    {
        Vector3 u = m_p1 - m_p0;
        Vector3 v = m_p2 - m_p0;
        Vector3 outward_normal = Cross(u, v);
        record.set_face_normal(ray, outward_normal);
        return outward_normal;
    }

private:
    Point3 m_p0;
    Point3 m_p1;
    Point3 m_p2;
    std::shared_ptr<TextureMaterial> m_textureMaterial;
};