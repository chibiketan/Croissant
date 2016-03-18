#include "Math/Matrix4.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector4.hpp"
#include "Math/Point4.hpp"

#include "gtest/gtest.h"

#define PI 3.14f

class QuaternionTest : public ::testing::Test
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

// ---------------------------------------------------------------- Rotation axe X
TEST_F(QuaternionTest, RotationDe0SurAxeX)
{
	Croissant::Math::Point4 expected{ 0.0f, 0.0f, 1.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 1.0f, 0.0f, 0.0f }, 0.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 0.0f, 0.0f, 1.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe90SurAxeX)
{
	Croissant::Math::Point4 expected{ 0.0f, -1.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 1.0f, 0.0f, 0.0f }, 90.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 0.0f, 0.0f, 1.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe180SurAxeX)
{
	Croissant::Math::Point4 expected{ 0.0f, 0.0f, -1.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 1.0f, 0.0f, 0.0f }, 180.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 0.0f, 0.0f, 1.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe270SurAxeX)
{
	Croissant::Math::Point4 expected{ 0.0f, 1.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 1.0f, 0.0f, 0.0f }, 270.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 0.0f, 0.0f, 1.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe360SurAxeX)
{
	Croissant::Math::Point4 expected{ 0.0f, 0.0f, 1.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 1.0f, 0.0f, 0.0f }, 360.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 0.0f, 0.0f, 1.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

// ---------------------------------------------------------------- Rotation axe Y
TEST_F(QuaternionTest, RotationDe0SurAxeY)
{
	Croissant::Math::Point4 expected{ 1.0f, 0.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 0.0f, 1.0f, 0.0f }, 0.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe90SurAxeY)
{
	Croissant::Math::Point4 expected{ 0.0f, 0.0f, -1.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 0.0f, 1.0f, 0.0f }, 90.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe180SurAxeY)
{
	Croissant::Math::Point4 expected{ -1.0f, 0.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 0.0f, 1.0f, 0.0f }, 180.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe270SurAxeY)
{
	Croissant::Math::Point4 expected{ 0.0f, 0.0f, 1.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 0.0f, 1.0f, 0.0f }, 270.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe360SurAxeY)
{
	Croissant::Math::Point4 expected{ 1.0f, 0.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 0.0f, 1.0f, 0.0f }, 360.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

// ---------------------------------------------------------------- Rotation axe Z
TEST_F(QuaternionTest, RotationDe0SurAxeZ)
{
	Croissant::Math::Point4 expected{ 1.0f, 0.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 0.0f, 0.0f, 1.0f }, 0.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe90SurAxeZ)
{
	Croissant::Math::Point4 expected{ 0.0f, -1.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4 { 0.0f, 0.0f, 1.0f  }, 90.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe180SurAxeZ)
{
	Croissant::Math::Point4 expected{ -1.0f, 0.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 0.0f, 0.0f, 1.0f }, 180.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe270SurAxeZ)
{
	Croissant::Math::Point4 expected{ 0.0f, 1.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 0.0f, 0.0f, 1.0f }, 270.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}

TEST_F(QuaternionTest, RotationDe360SurAxeZ)
{
	Croissant::Math::Point4 expected{ 1.0f, 0.0f, 0.0f };
	Croissant::Math::Quaternion q{ Croissant::Math::Vector4{ 0.0f, 0.0f, 1.0f }, 360.0f * PI / 180.0f };
	Croissant::Math::Point4 p{ 1.0f, 0.0f, 0.0f };
	auto pPrime = p * q.ToMatrix();

	ASSERT_EQ(expected, pPrime);
}
