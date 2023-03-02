#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Utils.h"

class Blob
{
public:
    Blob(unsigned int e, unsigned int d, Point3 position, double intensity)
    {
        m_e = e;
        m_d = d;

        m_potentials[0].position = position;
        m_potentials[0].intensity = intensity;
    }

    Mesh marchCubes()
    {
        Mesh mesh;

        auto material_ground = std::make_shared<UniformTexture>(Color3(0.8, 0.8, 0.0), 0.5f, 0.5f);

        for (int x = 0; x < m_e; x += m_d)
        {
            for (int y = 0; y < m_e; y += m_d)
            {
                for (int z = 0; z < m_e; z += m_d)
                {
                    // Potential cube
                    Point3 p7(x, y, z);
                    Point3 p6(x + m_d, y, z);
                    Point3 p2(x + m_d, y + m_d, z);
                    Point3 p3(x, y + m_d, z);
                    Point3 p4(x, y, z + m_d);
                    Point3 p5(x + m_d, y, z + m_d);
                    Point3 p1(x + m_d, y + m_d, z + m_d);
                    Point3 p8(x, y + m_d, z + m_d);

                    // Potential values
                    double v1 = getPotential(p1);
                    double v2 = getPotential(p2);
                    double v3 = getPotential(p3);
                    double v4 = getPotential(p4);
                    double v5 = getPotential(p5);
                    double v6 = getPotential(p6);
                    double v7 = getPotential(p7);
                    double v8 = getPotential(p8);

                    // Threshold
                    double Threshold = 1.0;
                    int index = 0;

                    if (v1 < Threshold) index |= 1;
                    if (v2 < Threshold) index |= 2;
                    if (v3 < Threshold) index |= 4;
                    if (v4 < Threshold) index |= 8;
                    if (v5 < Threshold) index |= 16;
                    if (v6 < Threshold) index |= 32;
                    if (v7 < Threshold) index |= 64;
                    if (v8 < Threshold) index |= 128;

                    char* line = lookUpTable[index];

                    for (size_t i = 0; i < 15; i += 3)
                    {
                        if (line[i] == -1)
                            break;

                        Point3 p1 = getMidPoint(line[i], p1);
                        Point3 p2 = getMidPoint(line[i + 1], p1);
                        Point3 p3 = getMidPoint(line[i + 2], p1);


                        Triangle t(p1, p2, p3, material_ground);

                        mesh.addTriangle(t);
                    }
                }
            }
        }

        return mesh;   
    }

private:
    unsigned int m_e;
    unsigned int m_d;

    struct Potential
    {
        Point3 position;
        double intensity;
    };

    Potential m_potentials[8];

    double getPotential(Point3 p)
    {
        double sum = 0.0;
        for (int i = 0; i < 8; i++)
        {
            sum += m_potentials[i].intensity / (p - m_potentials[i].position).Length();
        }

        return sum;
    }

    Point3 getMidPoint(int index, Point3 p)
    {
        switch (index)
        {
        case 0:
            return p + Point3(0.5, 1, 1) * m_d;
        case 1:
            return p + Point3(1, 1, 0.5) * m_d;
        case 2:
            return p + Point3(0.5, 1, 0) * m_d;
        case 3:
            return p + Point3(0, 1, 0.5) * m_d;
        case 4:
            return p + Point3(0.5, 0, 1) * m_d;
        case 5:
            return p + Point3(1, 0, 0.5) * m_d;
        case 6:
            return p + Point3(0.5, 0, 0) * m_d;
        case 7:
            return p + Point3(0, 0, 0.5) * m_d;
        case 8:
            return p + Point3(0, 1, 0.5) * m_d;
        case 9:
            return p + Point3(1, 0.5, 1) * m_d;
        case 10:
            return p + Point3(1, 0.5, 0) * m_d;
        case 11:
            return p + Point3(0, 0.5, 0) * m_d;
        default:
            std::runtime_error("Invalid index" + index);
        }
    }
};