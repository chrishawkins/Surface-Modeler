#pragma once

#include "Surface.h"

namespace SurfaceModeler {

    class BoxSurface : public Surface
    {
    public:
        Vector size;

        BoxSurface(Vector size = Vector::UNIT): size(size) {}

        float calculate(const Vector& point) const;
    };

}