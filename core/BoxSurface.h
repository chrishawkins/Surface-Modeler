#pragma once

#include "Surface.h"

namespace SurfaceModeler {

    class BoxSurface : public Surface
    {
    public:
        Vector size;
        float roundedness;

        BoxSurface(Vector size = Vector::UNIT, float roundedness = 0.0f): size(size), roundedness(roundedness) {}

        float calculate(const Vector& point) const;
    };

}