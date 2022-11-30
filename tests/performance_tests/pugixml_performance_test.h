/*******************************************************************************
* Copyright (C) 2018-2022 by Pavel Kisliak                                     *
* This file is part of BitSerializer library, licensed under the MIT license.  *
*******************************************************************************/
#pragma once
#include <stdexcept>
#include "bitserializer/pugixml_archive.h"
#include "archive_base_perf_test.h"
#include "base_test_models.h"


using PugiXmlTestModel = TestModelWithSubArrays<char>;
using PugiXmlBasePerfTest = CArchiveBasePerfTest<BitSerializer::Xml::PugiXml::XmlArchive, PugiXmlTestModel, char>;

class CPugiXmlPerformanceTest final : public PugiXmlBasePerfTest
{
public:
	using model_t = PugiXmlTestModel;
	using base_class_t = PugiXmlBasePerfTest;

	[[nodiscard]] std::string GetArchiveName() const override { return "PugiXml"; }
	[[nodiscard]] bool IsUseNativeLib() const override { return true; }

	size_t SaveModelViaNativeLib() override
	{
		pugi::xml_document mDoc;
		auto rootNode =  mDoc.append_child(PUGIXML_TEXT("root"));

		// Save array of booleans
		auto booleansXmlNode = rootNode.append_child(PUGIXML_TEXT("ArrayOfBooleans"));
		for (auto item : mSourceTestModel.mArrayOfBooleans) {
			booleansXmlNode.append_child(PUGIXML_TEXT("value")).text().set(item);
		}

		// Save array of integers
		auto integersXmlNode = rootNode.append_child(PUGIXML_TEXT("ArrayOfInts"));
		for (const auto item : mSourceTestModel.mArrayOfInts) {
			integersXmlNode.append_child(PUGIXML_TEXT("value")).text().set(item);
		}

		// Save array of strings
		auto stringsXmlNode = rootNode.append_child(PUGIXML_TEXT("ArrayOfStrings"));
		for (const auto& item : mSourceTestModel.mArrayOfStrings) {
			stringsXmlNode.append_child(PUGIXML_TEXT("value")).text().set(item.c_str());
		}

		// Save array of objects
		auto objectsXmlNode = rootNode.append_child(PUGIXML_TEXT("ArrayOfObjects"));
		for (const auto& item : mSourceTestModel.mArrayOfObjects) {
			auto objectNode = objectsXmlNode.append_child(PUGIXML_TEXT("object"));
			objectNode.append_child(PUGIXML_TEXT("TestBoolValue")).text().set(item.mTestBoolValue);
			objectNode.append_child(PUGIXML_TEXT("TestCharValue")).text().set(item.mTestCharValue);
			objectNode.append_child(PUGIXML_TEXT("TestInt64Value")).text().set(item.mTestInt64Value);
			objectNode.append_child(PUGIXML_TEXT("TestFloatValue")).text().set(item.mTestFloatValue);
			objectNode.append_child(PUGIXML_TEXT("TestDoubleValue")).text().set(item.mTestDoubleValue);
			objectNode.append_child(PUGIXML_TEXT("TestString1")).text().set(item.mTestString1.c_str());
			objectNode.append_child(PUGIXML_TEXT("TestString2")).text().set(item.mTestString2.c_str());
			objectNode.append_child(PUGIXML_TEXT("TestString3")).text().set(item.mTestString3.c_str());
			objectNode.append_child(PUGIXML_TEXT("StringWithQuotes")).text().set(item.mStringWithQuotes.c_str());
			objectNode.append_child(PUGIXML_TEXT("MultiLineString")).text().set(item.mMultiLineString.c_str());
		}

		// Build
		out_string_stream_t stream;
		mDoc.save(stream, PUGIXML_TEXT("\t"), pugi::format_raw, pugi::encoding_utf8);
		mNativeLibOutputData = stream.str();
		return mNativeLibOutputData.size();
	}

	size_t LoadModelViaNativeLib() override
	{
		pugi::xml_document mDoc;
		const auto result = mDoc.load_buffer(mNativeLibOutputData.data(), mNativeLibOutputData.size(), pugi::parse_default, pugi::encoding_auto);
		if (!result)
			throw std::runtime_error("PugiXml parse error");

		auto rootNode = mDoc.child(PUGIXML_TEXT("root"));

		// Load array of booleans
		auto booleansXmlNode = rootNode.child(PUGIXML_TEXT("ArrayOfBooleans"));
		int i = 0;
		for (auto it = booleansXmlNode.begin(); it != booleansXmlNode.end(); ++it)
		{
			mNativeLibModel.mArrayOfBooleans[i] = it->text().as_bool();
			++i;
		}

		// Load array of integers
		auto integersXmlNode = rootNode.child(PUGIXML_TEXT("ArrayOfInts"));
		i = 0;
		for (auto it = integersXmlNode.begin(); it != integersXmlNode.end(); ++it)
		{
			mNativeLibModel.mArrayOfInts[i] = it->text().as_llong();
			++i;
		}

		// Load array of strings
		auto stringsXmlNode = rootNode.child(PUGIXML_TEXT("ArrayOfStrings"));
		i = 0;
		for (auto it = stringsXmlNode.begin(); it != stringsXmlNode.end(); ++it)
		{
			mNativeLibModel.mArrayOfStrings[i] = it->text().as_string();
			++i;
		}

		// Load array of objects
		auto objectsXmlNode = rootNode.child(PUGIXML_TEXT("ArrayOfObjects"));
		i = 0;
		for (auto it = objectsXmlNode.begin(); it != objectsXmlNode.end(); ++it)
		{
			auto& obj = mNativeLibModel.mArrayOfObjects[i];
			obj.mTestBoolValue = it->child(PUGIXML_TEXT("TestBoolValue")).text().as_bool();
			obj.mTestCharValue = static_cast<char>(it->child(PUGIXML_TEXT("TestCharValue")).text().as_int());
			obj.mTestInt64Value = it->child(PUGIXML_TEXT("TestInt64Value")).text().as_llong();
			obj.mTestFloatValue = it->child(PUGIXML_TEXT("TestFloatValue")).text().as_float();
			obj.mTestDoubleValue = it->child(PUGIXML_TEXT("TestDoubleValue")).text().as_double();
			obj.mTestString1 = it->child(PUGIXML_TEXT("TestString1")).text().as_string();
			obj.mTestString2 = it->child(PUGIXML_TEXT("TestString2")).text().as_string();
			obj.mTestString3 = it->child(PUGIXML_TEXT("TestString3")).text().as_string();
			obj.mStringWithQuotes = it->child(PUGIXML_TEXT("StringWithQuotes")).text().as_string();
			obj.mMultiLineString = it->child(PUGIXML_TEXT("MultiLineString")).text().as_string();
			++i;
		}

		return mNativeLibOutputData.size();
	}

	void Assert() const override
	{
		base_class_t::Assert();

		mSourceTestModel.Assert(mNativeLibModel);
		// Output XML from BitSerializer and base library should be equal
		assert(mNativeLibOutputData == mBitSerializerOutputData);
	}

	private:
		model_t mNativeLibModel;
		output_format_t mNativeLibOutputData;
};
