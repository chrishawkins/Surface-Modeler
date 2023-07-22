#include "SceneReader.h"

#include "BoxSurface.h"
#include "SphereSurface.h"

using namespace std;
using namespace SurfaceModeler;

vector<shared_ptr<const Surface>> SceneReader::surfacesForScene(const Scene& scene) const
{
    std::vector<shared_ptr<const Surface>> surfaces;

    for (size_t i = 0; i < scene.sceneOperations.size(); i++) {
        auto op = scene.sceneOperations[i];

        switch (op.getType()) {

            case SceneOperationType::BOX: {
                Vector center = op.getVectorParam(0);
                float roundedness = op.getFloatParam(1);
                surfaces.push_back(std::make_shared<const BoxSurface>(center, roundedness));
                break;
            }

            case SceneOperationType::SPHERE: {
                float radius = op.getFloatParam(0);
                surfaces.push_back(std::make_shared<const SphereSurface>(radius));
                break;
            }

        }
    }

    return surfaces;
}
