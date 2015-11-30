///*
// * String.cpp
// *
// *  Created on: 11 juil. 2013
// *      Author: gcompte
// */
//
//#include "Core/String.hpp"
//#include "gtest/gtest.h"
//#include "gtest/gtest-spi.h"
//#include <iostream>
//
//using String = Croissant::Core::String;
//using EncodingType = Croissant::Core::EncodingType;
//
//class StringTest : public ::testing::Test
//{
//public:
//	virtual void SetUp()
//	{
//	}
//
//	virtual void TearDown()
//	{
//	}
//};
//
//TEST_F(StringTest, ConversionLatin1ToUTF8)
//{
//	String test("\xE9""crit", EncodingType::LATIN1);
//
//	ASSERT_STREQ("\xC3\xA9""crit", test.Str());
//}
//
//TEST_F(StringTest, ConversionASCIIToUTF8)
//{
//	String test("ecrit", EncodingType::ASCII);
//
//	ASSERT_STREQ("ecrit", test.Str());
//}
//
//TEST_F(StringTest, ConversionASCIIToUTF8WithValidation)
//{
//	EXPECT_FATAL_FAILURE(String test("\xE9""crit", EncodingType::ASCII), "invalid");
//}
//
//TEST_F(StringTest, IndexReadAccessOnlyASCII)
//{
//	String test("ecrit", EncodingType::ASCII);
//	ASSERT_EQ('c', test[1]);
//}
//
//TEST_F(StringTest, IndexReadAccessWithLatin1Char)
//{
//	String test("\xE9""crit", EncodingType::LATIN1);
//
//	ASSERT_EQ('c', test[1]);
//	ASSERT_EQ(0xC3A9, test[0]);
//}
//
//TEST_F(StringTest, StandardIteratorConstructor)
//{
//	std::string standard("ecrit");
//	String custom("ecrit", EncodingType::ASCII);
//	std::string iteratorStandard(standard.begin(), standard.end());
//	String iteratorCustom(custom.Begin(), custom.End());
//
//	ASSERT_STREQ(standard.c_str(), custom.Str());
//	ASSERT_STREQ(iteratorStandard.c_str(), iteratorCustom.Str());
//	ASSERT_STREQ(std::string(standard.begin() + 2, standard.end()).c_str(), String(custom.Begin() + 2, custom.End()).Str());
//	ASSERT_STREQ(std::string(standard.begin(), standard.end() - 2).c_str(), String(custom.Begin(), custom.End() - 2).Str());
//	ASSERT_STREQ(std::string(standard.begin() + 2, standard.end() - 2).c_str(), String(custom.Begin() + 2, custom.End() - 2).Str());
//}
//
//TEST_F(StringTest, StandardReverseIteratorConstructor)
//{
//    std::string standard("ecrit");
//    String custom("ecrit", EncodingType::ASCII);
//    std::string iteratorStandard(standard.rend().base(), standard.rbegin().base());
//    String iteratorCustom(custom.REnd().Base(), custom.RBegin().Base());
//
//    ASSERT_STREQ(standard.c_str(), custom.Str());
//    ASSERT_STREQ(iteratorStandard.c_str(), iteratorCustom.Str());
//    ASSERT_STREQ(std::string(standard.begin(), (standard.rbegin() + 2).base()).c_str(), String(custom.Begin(), (custom.RBegin() + 2).Base()).Str());
//    ASSERT_STREQ(std::string((standard.rend() - 2).base(), standard.end()).c_str(), String((custom.REnd() - 2).Base(), custom.End()).Str());
//    ASSERT_STREQ(std::string((standard.rend() - 2).base(), (standard.rbegin() + 2).base()).c_str(), String((custom.REnd() - 2).Base(), (custom.RBegin() + 2).Base()).Str());
//}
//
//
