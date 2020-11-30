#include "SphereSurface.h"

using namespace SurfaceModeler;

float SphereSurface::calculate(const Vector& point) const {
	return (center - point).length() - radius;
}