#pragma once

#include "Vector.h"
#include "Utils.h"

class Camera
{
public:
    Camera() = default;

    Camera(Point3 lookFrom, Point3 lookAt, Vector3 up, double vfov, double aspectRatio)
    {
        auto theta = vfov * M_PI / 180.0;
        auto h = tan(theta / 2);
        auto viewportHeight = 2.0 * h;
        auto viewportWidth = viewportHeight;

        auto w = Normalize(lookFrom - lookAt);
        auto u = Normalize(Cross(up, w));
        auto v = Cross(w, u);

        m_origin = lookFrom;
        m_horizontal = viewportWidth * u;
        m_vertical = viewportHeight * v;
        m_BottomLeftCorner = m_origin - m_horizontal / 2 - m_vertical / 2 - w;
    }

    Ray getRay(double s, double t) const
    {
        return Ray(m_origin, m_BottomLeftCorner + s * m_horizontal + t * m_vertical - m_origin);
    }

private:
    Point3 m_origin;
    Point3 m_BottomLeftCorner;
    Vector3 m_horizontal;
    Vector3 m_vertical;
};
