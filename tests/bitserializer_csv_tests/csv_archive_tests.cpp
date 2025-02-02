﻿/*******************************************************************************
* Copyright (C) 2018-2023 by Pavel Kisliak                                     *
* This file is part of BitSerializer library, licensed under the MIT license.  *
*******************************************************************************/
#include "testing_tools/common_test_methods.h"
#include "csv_archive_fixture.h"

using namespace BitSerializer;
using BitSerializer::Csv::CsvArchive;


//-----------------------------------------------------------------------------
// Tests of serialization for c-arrays (at root scope of archive)
//-----------------------------------------------------------------------------
TEST_F(CsvArchiveTests, SerializeArrayOfClasses)
{
	TestSerializeArray<CsvArchive, TestPointClass>();
}

//-----------------------------------------------------------------------------
// Test paths in archive
//-----------------------------------------------------------------------------
TEST_F(CsvArchiveTests, ShouldReturnPathInArrayScopeWhenLoading)
{
	// Arrange
	TestPointClass testList[3];
	::BuildFixture(testList);

	std::string outputData;
	BitSerializer::SaveObject<CsvArchive>(testList, outputData);

	// Act / Assert
	SerializationOptions options;
	SerializationContext context(options);
	CsvArchive::input_archive_type inputArchive(outputData, context);
	ASSERT_EQ(inputArchive.GetPath(), "");

	auto rootArrayScope = inputArchive.OpenArrayScope(3);
	ASSERT_TRUE(rootArrayScope.has_value());

	for (size_t k = 0; k < 3; k++)
	{
		auto objectScope = rootArrayScope->OpenObjectScope();
		ASSERT_TRUE(objectScope.has_value());

		ASSERT_EQ(CsvArchive::path_separator + Convert::ToString(k), rootArrayScope->GetPath());
		ASSERT_EQ(CsvArchive::path_separator + Convert::ToString(k), objectScope->GetPath());
	}
}

TEST_F(CsvArchiveTests, ShouldReturnPathInArrayScopeWhenSaving)
{
	// Arrange
	TestPointClass testList[3];
	::BuildFixture(testList);

	std::string outputData;
	SerializationOptions options;
	SerializationContext context(options);
	CsvArchive::output_archive_type outputArchive(outputData, context);

	// Act / Assert
	auto rootArrayScope = outputArchive.OpenArrayScope(3);
	ASSERT_TRUE(rootArrayScope.has_value());
	for (size_t k = 0; k < 3; k++)
	{
		auto objectScope = rootArrayScope->OpenObjectScope();
		ASSERT_TRUE(objectScope.has_value());

		ASSERT_EQ(CsvArchive::path_separator + Convert::ToString(k), rootArrayScope->GetPath());
		ASSERT_EQ(CsvArchive::path_separator + Convert::ToString(k), objectScope->GetPath());
	}
}

//-----------------------------------------------------------------------------
// Tests streams / files
//-----------------------------------------------------------------------------
TEST_F(CsvArchiveTests, SerializeArrayToStream)
{
	TestClassWithSubType<std::string> testArray[3];
	BuildFixture(testArray);
	TestSerializeArrayToStream<CsvArchive, char>(testArray);
}

TEST_F(CsvArchiveTests, SerializeUnicodeToEncodedStream) {
	TestClassWithSubType<std::wstring> TestArray[1] = { TestClassWithSubType<std::wstring>(L"Привет мир!") };
	TestSerializeArrayToStream<CsvArchive, char>(TestArray);
}

TEST_F(CsvArchiveTests, LoadFromUtf8Stream) {
	TestLoadCsvFromEncodedStream<Convert::Utf8>(false);
}
TEST_F(CsvArchiveTests, LoadFromUtf8StreamWithBom) {
	TestLoadCsvFromEncodedStream<Convert::Utf8>(true);
}
TEST_F(CsvArchiveTests, LoadFromUtf16LeStream) {
	TestLoadCsvFromEncodedStream<Convert::Utf16Le>(false);
}
TEST_F(CsvArchiveTests, LoadFromUtf16LeStreamWithBom) {
	TestLoadCsvFromEncodedStream<Convert::Utf16Le>(true);
}

TEST_F(CsvArchiveTests, LoadFromUtf16BeStream) {
	TestLoadCsvFromEncodedStream<Convert::Utf16Be>(false);
}
TEST_F(CsvArchiveTests, LoadFromUtf16BeStreamWithBom) {
	TestLoadCsvFromEncodedStream<Convert::Utf16Be>(true);
}

TEST_F(CsvArchiveTests, LoadFromUtf32LeStream) {
	TestLoadCsvFromEncodedStream<Convert::Utf32Le>(false);
}
TEST_F(CsvArchiveTests, LoadFromUtf32LeStreamWithBom) {
	TestLoadCsvFromEncodedStream<Convert::Utf32Le>(true);
}

TEST_F(CsvArchiveTests, LoadFromUtf32BeStream) {
	TestLoadCsvFromEncodedStream<Convert::Utf32Be>(false);
}
TEST_F(CsvArchiveTests, LoadFromUtf32BeStreamWithBom) {
	TestLoadCsvFromEncodedStream<Convert::Utf32Be>(true);
}

TEST_F(CsvArchiveTests, SaveToUtf8Stream) {
	TestSaveCsvToEncodedStream<Convert::Utf8>(false);
}
TEST_F(CsvArchiveTests, SaveToUtf8StreamWithBom) {
	TestSaveCsvToEncodedStream<Convert::Utf8>(true);
}

TEST_F(CsvArchiveTests, SaveToUtf16LeStream) {
	TestSaveCsvToEncodedStream<Convert::Utf16Le>(false);
}
TEST_F(CsvArchiveTests, SaveToUtf16LeStreamWithBom) {
	TestSaveCsvToEncodedStream<Convert::Utf16Le>(true);
}

TEST_F(CsvArchiveTests, SaveToUtf16BeStream) {
	TestSaveCsvToEncodedStream<Convert::Utf16Be>(false);
}
TEST_F(CsvArchiveTests, SaveToUtf16BeStreamWithBom) {
	TestSaveCsvToEncodedStream<Convert::Utf16Be>(true);
}

TEST_F(CsvArchiveTests, SaveToUtf32LeStream) {
	TestSaveCsvToEncodedStream<Convert::Utf32Le>(false);
}
TEST_F(CsvArchiveTests, SaveToUtf32LeStreamWithBom) {
	TestSaveCsvToEncodedStream<Convert::Utf32Le>(true);
}

TEST_F(CsvArchiveTests, SaveToUtf32BeStream) {
	TestSaveCsvToEncodedStream<Convert::Utf32Be>(false);
}
TEST_F(CsvArchiveTests, SaveToUtf32BeStreamWithBom) {
	TestSaveCsvToEncodedStream<Convert::Utf32Be>(true);
}

TEST_F(CsvArchiveTests, SerializeToFile) {
	TestSerializeArrayToFile<CsvArchive>();
}

//-----------------------------------------------------------------------------
// Tests of errors handling
//-----------------------------------------------------------------------------
TEST_F(CsvArchiveTests, ThrowParsingExceptionWhenBadSyntaxInSource)
{
	TestPointClass testList[1];
	EXPECT_THROW(BitSerializer::LoadObject<CsvArchive>(testList, "x,y\n10"), BitSerializer::ParsingException);
	EXPECT_THROW(BitSerializer::LoadObject<CsvArchive>(testList, "x\n10,20"), BitSerializer::ParsingException);
}

TEST_F(CsvArchiveTests, ThrowParsingExceptionWithCorrectPosition)
{
	TestPointClass testList[2];
	try
	{
		const auto testCsv = "x,y\n10,20\n11,\"21\n";
		BitSerializer::LoadObject<CsvArchive>(testList, testCsv);
		EXPECT_FALSE(true);
	}
	catch (const ParsingException& ex)
	{
		EXPECT_EQ(3, ex.Line);
	}
	catch (const std::exception&)
	{
		EXPECT_FALSE(true);
	}
}

TEST_F(CsvArchiveTests, ThrowExceptionWhenUnsupportedSeparator)
{
	SerializationOptions options;
	options.valuesSeparator = '+';
	TestPointClass testList[1];
	EXPECT_THROW(BitSerializer::LoadObject<CsvArchive>(testList, "x+y\n10+20", options), BitSerializer::SerializationException);
}

//-----------------------------------------------------------------------------
TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenMissedRequiredValue) {
	TestValidationForNamedValues<CsvArchive, TestClassForCheckValidation<int>>();
}

//-----------------------------------------------------------------------------
TEST_F(CsvArchiveTests, ThrowMismatchedTypesExceptionWhenLoadStringToBoolean) {
	TestMismatchedTypesPolicy<CsvArchive, std::string, bool>(MismatchedTypesPolicy::ThrowError);
}
TEST_F(CsvArchiveTests, ThrowMismatchedTypesExceptionWhenLoadStringToInteger) {
	TestMismatchedTypesPolicy<CsvArchive, std::string, int32_t>(MismatchedTypesPolicy::ThrowError);
}
TEST_F(CsvArchiveTests, ThrowMismatchedTypesExceptionWhenLoadStringToFloat) {
	TestMismatchedTypesPolicy<CsvArchive, std::string, float>(MismatchedTypesPolicy::ThrowError);
}

TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenLoadStringToBoolean) {
	TestMismatchedTypesPolicy<CsvArchive, std::string, bool>(MismatchedTypesPolicy::Skip);
}
TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenLoadStringToInteger) {
	TestMismatchedTypesPolicy<CsvArchive, std::string, int32_t>(MismatchedTypesPolicy::Skip);
}
TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenLoadStringToFloat) {
	TestMismatchedTypesPolicy<CsvArchive, std::string, float>(MismatchedTypesPolicy::Skip);
}
TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenLoadNullToAnyType) {
	// It doesn't matter what kind of MismatchedTypesPolicy is used, should throw only validation exception
	TestMismatchedTypesPolicy<CsvArchive, std::nullptr_t, bool>(MismatchedTypesPolicy::ThrowError);
	TestMismatchedTypesPolicy<CsvArchive, std::nullptr_t, uint32_t>(MismatchedTypesPolicy::Skip);
	TestMismatchedTypesPolicy<CsvArchive, std::nullptr_t, double>(MismatchedTypesPolicy::ThrowError);
}


//-----------------------------------------------------------------------------

TEST_F(CsvArchiveTests, ThrowSerializationExceptionWhenOverflowBool) {
	TestOverflowNumberPolicy<CsvArchive, int32_t, bool>(OverflowNumberPolicy::ThrowError);
}
TEST_F(CsvArchiveTests, ThrowSerializationExceptionWhenOverflowInt8) {
	TestOverflowNumberPolicy<CsvArchive, int16_t, int8_t>(OverflowNumberPolicy::ThrowError);
	TestOverflowNumberPolicy<CsvArchive, uint16_t, uint8_t>(OverflowNumberPolicy::ThrowError);
}
TEST_F(CsvArchiveTests, ThrowSerializationExceptionWhenOverflowInt16) {
	TestOverflowNumberPolicy<CsvArchive, int32_t, int16_t>(OverflowNumberPolicy::ThrowError);
	TestOverflowNumberPolicy<CsvArchive, uint32_t, uint16_t>(OverflowNumberPolicy::ThrowError);
}
TEST_F(CsvArchiveTests, ThrowSerializationExceptionWhenOverflowInt32) {
	TestOverflowNumberPolicy<CsvArchive, int64_t, int32_t>(OverflowNumberPolicy::ThrowError);
	TestOverflowNumberPolicy<CsvArchive, uint64_t, uint32_t>(OverflowNumberPolicy::ThrowError);
}
TEST_F(CsvArchiveTests, ThrowSerializationExceptionWhenOverflowFloat) {
	TestOverflowNumberPolicy<CsvArchive, double, float>(OverflowNumberPolicy::ThrowError);
}
TEST_F(CsvArchiveTests, ThrowSerializationExceptionWhenLoadFloatToInteger) {
	TestOverflowNumberPolicy<CsvArchive, float, uint32_t>(OverflowNumberPolicy::ThrowError);
	TestOverflowNumberPolicy<CsvArchive, double, uint32_t>(OverflowNumberPolicy::ThrowError);
}

TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenOverflowBool) {
	TestOverflowNumberPolicy<CsvArchive, int32_t, bool>(OverflowNumberPolicy::Skip);
}
TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenNumberOverflowInt8) {
	TestOverflowNumberPolicy<CsvArchive, int16_t, int8_t>(OverflowNumberPolicy::Skip);
	TestOverflowNumberPolicy<CsvArchive, uint16_t, uint8_t>(OverflowNumberPolicy::Skip);
}
TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenNumberOverflowInt16) {
	TestOverflowNumberPolicy<CsvArchive, int32_t, int16_t>(OverflowNumberPolicy::Skip);
	TestOverflowNumberPolicy<CsvArchive, uint32_t, uint16_t>(OverflowNumberPolicy::Skip);
}
TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenNumberOverflowInt32) {
	TestOverflowNumberPolicy<CsvArchive, int64_t, int32_t>(OverflowNumberPolicy::Skip);
	TestOverflowNumberPolicy<CsvArchive, uint64_t, uint32_t>(OverflowNumberPolicy::Skip);
}
TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenNumberOverflowFloat) {
	TestOverflowNumberPolicy<CsvArchive, double, float>(OverflowNumberPolicy::Skip);
}
TEST_F(CsvArchiveTests, ThrowValidationExceptionWhenLoadFloatToInteger) {
	TestOverflowNumberPolicy<CsvArchive, float, uint32_t>(OverflowNumberPolicy::Skip);
	TestOverflowNumberPolicy<CsvArchive, double, uint32_t>(OverflowNumberPolicy::Skip);
}
