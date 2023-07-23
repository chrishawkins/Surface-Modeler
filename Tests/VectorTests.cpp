#include <gtest/gtest.h>

#include "../SurfaceModeler/Vector.h"
#include "../SurfaceModeler/Vector.cpp"

using namespace MathUtils;

TEST(VectorMath, testArithmetic) {
  auto a = Vector(1, 2, 3);
	auto b = Vector(3, 2, 3);

	ASSERT_EQ(Vector(4, 4, 6), a + b) << "";
	ASSERT_EQ(Vector(-2, 0, 0), a - b);
	ASSERT_EQ(Vector(2, 4, 6), a * 2.0f);
	ASSERT_EQ(Vector(0.5f, 1, 1.5f), a / 2.0f);
}

TEST(VectorMath, testDotProduct)
{
	auto a = Vector(4, 8, 10);
	auto b = Vector(9, 2, 7);

	ASSERT_EQ(a * b, 122.0f);
}

TEST(VectorMath, testReflect)
{
	auto incident = Vector(-3, -3, 0);
	auto normal = Vector(0, 1, 0);

	ASSERT_EQ(Vector(-3, 3, 0), Vector::reflect(incident, normal));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
