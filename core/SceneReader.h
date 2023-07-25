#pragma once

#include <vector>
#include <memory>

#include "Vector.h"
#include "Scene.h"
#include "Surface.h"

using namespace MathUtils;
using namespace std;

namespace SurfaceModeler {

	class SceneReader
	{
	public:
		vector<shared_ptr<const Surface>> surfacesForScene(const Scene& scene) const;
	};

}