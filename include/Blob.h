#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Utils.h"

class Blob
{
public:
    Blob(Point3 position, double e, double d, double threshold, std::shared_ptr<TextureMaterial> TextureMaterial)
        : m_position(position), m_e(e), m_d(d), m_threshold(threshold), m_TextureMaterial(TextureMaterial)
    {}

    Mesh marchCubes()
    {
        Mesh mesh;

        for (double x = m_position.getX(); x < m_e; x += m_d)
        {
            for (double y = m_position.getY(); y < m_e; y += m_d)
            {
                for (double z = m_position.getZ(); z < m_e; z += m_d)
                {
                    Point3 p(x, y, z);
                    processMarchCube(p, mesh);

                    std::cout << "Marching cube: " << x << ", " << y << ", " << z << std::endl;
                }
            }
        }

        return mesh;   
    }

    void processMarchCube(Point3& p, Mesh& mesh)
    {
        getCorners(p);
        int index = getIndex();
        getVertices(index);
        addTriangles(mesh);

        m_corners.clear();
        m_vertices.clear();
    }

private:
    Point3 m_position;
    double m_e;
    double m_d;
    double m_threshold;
    std::shared_ptr<TextureMaterial> m_TextureMaterial;

    double m_pointsPotential[8];
    std::vector<Point3> m_corners;
    std::vector<Point3> m_vertices;

    struct Edge
    {
        int vp1;
        int vp2;
    };

    void getCorners(Point3 p)
    {
        m_corners.push_back(p + Point3(0, 0, 0));
        m_corners.push_back(p + Point3(m_d, 0, 0));
        m_corners.push_back(p + Point3(m_d, 0, m_d));
        m_corners.push_back(p + Point3(0, 0, m_d));
        m_corners.push_back(p + Point3(0, m_d, 0));
        m_corners.push_back(p + Point3(m_d, m_d, 0));
        m_corners.push_back(p + Point3(m_d, m_d, m_d));
        m_corners.push_back(p + Point3(0, m_d, m_d));
    }

    double getPotential(Point3 p)
    {
        Point3 center = Point3(m_e / 2, m_e / 2, m_e / 2);
        double r = (p - center).Length();
        double d = r - m_threshold;
        return d;
    }

    int getIndex()
    {
        int index = 0;
        
        m_pointsPotential[0] = getPotential(m_corners[0]);
        m_pointsPotential[1] = getPotential(m_corners[1]);
        m_pointsPotential[2] = getPotential(m_corners[2]);
        m_pointsPotential[3] = getPotential(m_corners[3]);
        m_pointsPotential[4] = getPotential(m_corners[4]);
        m_pointsPotential[5] = getPotential(m_corners[5]);
        m_pointsPotential[6] = getPotential(m_corners[6]);
        m_pointsPotential[7] = getPotential(m_corners[7]);

        if (m_pointsPotential[0] < m_threshold) index |= 1;
        if (m_pointsPotential[1] < m_threshold) index |= 2;
        if (m_pointsPotential[2] < m_threshold) index |= 4;
        if (m_pointsPotential[3] < m_threshold) index |= 8;
        if (m_pointsPotential[4] < m_threshold) index |= 16;
        if (m_pointsPotential[5] < m_threshold) index |= 32;
        if (m_pointsPotential[6] < m_threshold) index |= 64;
        if (m_pointsPotential[7] < m_threshold) index |= 128;

        return index;
    }

    void getVertices(int index)
    {
        for (int i = 0; i < lookUpTableCols; i++)
        {
            int edgeIndex = lookUpTable[index][i];

            if (edgeIndex == -1) break;

            Edge edge = getVerticeIndex(edgeIndex);
            if (edge.vp1 == -1 || edge.vp2 == -1)
            {
                std::cout << "Error: Invalid edge index" << std::endl;
                return;
            }

            Point3 middleEdge = (m_corners[edge.vp1] + m_corners[edge.vp2]) / 2;

            m_vertices.push_back(middleEdge);
        }
    }
    
    Edge getVerticeIndex(int edgeIndex)
    {
        switch (edgeIndex)
        {
        case 0:
            return Edge{ 0, 1 };
        case 1:
            return Edge{ 1, 2 };
        case 2:
            return Edge{ 2, 3 };
        case 3:
            return Edge{ 3, 0 };
        case 4:
            return Edge{ 4, 5 };
        case 5:
            return Edge{ 5, 6 };
        case 6:
            return Edge{ 6, 7 };
        case 7:
            return Edge{ 7, 4 };
        case 8:
            return Edge{ 0, 4 };
        case 9:
            return Edge{ 1, 5 };
        case 10:
            return Edge{ 2, 6 };
        case 11:
            return Edge{ 3, 7 };
        default:
            return Edge{ -1, -1 };
        }
    }

    void addTriangles(Mesh& mesh)
    {
        for (int i = 0; i < m_vertices.size(); i += 3)
        {
            Point3 p1 = m_vertices[i];
            Point3 p2 = m_vertices[i + 1];
            Point3 p3 = m_vertices[i + 2];

            Triangle triangle(p1, p2, p3, m_TextureMaterial);
            mesh.addTriangle(triangle);
        }
    }
};