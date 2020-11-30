#include "Vector.h"

using namespace SurfaceModeler;

const Vector Vector::ZERO = Vector(0.0f, 0.0f, 0.0f);
const Vector Vector::UNIT = Vector(1.0f, 1.0f, 1.0f);

Vector Vector::reflect(const Vector& incident, const Vector& normal) {
	return incident - 2.0f * (incident * normal) * normal;
}