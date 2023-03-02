#pragma once

#include "Vector.h"
#include "Utils.h"

class Camera
{
public:
    Camera()
    {
        m_lookFrom = Point3(-2, 1, 1);
        m_lookAt = Point3(0, 0, 0);
        m_up = Vector3(0, 1, 0);
        m_fov = 90.0f;
        m_aspect_ratio = 16.0f / 9.0f;

        auto tetha = m_fov * M_PI / 180.0;
        auto half_height = tan(tetha / 2);

        m_beta = 2.0f * half_height; // Viewport height
        m_alpha = m_aspect_ratio * m_beta; // Viewport width
        m_zmin = 1.0f; // Focal length

        m_w = Normalize(m_lookFrom - m_lookAt);
        m_u = Normalize(Cross(m_up, m_w));
        m_v = Cross(m_w, m_u);

        m_center = m_lookFrom; // Camera position
    }

    Ray getRay(double s, double t) const
    {
        Vector3 horizontal = m_alpha * m_u;
        Vector3 vertical = m_beta * m_v;
        Vector3 lower_left_corner = m_center - horizontal / 2 - vertical / 2 - m_zmin * m_w;
        Vector3 direction = lower_left_corner + s * horizontal + t * vertical - m_center;
        return Ray(m_center, direction);
    }

    inline Point3 getCenter() const { return m_center; }
    inline Vector3 getUp() const { return m_up; }
    inline double getAlpha() const { return m_alpha; }
    inline double getBeta() const { return m_beta; }
    inline double getZmin() const { return m_zmin; }
    inline double getFov() const { return m_fov; }
    inline double getAspectRatio() const { return m_aspect_ratio; }
    inline Point3 getLookFrom() const { return m_lookFrom; }
    inline Point3 getLookAt() const { return m_lookAt; }

private:
    Point3 m_center;
    double m_alpha;
    double m_beta;
    double m_zmin;

    double m_fov;
    double m_aspect_ratio;
    Point3 m_lookFrom;
    Point3 m_lookAt;
    Vector3 m_up;

    Vector3 m_u;
    Vector3 m_v;
    Vector3 m_w;
};
