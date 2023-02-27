#pragma once

#include "Vector.h"
#include "Utils.h"

class Camera
{
public:
    Camera()
    {
        m_beta = 2.0f; // Viewport height
        m_alpha = 2.0f; // Viewport width
        m_zmin = 1.0f; // Focal length

        m_center = Point3(0, 0, 0); // Camera position
        m_up = Vector3(0, m_beta, 0); // Camera up vector
        //m_target = Point3(0, 0, -m_zmin); // Camera target
    }

    Ray getRay(double u, double v) const
    {
        Vector3 horizontal = Vector3(m_alpha, 0, 0);
        Vector3 vertical = Vector3(0, m_beta, 0);
        Vector3 lower_left_corner = m_center - horizontal / 2 - vertical / 2 - Vector3(0, 0, m_zmin);
        Vector3 direction = lower_left_corner + u * horizontal + v * vertical - m_center;
        return Ray(m_center, direction);
    }

    inline Point3 getCenter() const { return m_center; }
    //inline Point3 getTarget() const { return m_target; }
    inline Vector3 getUp() const { return m_up; }
    inline double getAlpha() const { return m_alpha; }
    inline double getBeta() const { return m_beta; }
    inline double getZmin() const { return m_zmin; }

private:
    Point3 m_center;
    Point3 m_target;
    Vector3 m_up;
    double m_alpha;
    double m_beta;
    double m_zmin;
};
