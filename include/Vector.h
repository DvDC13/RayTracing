#pragma once

#include <cmath>
#include <iostream>

#include "Utils.h"

class Vector3
{
public:
    Vector3() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
    Vector3(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}
    Vector3(const Vector3& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {}

    inline double getX() const { return m_x; }
    inline double getY() const { return m_y; }
    inline double getZ() const { return m_z; }

    Vector3& operator=(const Vector3& other) { m_x = other.m_x; m_y = other.m_y; m_z = other.m_z; return *this; }

    Vector3 operator-() const { return Vector3(-m_x, -m_y, -m_z); }

    Vector3& operator+=(const Vector3& other)
    {
        m_x += other.m_x;
        m_y += other.m_y;
        m_z += other.m_z;
        return *this;
    }

    Vector3& operator*=(double scalar)
    {
        m_x *= scalar;
        m_y *= scalar;
        m_z *= scalar;
        return *this;
    }

    Vector3& operator/=(double scalar)
    {
        return *this *= (1.0f / scalar);
    }

    double LengthSquared() const
    {
        return m_x * m_x + m_y * m_y + m_z * m_z;
    }

    double Length() const
    {
        return std::sqrt(LengthSquared());
    }

    inline static Vector3 random() { return Vector3(RandomDouble(), RandomDouble(), RandomDouble()); }

    inline static Vector3 random(double min, double max)
    { 
        return Vector3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
    }

    inline bool nearZero() const
    {
        const double s = 1e-8;
        return (std::fabs(m_x) < s) && (std::fabs(m_y) < s) && (std::fabs(m_z) < s);
    }

    private:
        double m_x;
        double m_y;
        double m_z;
};

inline Vector3 operator+(const Vector3& left, const Vector3& right)
{
    return Vector3(left.getX() + right.getX(), left.getY() + right.getY(), left.getZ() + right.getZ());
}

inline Vector3 operator-(const Vector3& left, const Vector3& right)
{
    return Vector3(left.getX() - right.getX(), left.getY() - right.getY(), left.getZ() - right.getZ());
}    

inline Vector3 operator*(const Vector3& left, const Vector3& right)
{
    return Vector3(left.getX() * right.getX(), left.getY() * right.getY(), left.getZ() * right.getZ());
}

inline Vector3 operator*(const Vector3& left, double scalar)
{
    return Vector3(left.getX() * scalar, left.getY() * scalar, left.getZ() * scalar);
}

inline Vector3 operator*(double scalar, const Vector3& right)
{
    return right * scalar;
}

inline Vector3 operator/(const Vector3& left, double scalar)
{
    return left * (1.0f / scalar);
}

inline double Dot(const Vector3& left, const Vector3& right)
{
    return left.getX() * right.getX() + left.getY() * right.getY() + left.getZ() * right.getZ();
}

inline Vector3 Cross(const Vector3& left, const Vector3& right)
{
    return Vector3(
        left.getY() * right.getZ() - left.getZ() * right.getY(),
        left.getZ() * right.getX() - left.getX() * right.getZ(),
        left.getX() * right.getY() - left.getY() * right.getX()
    );
}

inline Vector3 Normalize(const Vector3& vector)
{
    return vector / vector.Length();
}

Vector3 RandomInUnitSphereVector()
{
    while (true)
    {
        auto p = Vector3::random(-1,1);
        if (p.LengthSquared() >= 1) continue;
        return Normalize(p);
    }
}

inline Vector3 reflect(const Vector3& v, const Vector3& n)
{
    return v - 2 * Dot(v, n) * n;
}

using Point3 = Vector3;
using Color3 = Vector3;