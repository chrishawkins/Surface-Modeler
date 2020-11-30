#include "pch.h"
#include "CppUnitTest.h"
#include "TestLogging.h"

#include "../Surface Modeler/Vector.h"
#include "../Surface Modeler/Vector.cpp"

using namespace SurfaceModeler;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(TestVectorArithmetic)
		{
			auto a = Vector(1, 2, 3);
			auto b = Vector(3, 2, 3);

			Assert::AreEqual(Vector(4, 4, 6), a + b);
			Assert::AreEqual(Vector(-2, 0, 0), a - b);
			Assert::AreEqual(Vector(2, 4, 6), a * 2.0f);
			Assert::AreEqual(Vector(0.5f, 1, 1.5f), a / 2.0f);
		}

		TEST_METHOD(TestVectorDotProduct)
		{
			auto a = Vector(4, 8, 10);
			auto b = Vector(9, 2, 7);

			Assert::AreEqual(a * b, 122.0f);
		}

		TEST_METHOD(TestVectorReflection)
		{
			auto incident = Vector(-3, -3, 0);
			auto normal = Vector(0, 1, 0);

			Assert::AreEqual(Vector(-3, 3, 0), Vector::reflect(incident, normal));
		}
	};
}
