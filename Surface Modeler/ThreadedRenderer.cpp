#include "ThreadedRenderer.h"

#include <thread>

namespace SurfaceModeler {

	void ThreadedRenderer::initialize() {
		rendererThread = std::unique_ptr<std::thread>(new std::thread([&] {
			while (!_exiting) {
				if (!_isRendering) {
					continue;
				}

				renderer->render();
				onRender();
				_isRendering = false;
			}
			}));
		_initializing = false;
	}

	void ThreadedRenderer::startRender() {
		if (_isRendering) {
			return;
		}

		_isRendering = true;
	}

	void ThreadedRenderer::exit() {
		_exiting = true;
		rendererThread->join();
	}
}
