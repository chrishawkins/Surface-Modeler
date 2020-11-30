#pragma once

#include <string>

#include "../Surface Modeler/Vector.h"

using namespace SurfaceModeler;
using namespace std;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			static wstring ToString(const Vector& vector) {
				auto ss = wstringstream();
				ss << "{ x = " << vector.x << ", y = " << vector.y << ", z = " << vector.z << " }";
				return ss.str();
			}
		}
	}
}