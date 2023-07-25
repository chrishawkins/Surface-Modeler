#pragma once

#include <exception>
#include <memory>

using std::shared_ptr;

namespace SurfaceModeler {

	enum class SceneOperationParamType {
		INT,
		FLOAT,
		VECTOR
	};

	class SceneOperationMissingParameterTypeException : public std::exception
	{
		virtual const char* what() const throw() {
			return "Missing parameter type for scene operation";
		}
	};

	struct SceneOperationParamLayout {
		char numParams;
		shared_ptr<SceneOperationParamType[]> layout;

		SceneOperationParamLayout(char numParams, SceneOperationParamType* layout) : numParams(numParams), layout(layout) {}

		static char paramSize(SceneOperationParamType paramType) {
			switch (paramType) {
			case SceneOperationParamType::INT:
				return sizeof(int);
			case SceneOperationParamType::FLOAT:
				return sizeof(float);
			case SceneOperationParamType::VECTOR:
				return 3 * sizeof(float);
			default: 
				throw SceneOperationMissingParameterTypeException();
			}
		}

		char calculateByteOffset(char paramIndex) {
			char offset = 0;
			for (int i = 0; i <= paramIndex; i++) {
				offset += paramSize(layout[i]);
			}
			return offset;
		}
	};

	namespace SceneOperationParamLayouts {
		static SceneOperationParamLayout boxLayout = SceneOperationParamLayout(2, new SceneOperationParamType[]{ SceneOperationParamType::VECTOR, SceneOperationParamType::FLOAT });
		static SceneOperationParamLayout sphereLayout = SceneOperationParamLayout(1, new SceneOperationParamType[]{ SceneOperationParamType::FLOAT });
	}
}
