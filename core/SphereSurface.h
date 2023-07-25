#pragma once

#include "Surface.h"

namespace SurfaceModeler {

    class SphereSurface : public Surface
    {
    public:
        float radius;

        SphereSurface(float radius = 1.0f): radius(radius) { }

        float calculate(const Vector& point) const;
    };

}