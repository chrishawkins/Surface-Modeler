#include "BoxSurface.h"
#include "MathUtils.h"
#include "VectorMathUtils.h"

using namespace SurfaceModeler;
using namespace MathUtils;

float BoxSurface::calculate(const Vector& point) const {
	auto q = point.abs() - size;
	return maxv(q, 0.0).length() + minf(maxf(q.x, maxf(q.y, q.z)), 0.0) - roundedness;
}
