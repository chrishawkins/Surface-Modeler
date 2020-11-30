#pragma once

#include <functional>
#include <memory>
#include <thread>

#include "Renderer.h"

namespace SurfaceModeler {

	class ThreadedRenderer
	{
	public:
		ThreadedRenderer(int width, int height, std::shared_ptr<Surface> surface, std::function<void()> onRender) :
			rendererThread(nullptr), onRender(onRender), _isRendering(false), _initializing(true), _exiting(false) {
			renderer = std::unique_ptr<Renderer>(new Renderer(width, height));
			renderer->surface = surface;
		}

		void initialize();
		void startRender();
		void exit();

		bool isRendering() const { return _isRendering; }
		int getWidth() const { return renderer->width; }
		int getHeight() const { return renderer->height; }

		const Color& getBuffer(int x, int y) const {
			if (_initializing) {
				return Color(0, 0, 0);
			}
			return renderer->getBuffer(x, y);
		}

	private:
		std::unique_ptr<Renderer> renderer;
		bool _isRendering;
		bool _initializing;
		bool _exiting;
		std::unique_ptr<std::thread> rendererThread;
		std::function<void()> onRender;
	};

}