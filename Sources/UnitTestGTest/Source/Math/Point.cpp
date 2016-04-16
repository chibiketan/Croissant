#include "Debug/MemoryManager.hpp"
#include "Math/Point4.hpp"
#include "Math/EulerAngle.hpp"
#include "Math/Math.hpp"
#include "Math/Matrix4.hpp"
//#include "Math/Point.hpp"
//#include "Math/Vector.hpp"
//
#include "gtest/gtest.h"

class PointTest : public ::testing::Test
{
public:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

protected:

};

TEST_F(PointTest, RotateWithEulerAngle)
{
	Croissant::Math::EulerAngle angle{ 0.0f, 0.0f, 45.0f };
	Croissant::Math::Point4 point{ 0.0, 1.0, 0.0 };
	Croissant::Math::Point4 expected{ 0.0, -1.0, 0.0 };

	auto result = point * Croissant::Math::ToMatrix(Croissant::Math::ToQuaternion(angle));

	ASSERT_EQ(expected, result);
}

//
//TEST_F(PointTest, CanCreateSimplePointAndGetProperties)
//{
//	Croissant::Math::Point point(1.0f, 1.0f, 1.0f);
//
//	ASSERT_EQ(1.0f, point.X());
//	ASSERT_EQ(1.0f, point.Y());
//	ASSERT_EQ(1.0f, point.Z());
//}
//
//TEST_F(PointTest, TwoPointsWithSameCoordonatesAreEqual)
//{
//	Croissant::Math::Point point(1.45f, 1.32f, 0.765f);
//	Croissant::Math::Point copy(1.45f, 1.32f, 0.765f);
//
//	ASSERT_EQ(point.X(), copy.X());
//	ASSERT_EQ(point.Y(), copy.Y());
//	ASSERT_EQ(point.Z(), copy.Z());
//	ASSERT_EQ(point, copy);
//}
//
//TEST_F(PointTest, TwoPointsWithDifferentCoordonatesAreNotEqual)
//{
//	Croissant::Math::Point point(1.45f, 1.32f, 0.765f);
//	Croissant::Math::Point copy(1.45f, 1.32f, 6.35f);
//
//	ASSERT_EQ(point.X(), copy.X());
//	ASSERT_EQ(point.Y(), copy.Y());
//	ASSERT_NE(point.Z(), copy.Z());
//	ASSERT_NE(point, copy);
//}
//
//TEST_F(PointTest, CanCopyPointByConstructorToGetEqualPoint)
//{
//	Croissant::Math::Point point(1.45f, 1.32f, 0.765f);
//	Croissant::Math::Point copy(point);
//
//	ASSERT_EQ(point, copy);
//}
//
//TEST_F(PointTest, CanCopyPointByAffectationToGetEqualPoint)
//{
//	Croissant::Math::Point point(1.45f, 1.32f, 0.765f);
//	Croissant::Math::Point copy;
//		
//	copy = point;
//	ASSERT_EQ(point, copy);
//}
//
//TEST_F(PointTest, DifferenceBeteenTwoPointGiveAVector)
//{
//	Croissant::Math::Point point1(1.0f, 1.0f, 1.0f);
//	Croissant::Math::Point point2(2.0f, 2.0f, 2.0f);
//	auto vector = point1 - point2;
//
//	ASSERT_EQ(point1.X() - point2.X(), vector.X());
//	ASSERT_EQ(point1.Y() - point2.Y(), vector.Y());
//	ASSERT_EQ(point1.Z() - point2.Z(), vector.Z());
//}
//
//
//
