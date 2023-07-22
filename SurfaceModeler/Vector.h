#pragma once

#include <cmath>
#include <cstdlib>

#include "MathUtils.h"

namespace MathUtils {

	class Vector
	{
	public:
		float x, y, z;

		Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		float lengthSquared() const {
			return x * x + y * y + z * z;
		}

		float length() const {
			return sqrt(lengthSquared());
		}

		Vector normalize() const {
			return operator/(length());
		}

		Vector abs() const {
			return Vector(std::abs(x), std::abs(y), std::abs(z));
		}

		float dot(const Vector& b) const {
			return x * b.x + y * b.y + z * b.z;
		}

		Vector componentMultiply(const Vector& b) const {
			return Vector(x * b.x, y * b.y, z * b.z);
		}

		Vector operator+(const Vector& b) const {
			return Vector(this->x + b.x, this->y + b.y, this->z + b.z);
		}

		Vector operator-() const {
			return Vector(-x, -y, -z);
		}

		Vector operator-(const Vector& b) const {
			return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
		}

		void operator+=(const Vector& b) {
			x += b.x;
			y += b.y;
			z += b.z;
		}

		void operator-=(const Vector& b) {
			x -= b.x;
			y -= b.y;
			z -= b.z;
		}

		Vector operator*(const float& m) const {
			return Vector(this->x * m, this->y * m, this->z * m);
		}

		Vector operator/(const float& d) const {
			return Vector(this->x / d, this->y / d, this->z / d);
		}

		float operator*(const Vector& b) const {
			return dot(b);
		}

		bool operator==(const Vector& b) const {
			return x == b.x && y == b.y && z == b.z;
		}

		static Vector reflect(const Vector& incident, const Vector& normal);

		static Vector clampComponents(const Vector& vec, float min, float max) {
			return Vector(
				MathUtils::clamp(vec.x, min, max),
				MathUtils::clamp(vec.y, min, max),
				MathUtils::clamp(vec.z, min, max)
			);
		}

		static const Vector ZERO, UNIT;
	};

	inline Vector operator*(const float& m, const Vector& v) {
		return v * m;
	}
}