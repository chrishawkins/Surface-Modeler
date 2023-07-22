#pragma once

#include <memory>
#include <exception>

#include "Vector.h"
#include "SceneOperationParamLayout.h"

using namespace MathUtils;

namespace SurfaceModeler {

	constexpr auto PARAM_BUFFER_BYTES = 48;

	enum class SceneOperationType {
		BOX,
		SPHERE
	};

	class SceneOperationMissingParameterLayoutException : public std::exception
	{
		virtual const char* what() const throw() {
			return "Missing parameter layout for scene operation type";
		}
	};

	class SceneOperation
	{
	public:
		SceneOperation() : SceneOperation(SceneOperationType::BOX) {}

		SceneOperation(SceneOperationType type) : type(type), paramBuffer() {}

		SceneOperationType getType() {
			return type;
		}

		void serialize(char* buffer, int startIndex) const {
			memcpy(buffer + startIndex, &type, sizeof(char));
			memcpy(buffer + startIndex + 1, paramBuffer, PARAM_BUFFER_BYTES);
		}

		void deserialize(const char* const buffer, int startIndex) {
			memcpy(&type, buffer + startIndex, sizeof(char));
			memcpy(paramBuffer, buffer + startIndex + 1, PARAM_BUFFER_BYTES);
		}

		// TODO refactor these impls out of the header so I don't expose Parameter Layouts outside of the FX

		int getIntParam(int index) const {
			char offset = layoutForType(type).calculateByteOffset(index);
			return readIntParam(offset);
		}

		float getFloatParam(int index) const {
			char offset = layoutForType(type).calculateByteOffset(index);
			return readFloatParam(offset);
		}

		Vector getVectorParam(int index) const {
			char offset = layoutForType(type).calculateByteOffset(index);
			return readVectorParam(offset);
		}

		void setIntParam(int index, int value) {
			char offset = layoutForType(type).calculateByteOffset(index);
			return writeIntParam(offset, value);
		}

		void setFloatParam(int index, float value) {
			char offset = layoutForType(type).calculateByteOffset(index);
			return writeFloatParam(offset, value);
		}

		void setVectorParam(int index, const Vector& value) {
			char offset = layoutForType(type).calculateByteOffset(index);
			return writeVectorParam(offset, value);
		}

		static SceneOperation box(const Vector& size, float roundedness) {
			SceneOperation op(SceneOperationType::BOX);
			op.setVectorParam(0, size);
			op.setFloatParam(1, roundedness);
			return op;
		}

		static SceneOperation sphere(float radius) {
			SceneOperation op(SceneOperationType::SPHERE);
			op.setVectorParam(0, radius);
			return op;
		}

	private:
		SceneOperationType type;
		char paramBuffer[PARAM_BUFFER_BYTES];

		int readIntParam(char startIndex) const {
			int value;
			memcpy(&value, paramBuffer + startIndex, sizeof(int));
			return value;
		}

		float readFloatParam(char startIndex) const {
			float value;
			memcpy(&value, paramBuffer + startIndex, sizeof(float));
			return value;
		}

		Vector readVectorParam(char startIndex) const {
			float x = readFloatParam(startIndex);
			float y = readFloatParam(startIndex + sizeof(float));
			float z = readFloatParam(startIndex + 2 * sizeof(float));
			return Vector(x, y, z);
		}

		void writeIntParam(char startIndex, int value) {
			memcpy(paramBuffer + startIndex, &value, sizeof(int));
		}

		void writeFloatParam(char startIndex, float value) {
			memcpy(paramBuffer + startIndex, &value, sizeof(float));
		}

		void writeVectorParam(char startIndex, const Vector& value) {
			writeFloatParam(startIndex, value.x);
			writeFloatParam(startIndex + sizeof(float), value.y);
			writeFloatParam(startIndex + 2 * sizeof(float), value.z);
		}

		static SceneOperationParamLayout layoutForType(SceneOperationType type) {
			switch (type) {
			case SceneOperationType::BOX:
				return SceneOperationParamLayouts::boxLayout;
			case SceneOperationType::SPHERE:
				return SceneOperationParamLayouts::sphereLayout;
			default:
				throw SceneOperationMissingParameterLayoutException();
			}
		}
	};

}

