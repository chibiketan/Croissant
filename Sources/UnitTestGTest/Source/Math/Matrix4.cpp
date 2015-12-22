#include "Math/Matrix4.hpp"

#include "gtest/gtest.h"
#include <cmath>

#define PI 3.14f

class Matrix4Test : public ::testing::Test
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

TEST_F(Matrix4Test, DefaultConstructorIsIdentity)
{
	Croissant::Math::Matrix4 matrix;

	// line 1
	ASSERT_FLOAT_EQ(1.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	// line 2
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(1.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	// line 3
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(3.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	// line 4
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(0.0f, matrix(0, 0));
	ASSERT_FLOAT_EQ(1.0f, matrix(0, 0));
}

TEST_F(Matrix4Test, MultiplicationOfIdentity)
{
	Croissant::Math::Matrix4 first;
	Croissant::Math::Matrix4 second;
	Croissant::Math::Matrix4 expected;
	auto res = first * second;

	ASSERT_EQ(expected, res);
}

TEST_F(Matrix4Test, MultiplicationOfYawPitchRoll)
{
	auto angleX = 10.0f * PI / 180.0f;
	auto angleY = 20.0f * PI / 180.0f;
	auto angleZ = 30.0f * PI / 180.0f;
	Croissant::Math::Matrix4 matrix;

	auto sinX = std::sinf(angleX);
	auto cosX = std::cosf(angleX);
	auto sinY = std::sinf(angleY);
	auto cosY = std::cosf(angleY);
	auto sinZ = std::sinf(angleZ);
	auto cosZ = std::cosf(angleZ);
	// source : http://planning.cs.uiuc.edu/node102.html
	auto expected = Croissant::Math::Matrix4({
		cosZ * cosY,	cosZ * sinY * sinX - sinZ * cosX,	cosZ * sinY * cosX + sinZ * sinX,	0.0f,
		sinZ * cosY,	sinZ * sinY * sinX + cosZ * cosX,	sinZ * sinY * cosX - cosZ * sinX,	0.0f,
		-sinY,			cosY * sinX,						cosY * cosX,						0.0f,
		0.0f,			0.0f,								0.0f,								1.0f
	});

	auto yaw = Croissant::Math::Matrix4({
		cosZ,	-sinZ,	0.0f,	0.0f,
		sinZ,	cosZ,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	});

	// rotation sur Y
	auto pitch = Croissant::Math::Matrix4({
		cosY,	0.0f,	sinY,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		-sinY,	0.0f,	cosY,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	});

	// rotation sur X
	auto roll = Croissant::Math::Matrix4({
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	cosX,	-sinX,	0.0f,
		0.0f,	sinX,	cosX,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	});

	auto rotationTotal = yaw * pitch * roll;

	EXPECT_EQ(expected, rotationTotal);

}
