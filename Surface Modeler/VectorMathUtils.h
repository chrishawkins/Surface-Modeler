#pragma once

#include "Vector.h"

namespace MathUtils {

	inline float mina(float* nums, int n) {
		float minValue = nums[0];
		for (int i = 1; i < n; i++) {
			if (nums[i] < minValue) {
				minValue = nums[i];
			}
		}
		return minValue;
	}

	inline float maxa(float* nums, int n) {
		float maxValue = nums[0];
		for (int i = 1; i < n; i++) {
			if (nums[i] > maxValue) {
				maxValue = nums[i];
			}
		}
		return maxValue;
	}

	inline Vector minv(Vector a, float b) {
		return Vector(
			minf(a.x, b),
			minf(a.y, b),
			minf(a.z, b)
		);
	}

	inline Vector maxv(Vector a, float b) {
		return Vector(
			maxf(a.x, b),
			maxf(a.y, b),
			maxf(a.z, b)
		);
	}
}