#include "Math/Vector4.hpp"

#include "gtest/gtest.h"

class Vector4Test : public ::testing::Test
{
public:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

TEST_F(Vector4Test, ConstructBaseVector)
{
	Croissant::Math::Vector4 t(4, 5, 6);

	ASSERT_EQ(4.0f, t.X());
	ASSERT_EQ(5.0f, t.Y());
	ASSERT_EQ(6.0f, t.Z());
	ASSERT_EQ(0.0f, t.W());
}
