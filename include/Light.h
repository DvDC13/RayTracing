#pragma once

#include "Vector.h"

class Light
{
public:
    virtual ~Light() = default;

    virtual Vector3 getDirection(const Point3& point) const = 0;

    virtual double getDistance(const Point3& point) const = 0;

    virtual Color3 getColor() const = 0;
};

class PointLight : public Light {
public:
    PointLight(const Point3& position, const Color3& color, double intensity)
        : position(position), color(color), intensity(intensity) {}

    virtual Vector3 getDirection(const Point3& point) const override
    {
        return Normalize(position - point);
    }

    virtual double getDistance(const Point3& point) const override
    {
        return (position - point).Length();
    }

    virtual Color3 getColor() const override
    {
        return color * intensity;
    }

private:
    Point3 position;
    Color3 color;
    double intensity;
};
