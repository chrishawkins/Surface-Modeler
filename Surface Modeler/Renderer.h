#pragma once

#include <memory>

#include "Surface.h"
#include "Vector.h"

namespace SurfaceModeler {

	typedef Vector Color;

	class Renderer
	{
	public:
		int width, height;
		Vector cameraPosition;
		std::shared_ptr<Surface> surface;

		Renderer(int width, int height, const Vector& cameraPosition = Vector(0, 0, -10)) :
			width(width), height(height), cameraPosition(cameraPosition)
		{
			this->buffer = new Color * [width];
			for (int x = 0; x < width; x++) {
				this->buffer[x] = new Color[height];
			}
			clearBuffer();
		};

		void clearBuffer();
		void render();

		const Color& getBuffer(int x, int y) const {
			return buffer[x][y];
		}

	private:
		Color** buffer;

		// Core Renderer
		Color renderRay(int x, int y) const;
		float shortestDistanceToSurface(const Vector& eyePosition, const Vector& rayDirection) const;

		// Lighting Functions
		Vector estimateNormal(const Vector& point) const;
		Vector phongContrib(
			const Color& diffuse,
			const Color& specular,
			const float& shininess,
			const Vector& point,
			const Vector& eye,
			const Vector& lightPosition,
			const Vector& lightIntensity
		) const;

		Vector phongIllumination(
			const Color& ambient,
			const Color& diffuse,
			const Color& specular,
			const float& shininess,
			const Vector& point,
			const Vector& eye
		) const;
	};

}