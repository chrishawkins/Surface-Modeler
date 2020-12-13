#pragma once

#include "Scene.h"

namespace SurfaceModeler {
	namespace SampleScenes {

		Scene roundedCube() {
			Scene scene;
			scene.sceneOperations.push_back(SceneOperation::box(Vector(0.5f, 0.2f, 0.5f), 0.05f));
			return scene;
		}

		Scene sphere() {
			Scene scene;
			scene.sceneOperations.push_back(SceneOperation::sphere(0.5f));
			return scene;
		}

	}
}