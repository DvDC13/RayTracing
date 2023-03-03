#pragma once

#include "TextureMaterial.h"
#include "Object.h"

class Mesh : public Object
{
public:
    Mesh() = default;

    void addTriangle(Triangle triangle)
    {
        m_mesh.emplace_back(triangle);
    }

    void translate(Vector3 v)
    {
        for (auto& triangle : m_mesh)
        {
            triangle.setP0(triangle.getP0() + v);
            triangle.setP1(triangle.getP1() + v);
            triangle.setP2(triangle.getP2() + v);
        }
    }

    void addCube()
    {
        auto material_ground = std::make_shared<MirrorTexture>(Color3(0.0, 0.0, 0.8));

        Triangle tri1South = Triangle(
            Point3(0.0, 0.0, 0.0),
            Point3(0.0, 1.0, 0.0),
            Point3(1.0, 1.0, 0.0),
            material_ground
        );

        Triangle tri2South = Triangle(
            Point3(0.0, 0.0, 0.0),
            Point3(1.0, 1.0, 0.0),
            Point3(1.0, 0.0, 0.0),
            material_ground
        );

        Triangle tri1East = Triangle(
            Point3(1.0, 0.0, 0.0),
            Point3(1.0, 1.0, 0.0),
            Point3(1.0, 1.0, 1.0),
            material_ground
        );

        Triangle tri2East = Triangle(
            Point3(1.0, 0.0, 0.0),
            Point3(1.0, 1.0, 1.0),
            Point3(1.0, 0.0, 1.0),
            material_ground
        );

        Triangle tri1North = Triangle(
            Point3(1.0, 0.0, 1.0),
            Point3(1.0, 1.0, 1.0),
            Point3(0.0, 1.0, 1.0),
            material_ground
        );

        Triangle tri2North = Triangle(
            Point3(1.0, 0.0, 1.0),
            Point3(0.0, 1.0, 1.0),
            Point3(0.0, 0.0, 1.0),
            material_ground
        );

        Triangle tri1West = Triangle(
            Point3(0.0, 0.0, 1.0),
            Point3(0.0, 1.0, 1.0),
            Point3(0.0, 1.0, 0.0),
            material_ground
        );

        Triangle tri2West = Triangle(
            Point3(0.0, 0.0, 1.0),
            Point3(0.0, 1.0, 0.0),
            Point3(0.0, 0.0, 0.0),
            material_ground
        );

        Triangle tri1Top = Triangle(
            Point3(0.0, 1.0, 0.0),
            Point3(0.0, 1.0, 1.0),
            Point3(1.0, 1.0, 1.0),
            material_ground
        );

        Triangle tri2Top = Triangle(
            Point3(0.0, 1.0, 0.0),
            Point3(1.0, 1.0, 1.0),
            Point3(1.0, 1.0, 0.0),
            material_ground
        );

        Triangle tri1Bottom = Triangle(
            Point3(1.0, 0.0, 1.0),
            Point3(0.0, 0.0, 1.0),
            Point3(0.0, 0.0, 0.0),
            material_ground
        );

        Triangle tri2Bottom = Triangle(
            Point3(1.0, 0.0, 1.0),
            Point3(0.0, 0.0, 0.0),
            Point3(1.0, 0.0, 0.0),
            material_ground
        );

        m_mesh.push_back(tri1South);
        m_mesh.push_back(tri2South);
        m_mesh.push_back(tri1East);
        m_mesh.push_back(tri2East);
        m_mesh.push_back(tri1North);
        m_mesh.push_back(tri2North);
        m_mesh.push_back(tri1West);
        m_mesh.push_back(tri2West);
        m_mesh.push_back(tri1Top);
        m_mesh.push_back(tri2Top);
        m_mesh.push_back(tri1Bottom);
        m_mesh.push_back(tri2Bottom);
    }

    virtual bool intersects(const Ray& ray, double t_min, double t_max, hit_record& record) const override
    {
        hit_record tmp_record;
        bool hit = false;
        double closest_so_far = t_max;

        for (const auto& object : m_mesh)
        {
            if (object.intersects(ray, t_min, closest_so_far, tmp_record))
            {
                hit = true;
                closest_so_far = tmp_record.t;
                record = tmp_record;
            }
        }

        return hit;
    }

    virtual Vector3 normalAt(const Point3& point, const Ray& ray, hit_record& record) const override
    {
        return record.normal;
    }

    inline std::vector<Triangle> getMesh() const { return m_mesh; }

private:
    std::vector<Triangle> m_mesh;
};