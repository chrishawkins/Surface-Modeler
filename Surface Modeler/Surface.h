#pragma once

#include "Vector.h"

namespace SurfaceModeler {

	class Surface
	{
	public:
		virtual float calculate(const Vector& point) const = 0;
	};

}