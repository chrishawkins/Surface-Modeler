#include "Renderer.h"

#include "MathUtils.h"

using namespace SurfaceModeler;

#define MAX_MARCHING_STEPS 10000
#define MAX_DRAW_DISTANCE 100
#define MIN_DRAW_DISTANCE 0.1f
#define EPSILON 0.001f
#define HIT_COLOR Color(1.0f, 0.0f, 0.0f)
#define BLACK Color()

float Renderer::shortestDistanceToSurface(const Vector& eyePosition, const Vector& rayDirection) const {
	float depth = MIN_DRAW_DISTANCE;

	for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
		float distance = surface->calculate(
			eyePosition + rayDirection * depth
		);

		if (distance < EPSILON) {
			return depth;
		}

		depth += distance;

		if (depth >= MAX_DRAW_DISTANCE) {
			return MAX_DRAW_DISTANCE;
		}
	}

	return MAX_DRAW_DISTANCE;
}

Vector Renderer::estimateNormal(const Vector& point) const {
	return Vector(
		surface->calculate(Vector(point.x + EPSILON, point.y, point.z)) - surface->calculate(Vector(point.x - EPSILON, point.y, point.z)),
		surface->calculate(Vector(point.x, point.y + EPSILON, point.z)) - surface->calculate(Vector(point.x, point.y - EPSILON, point.z)),
		surface->calculate(Vector(point.x, point.y, point.z + EPSILON)) - surface->calculate(Vector(point.x, point.y, point.z - EPSILON))
	).normalize();
}


Vector Renderer::phongContrib(
	const Color& diffuse,
	const Color& specular,
	const float& shininess,
	const Vector& point,
	const Vector& eye,
	const Vector& lightPosition,
	const Vector& lightIntensity
) const {
	auto normal = estimateNormal(point);
	auto lightDirection = (lightPosition - point).normalize();
	auto rayDirection = (eye - point).normalize();
	auto reflection = Vector::reflect(-lightDirection, normal);

	float dotLightNormal = MathUtils::clamp(lightDirection * normal, 0.0f, 1.0f);
	float dotReflectionRay = reflection * rayDirection;

	if (dotLightNormal < 0) {
		return BLACK;
	}

	Vector color = lightIntensity * (diffuse * dotLightNormal);

	if (dotReflectionRay >= 0) {
		color += specular * pow(dotReflectionRay, shininess);
	}

	return Vector::clampComponents(color, 0.0f, 1.0f);
}

Vector Renderer::phongIllumination(
	const Color& ambient,
	const Color& diffuse,
	const Color& specular,
	const float& shininess,
	const Vector& point,
	const Vector& eye
) const {
	auto ambientLight = 0.5f * Vector::UNIT;
	auto color = ambientLight.componentMultiply(ambient);

	auto light1Position = Vector(4, 2, 4);
	auto light1Intensity = Vector(0.4f, 0.4f, 0.4f);

	color += phongContrib(diffuse, specular, shininess, point, eye,
		light1Position,
		light1Intensity);

	auto light2Position = Vector(2, 2, 2);
	auto light2Intensity = Vector(0.4f, 0.4f, 0.4f);

	color += phongContrib(diffuse, specular, shininess, point, eye,
		light2Position,
		light2Intensity);

	return Vector::clampComponents(color, 0.0f, 1.0f);
}

Color Renderer::renderRay(int x, int y) const {

	if (surface == nullptr) {
		return BLACK;
	}

	float aspectRatio = (float)width / height;
	float worldX = (2 * (x / (float)width) - 1) * aspectRatio;
	float worldY = 2 * (y / (float)height) - 1;

	Vector pixelPosition = Vector(worldX, worldY, 2);
	Vector eyePosition = Vector(worldX, worldY, 5);
	Vector rayDirection = (pixelPosition - eyePosition).normalize();

	float dist = shortestDistanceToSurface(eyePosition, rayDirection);

	if (dist > MAX_DRAW_DISTANCE - EPSILON) {
		return BLACK;
	}

	Vector point = eyePosition + dist * rayDirection;

	Color ambient = Color(0.2f, 0.2f, 0.2f);
	Color diffuse = Color(0.7f, 0.2f, 0.2f);
	Color specular = Color(1.0f, 1.0f, 1.0f);
	float shininess = 10.0f;

	return phongIllumination(ambient, diffuse, specular, shininess, point, eyePosition);
}

void Renderer::clearBuffer() {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			buffer[x][y] = BLACK;
		}
	}
}

void Renderer::render() {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			buffer[x][y] = renderRay(x, y);
		}
	}
}