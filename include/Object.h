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
        {
            m_textureMaterial = material;
        }
    
    // See: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution.html
    virtual bool intersects(const Ray& ray, double t_min, double t_max, hit_record& record) const override
    {
        // Plane equation: Ax + By + Cz + D = 0
        // N = (A, B, C)
        // D = -Dot(N, P0) // P0 is a point on the plane
        // t = -(Dot(N, P0) + D) / Dot(N, RD)
        // P = O + t * RD

        Vector3 u = m_p1 - m_p0;
        Vector3 v = m_p2 - m_p0;
        Vector3 N = Cross(u, v);
        double NsRD = Dot(N, ray.direction());
        if (NsRD == 0) return false; // ray is parallel to triangle

        double D = -Dot(N, m_p0);

        /*
        P = O + t * RD
        Ax + By + Cz + D = 0
        A*Px + B*Py + C*Pz + D = 0
        A*(Ox + t * RDx) + B*(Oy + t * RDy) + C*(Oz + t * RDz) + D = 0
        A*Ox + A*t * RDx + B*Oy + B*t * RDy + C*Oz + C*t * RDz + D = 0
        A*Ox + B*Oy + C*Oz + D + A*t * RDx + B*t * RDy + C*t * RDz = 0
        t*(A*RDx + B*RDy + C*RDz) = -(A*Ox + B*Oy + C*Oz + D)
        t = -(A*Ox + B*Oy + C*Oz + D) / (A*RDx + B*RDy + C*RDz)
        t = -(Dot(N, O) + D) / Dot(N, RD)
        */
        double t = -(Dot(N, ray.origin()) + D) / NsRD;

        if (t < t_min || t > t_max) return false;

        Point3 P = ray.at(t);
        Vector3 C;

        Vector3 edge0 = m_p1 - m_p0;
        Vector3 vp0 = P - m_p0;
        C = Cross(edge0, vp0);
        if (Dot(N, C) < 0) return false;

        Vector3 edge1 = m_p2 - m_p1;
        Vector3 vp1 = P - m_p1;
        C = Cross(edge1, vp1);
        if (Dot(N, C) < 0) return false;

        Vector3 edge2 = m_p0 - m_p2;
        Vector3 vp2 = P - m_p2;
        C = Cross(edge2, vp2);
        if (Dot(N, C) < 0) return false;

        record.t = t;
        record.p = P;
        record.normal = normalAt(P, ray, record);
        record.textureMaterial = m_textureMaterial;

        return true;
    }

    virtual Vector3 normalAt(const Point3& point, const Ray& ray, hit_record& record) const override
    {
        Vector3 u = m_p1 - m_p0;
        Vector3 v = m_p2 - m_p0;
        Vector3 outward_normal = Cross(u, v);
        record.set_face_normal(ray, outward_normal);
        return outward_normal;
    }

    inline Point3 getP0() const { return m_p0; }
    inline Point3 getP1() const { return m_p1; }
    inline Point3 getP2() const { return m_p2; }

    inline void setP0(Point3 p0) { m_p0 = p0; }
    inline void setP1(Point3 p1) { m_p1 = p1; }
    inline void setP2(Point3 p2) { m_p2 = p2; }

private:
    Point3 m_p0;
    Point3 m_p1;
    Point3 m_p2;
    std::shared_ptr<TextureMaterial> m_textureMaterial;
};