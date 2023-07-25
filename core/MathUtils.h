#pragma once

namespace MathUtils {

	inline float clamp(float v, float min, float max) {
		if (v < min) {
			return min;
		}
		if (v > max) {
			return max;
		}
		return v;
	}

	inline float minf(float a, float b) {
		if (a < b) {
			return a;
		}
		return b;
	}

	inline float maxf(float a, float b) {
		if (a > b) {
			return a;
		}
		return b;
	}

	inline float squared(float x) {
		return x * x;
	}

}