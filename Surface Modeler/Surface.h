#pragma once

#include "Vector.h"

using namespace MathUtils;

namespace SurfaceModeler {

	class Surface
	{
	public:
		virtual float calculate(const Vector& point) const = 0;
	};

}