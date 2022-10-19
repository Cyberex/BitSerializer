﻿/*******************************************************************************
* Copyright (C) 2020 by Artsiom Marozau                                        *
* This file is part of BitSerializer library, licensed under the MIT license.  *
*******************************************************************************/
#include "test_helpers/common_test_methods.h"
#include "test_helpers/common_json_test_methods.h"
#include "test_helpers/common_yaml_test_methods.h"
#include "bitserializer/rapidyaml_archive.h"

using YamlArchive = BitSerializer::Yaml::RapidYaml::YamlArchive;

//-----------------------------------------------------------------------------
// Tests of serialization for c-arrays (at root scope of archive)
//-----------------------------------------------------------------------------
TEST(RapidYamlArchive, SerializeArrayOfBooleans)
{
	TestSerializeArray<YamlArchive, bool>();
}

TEST(RapidYamlArchive, SerializeArrayOfChars)
{
	TestSerializeArray<YamlArchive, char>();
	TestSerializeArray<YamlArchive, unsigned char>();
}

TEST(RapidYamlArchive, SerializeArrayOfIntegers)
{
	TestSerializeArray<YamlArchive, uint16_t>();
	TestSerializeArray<YamlArchive, int64_t>();
}

TEST(RapidYamlArchive, SerializeArrayOfFloats)
{
	TestSerializeArray<YamlArchive, float>();
	TestSerializeArray<YamlArchive, double>();
}

TEST(RapidYamlArchive, SerializeArrayOfNullptrs)
{
	TestSerializeArray<YamlArchive, std::nullptr_t>();
}

TEST(RapidYamlArchive, SerializeArrayOfStrings)
{
	TestSerializeArray<YamlArchive, std::string>();
}

TEST(RapidYamlArchive, SerializeArrayOfUnicodeStrings)
{
	TestSerializeArray<YamlArchive, std::wstring>();
	TestSerializeArray<YamlArchive, std::u16string>();
	TestSerializeArray<YamlArchive, std::u32string>();
}

TEST(RapidYamlArchive, SerializeArrayOfClasses)
{
	TestSerializeArray<YamlArchive, TestPointClass>();
}

TEST(RapidYamlArchive, SerializeTwoDimensionalArray)
{
	TestSerializeTwoDimensionalArray<YamlArchive, int32_t>();
}

//-----------------------------------------------------------------------------
// Tests of serialization for classes
//-----------------------------------------------------------------------------
TEST(RapidYamlArchive, SerializeClassWithMemberBoolean)
{
	TestSerializeClass<YamlArchive>(TestClassWithSubTypes<bool>(false));
	TestSerializeClass<YamlArchive>(TestClassWithSubTypes<bool>(true));
}

TEST(RapidYamlArchive, SerializeClassWithMemberInteger)
{
	TestSerializeClass<YamlArchive>(BuildFixture<TestClassWithSubTypes<int8_t, uint8_t, int64_t, uint64_t>>());
	TestSerializeClass<YamlArchive>(TestClassWithSubTypes(std::numeric_limits<int64_t>::min(), std::numeric_limits<uint64_t>::max()));
}

TEST(RapidYamlArchive, SerializeClassWithMemberFloat)
{
	TestSerializeClass<YamlArchive>(TestClassWithSubTypes(std::numeric_limits<float>::min(), 0.0f, std::numeric_limits<float>::max()));
}

TEST(RapidYamlArchive, SerializeClassWithMemberDouble)
{
	TestSerializeClass<YamlArchive>(TestClassWithSubTypes(std::numeric_limits<double>::min(), 0.0, std::numeric_limits<double>::max()));
}

TEST(RapidYamlArchive, SerializeClassWithMemberNullptr)
{
	TestSerializeClass<YamlArchive>(BuildFixture<TestClassWithSubTypes<std::nullptr_t>>());
}

TEST(RapidYamlArchive, SerializeClassWithMemberString)
{
	TestSerializeClass<YamlArchive>(BuildFixture<TestClassWithSubTypes<std::string, std::wstring, std::u16string, std::u32string>>());
}

TEST(RapidYamlArchive, SerializeClassHierarchy)
{
	TestSerializeClass<YamlArchive>(BuildFixture<TestClassWithInheritance>());
}

TEST(RapidYamlArchive, SerializeClassWithMemberClass)
{
	using TestClassType = TestClassWithSubTypes<TestClassWithSubTypes<int64_t>>;
	TestSerializeClass<YamlArchive>(BuildFixture<TestClassType>());
}

TEST(RapidYamlArchive, SerializeClassWithSubArray)
{
	TestSerializeClass<YamlArchive>(BuildFixture<TestClassWithSubArray<int64_t>>());
}

TEST(RapidYamlArchive, SerializeClassWithSubArrayOfClasses)
{
	TestSerializeClass<YamlArchive>(BuildFixture<TestClassWithSubArray<TestPointClass>>());
}

TEST(RapidYamlArchive, SerializeClassWithSubTwoDimArray)
{
	TestSerializeClass<YamlArchive>(BuildFixture<TestClassWithSubTwoDimArray<int32_t>>());
}

TEST(RapidYamlArchive, ShouldIterateKeysInObjectScope)
{
	TestIterateKeysInObjectScope<YamlArchive>();
}

//-----------------------------------------------------------------------------
// Test paths in archive
//-----------------------------------------------------------------------------
TEST(RapidYamlArchive, ShouldReturnPathInObjectScopeWhenLoading)
{
	TestGetPathInJsonObjectScopeWhenLoading<YamlArchive>();
}

TEST(RapidYamlArchive, ShouldReturnPathInObjectScopeWhenSaving)
{
	TestGetPathInJsonObjectScopeWhenSaving<YamlArchive>();
}

TEST(RapidYamlArchive, ShouldReturnPathInArrayScopeWhenLoading)
{
	TestGetPathInJsonArrayScopeWhenLoading<YamlArchive>();
}

TEST(RapidYamlArchive, ShouldReturnPathInArrayScopeWhenSaving)
{
	TestGetPathInJsonArrayScopeWhenSaving<YamlArchive>();
}

//-----------------------------------------------------------------------------
// Test the validation for named values (boolean result, which returns by archive's method SerializeValue()).
//-----------------------------------------------------------------------------
TEST(RapidYamlArchive, ShouldCollectErrorsAboutRequiredNamedValues)
{
	TestValidationForNamedValues<YamlArchive, TestClassForCheckValidation<bool>>();
	TestValidationForNamedValues<YamlArchive, TestClassForCheckValidation<int>>();
	TestValidationForNamedValues<YamlArchive, TestClassForCheckValidation<double>>();
	TestValidationForNamedValues<YamlArchive, TestClassForCheckValidation<std::string>>();
	TestValidationForNamedValues<YamlArchive, TestClassForCheckValidation<TestPointClass>>();
	TestValidationForNamedValues<YamlArchive, TestClassForCheckValidation<int[3]>>();
}

TEST(RapidYamlArchive, ShouldCollectErrorsWhenLoadingFromNotCompatibleTypes)
{
	using SourceStringType = TestClassForCheckCompatibleTypes<std::string>;
	TestValidationForNotCompatibleTypes<YamlArchive, SourceStringType, TestClassForCheckCompatibleTypes<std::nullptr_t>>();
	TestValidationForNotCompatibleTypes<YamlArchive, SourceStringType, TestClassForCheckCompatibleTypes<bool>>();
	TestValidationForNotCompatibleTypes<YamlArchive, SourceStringType, TestClassForCheckCompatibleTypes<int>>();
	TestValidationForNotCompatibleTypes<YamlArchive, SourceStringType, TestClassForCheckCompatibleTypes<double>>();
	TestValidationForNotCompatibleTypes<YamlArchive, SourceStringType, TestClassForCheckCompatibleTypes<TestPointClass>>();
	TestValidationForNotCompatibleTypes<YamlArchive, SourceStringType, TestClassForCheckCompatibleTypes<int[3]>>();
}

//-----------------------------------------------------------------------------
// Tests streams / files
//-----------------------------------------------------------------------------
TEST(RapidYamlArchive, SerializeClassToStream) {
	TestSerializeClassToStream<YamlArchive, char>(BuildFixture<TestPointClass>());
}

TEST(RapidYamlArchive, SerializeUnicodeToEncodedStream) {
	TestClassWithSubType<std::wstring> TestValue(L"Привет мир!");
	TestSerializeClassToStream<YamlArchive, char>(TestValue);
}

TEST(RapidYamlArchive, LoadFromUtf8Stream) {
	TestLoadYamlFromEncodedStream<YamlArchive, BitSerializer::Convert::Utf8>(false);
}
TEST(RapidYamlArchive, LoadFromUtf8StreamWithBom) {
	TestLoadYamlFromEncodedStream<YamlArchive, BitSerializer::Convert::Utf8>(true);
}

TEST(RapidYamlArchive, SaveToUtf8Stream) {
	TestSaveYamlToEncodedStream<YamlArchive, BitSerializer::Convert::Utf8>(false);
}
TEST(RapidYamlArchive, SaveToUtf8StreamWithBom) {
	TestSaveYamlToEncodedStream<YamlArchive, BitSerializer::Convert::Utf8>(true);
}

TEST(RapidYamlArchive, SerializeToFile) {
	TestSerializeArrayToFile<YamlArchive>();
}

//-----------------------------------------------------------------------------
// Tests of errors handling
//-----------------------------------------------------------------------------
TEST(RapidYamlArchive, ThrowExceptionWhenBadSyntaxInSource)
{
	int testInt[2];
	EXPECT_THROW(BitSerializer::LoadObject<YamlArchive>(testInt, "- 10\n20"), BitSerializer::SerializationException);
}
