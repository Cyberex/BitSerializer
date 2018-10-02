/*******************************************************************************
* Copyright (C) 2018 by Pavel Kisliak                                          *
* This file is part of BitSerializer library, licensed under the MIT license.  *
*******************************************************************************/
#include "pch.h"
#include "common_test_entities.h"
#include "bitserializer/bit_serializer.h"
#include "bitserializer/archives/json_restcpp_archive.h"

using namespace BitSerializer;

//-----------------------------------------------------------------------------
// Tests of serialization for fundamental types (at root scope of archive)
//-----------------------------------------------------------------------------
TEST(JsonRestCpp, SerializeBoolean) {
	TestSerializeType<JsonArchive, bool>(false);
	TestSerializeType<JsonArchive, bool>(true);
}

TEST(JsonRestCpp, SerializeInteger) {
	TestSerializeType<JsonArchive, int32_t>(std::numeric_limits<int32_t>::min());
	TestSerializeType<JsonArchive, uint32_t>(std::numeric_limits<uint32_t>::max());
	TestSerializeType<JsonArchive, int64_t>(std::numeric_limits<int64_t>::min());
	TestSerializeType<JsonArchive, uint64_t>(std::numeric_limits<uint64_t>::max());
}

TEST(JsonRestCpp, SerializeFloat) {
	TestSerializeType<JsonArchive, float>(::BuildFixture<float>());
}

TEST(JsonRestCpp, SerializeDouble) {
	TestSerializeType<JsonArchive, double>(::BuildFixture<double>());
}

TEST(JsonRestCpp, SerializeEnum) {
	TestSerializeType<JsonArchive, TestEnum>(TestEnum::Two);
}

//-----------------------------------------------------------------------------
// Tests of serialization for std::string and std::wstring (at root scope of archive)
//-----------------------------------------------------------------------------
TEST(JsonRestCpp, SerializeString) {
	TestSerializeType<JsonArchive, std::string>("Test ANSI string");
}

TEST(JsonRestCpp, SerializeWString) {
	TestSerializeType<JsonArchive, std::wstring>(L"Test wide string");
}

//-----------------------------------------------------------------------------
// Tests of serialization for c-arrays (at root scope of archive)
//-----------------------------------------------------------------------------
TEST(JsonRestCpp, SerializeArrayOfFundamentalTypes) {
	TestSerializeArray<JsonArchive, bool>();
	TestSerializeArray<JsonArchive, int32_t>();
	TestSerializeArray<JsonArchive, int64_t>();
	TestSerializeArray<JsonArchive, float>();
	TestSerializeArray<JsonArchive, double>();
}

TEST(JsonRestCpp, SerializeArrayOfStrings) {
	TestSerializeArray<JsonArchive, std::string>();
}

TEST(JsonRestCpp, SerializeArrayOfWStrings) {
	TestSerializeArray<JsonArchive, std::wstring>();
}

TEST(JsonRestCpp, SerializeTwoDimensionalArray) {
	TestSerializeTwoDimensionalArray<JsonArchive, int32_t>();
}

//-----------------------------------------------------------------------------
// Tests of serialization for classes
//-----------------------------------------------------------------------------
TEST(JsonRestCpp, SerializeClassWithFundamentalTypes) {
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithFundamentalTypes>());
}

TEST(JsonRestCpp, SerializeClassHierarchy) {
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithInheritance>());
}

TEST(JsonRestCpp, SerializeClassWithSubClass) {
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubClass>());
}

TEST(JsonRestCpp, SerializeClassWithSubArrayOfFundamentalTypes) {
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubArray<bool>>());
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubArray<int32_t>>());
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubArray<int64_t>>());
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubArray<float>>());
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubArray<double>>());
}

TEST(JsonRestCpp, SerializeClassWithSubArrayOfStringTypes) {
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubArray<std::string>>());
}

TEST(JsonRestCpp, SerializeClassWithSubArrayOfWStringTypes) {
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubArray<std::wstring>>());
}

TEST(JsonRestCpp, SerializeClassWithSubArrayOfClasses) {
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubArray<TestPointClass>>());
}

TEST(JsonRestCpp, SerializeClassWithSubTwoDimArray) {
	TestSerializeClass<JsonArchive>(BuildFixture<TestClassWithSubTwoDimArray<int32_t>>());
}

//-----------------------------------------------------------------------------
// Tests streams
//-----------------------------------------------------------------------------
TEST(JsonRestCpp, SerializeClassToStream) {
	TestSerializeClassToStream<JsonArchive, utility::char_t>(BuildFixture<TestClassWithFundamentalTypes>());
}

//-----------------------------------------------------------------------------
// Tests of errors handling
//-----------------------------------------------------------------------------
TEST(JsonRestCpp, ThrowExceptionWhenBadSyntaxInSource) {
	int testInt;
	EXPECT_THROW(LoadObject<JsonArchive>(testInt, L"10 }}"), SerializationException);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}