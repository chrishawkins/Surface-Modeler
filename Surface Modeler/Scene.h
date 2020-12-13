#pragma once

#include <vector>

#include "SceneOperation.h"

namespace SurfaceModeler {

	using SceneOperationList = std::vector<SceneOperation>;

	class Scene
	{
	public:
		SceneOperationList sceneOperations;

		void serialize(char* const buffer) const {
			int numberOfOperations = sceneOperations.size();
			memcpy(buffer, &numberOfOperations, sizeof(int));
			for (int i = 0; i < numberOfOperations; i++) {
				int byteIndex = sizeof(int) + PARAM_BUFFER_BYTES * i;
				sceneOperations.at(i).serialize(buffer, byteIndex);
			}
		}

		void deserialize(const char* const buffer) {
			int numberOfOperations;
			memcpy(&numberOfOperations, buffer, sizeof(int));
			for (int i = 0; i < numberOfOperations; i++) {
				int byteIndex = sizeof(int) + PARAM_BUFFER_BYTES * i;
				SceneOperation op;
				op.deserialize(buffer, byteIndex);
				sceneOperations.push_back(op);
			}
		}
	};

}
