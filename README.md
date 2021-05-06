# BitSerializer ![Generic badge](https://img.shields.io/badge/Version-0.12-blue) [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](license.txt) [![Build Status](https://dev.azure.com/real0793/BitSerializer/_apis/build/status/BitSerializer-CI?branchName=master)](https://dev.azure.com/real0793/BitSerializer/_build/latest?definitionId=4&branchName=master) 
___

### Design goals:
- Make a thin wrapper around existing libraries for have one common serialization interface.
- Make easy serialization for all kind of C++ types and STL containers.
- Produce clear JSON for easy integration with Javascript.
- Good test coverage for keep stability of project.
- Cross-platform (Windows, Linux, MacOS).

### Main features:
- One common interface for different kind of formats (currently supported JSON, XML and YAML).
- Simple syntax which is similar to serialization in the Boost library.
- Validation of deserialized values with producing an output list of errors.
- Support serialization for enum types (via declaring names map).
- Support serialization for all STD containers.
- Support serialization to streams and files.
- Encoding to various UTF formats.
- Useful [string conversion submodule](docs\bitserializer_convert.md) (supports enums, classes, UTF encoding).

#### Supported formats:
| BitSerializer sub-module | Format | Encoding | Pretty format | Based on |
| ------ | ------ | ------ |:------:| ------ |
| [cpprestjson-archive](docs/bitserializer_cpprest_json.md) | JSON | UTF-8 | ✖ | [C++ REST SDK](https://github.com/Microsoft/cpprestsdk) |
| [rapidjson-archive](docs/bitserializer_rapidjson.md) | JSON | UTF-8, UTF-16LE, UTF-16BE, UTF-32LE, UTF-32BE | ✅ | [RapidJson](https://github.com/Tencent/rapidjson) |
| [pugixml-archive](docs/bitserializer_pugixml.md) | XML | UTF-8, UTF-16LE, UTF-16BE, UTF-32LE, UTF-32BE | ✅ | [PugiXml](https://github.com/zeux/pugixml) |
| [rapidyaml-archive](docs/bitserializer_rapidyaml.md) | YAML | UTF-8 | ✖ | [RapidYAML](https://github.com/biojppm/rapidyaml) |

#### Requirements:
  - C++ 17 (VS2017, GCC-8, CLang-8).
  - Dependencies which are required by selected type of archive.

##### What's new in version 0.10:
- [ ! ] Changed main concept with separate library for each format to all-in-one library with components.
- [ ! ] Changed include paths for archives (all archive implementations are now in the "bitserializer" directory).

##### What's new in version 0.9:
- [ ! ] Added XML serialization support (based on library PugiXml).
- [ ! ] Added YAML serialization support (based on library RapidYaml).
- [ ! ] Add CI with builds for Windows, Linux (GCC, Clang) and MaOS (AppleClang).
- [ + ] Add formatting options for output text (but formatting is not supported in CppRestJson).
- [ + ] Add support encoding to various UTF based formats (defines in serialization options).
- [ + ] Add optional writing the BOM to output stream/file.
- [ + ] Add ability for pretty format of output text.
- [ + ] Add UTF encoding when serializing std::wstring.
- [ + ] Add serialization for all STD containers which were missed before.
- [ + ] Add serialization C++ union type.
- [ \* ] Split implementation of serialization for std types into separate files.
- [ \* ] Change string type for path in archive from std::wstring to std::string (in UTF-8 encoding).
- [ \* ] For archive based on RapidJson was changed in-memory encoding from UTF-16 to UTF-8.
- [ \* ] Add path into exceptions about I/O errors with files.
- [ \* ] Fix registration enum types not in global namespace.
- [ \* ] Add constants with library version.

[Full log of changes](History.md)

### Performance
For check performance overhead, was developed a test which serializes a model via the BitSerializer and via the API provided by base libraries. The model for tests includes a various types that are supported by all formats. The source code of the test also available [here](tests/performance_tests).

| Base library name | Format | Operation | Native API | BitSerializer | Difference |
| ------ | ------ | ------ |  ------ | ------ | ------ |
| RapidJson | JSON | Save object | 26 msec | 28 msec | 2 msec **(-7.1%)** |
| RapidJson | JSON | Load object | 35 msec | 38 msec | 3 msec **(-7.9%)** |
| C++ REST SDK | JSON | Save object | 199 msec | 200 msec | 1 msec **(-0.5%)** |
| C++ REST SDK | JSON | Load object | 184 msec | 188 msec | 4 msec **(-2.1%)** |
| PugiXml | XML | Save object | 77 msec | 79 msec | 2 msec **(-2.5%)** |
| PugiXml | XML | Load object | 42 msec | 44 msec | 2 msec **(-4.5%)** |
| RapidYAML | YAML | Save object | 550 msec | 549 msec | 1 msec **(-0.2%)** |
| RapidYAML | YAML | Load object | 343 msec | 348 msec | 5 msec **(-1.4%)** |

Results are depend to system hardware and compiler options, there is important only **differences in percentages** which show BitSerializer's overhead over base libraries.

___
## Table of contents
- [How to install](#markdown-header-how-to-install)
- [Hello world](#markdown-header-hello-world)
- [Unicode support](#markdown-header-unicode-support)
- [Serializing class](#markdown-header-serializing-class)
- [Serializing base class](#markdown-header-serializing-base-class)
- [Serializing third party class](#markdown-header-serializing-third-party-class)
- [Serializing enum types](#markdown-header-serializing-enum-types)
- [Serialize to multiple formats](#markdown-header-serialize-to-multiple-formats)
- [Serialization STD types](#markdown-header-serialization-std-types)
- [Specifics of serialization STD map](#markdown-header-specifics-of-serialization-std-map)
- [Conditions for checking the serialization mode](#markdown-header-conditions-for-checking-the-serialization-mode)
- [Validation of deserialized values](#markdown-header-validation-of-deserialized-values)
- [Serialization to streams and files](#markdown-header-serialization-to-streams-and-files)
- [Compile time checking](#markdown-header-compile-time-checking)
- [Error handling](#markdown-header-error-handling)
- [Thanks](#markdown-header-thanks)
- [License](#markdown-header-license)

### Details of archives
- [JSON archive "bitserializer-cpprestjson"](docs/bitserializer_cpprest_json.md)
- [JSON archive "bitserializer-rapidjson"](docs/bitserializer_rapidjson.md)
- [XML archive "bitserializer-pugixml"](docs/bitserializer_pugixml.md)
- [YAML archive "bitserializer-rapidyaml"](docs/bitserializer_rapidyaml.md)

___


### How to install
The library consists of header files only, but it uses third-party libraries which should be also installed.
The easiest way is to use one of supported package managers, in this case, third-party libraries will be installed automatically.
Please follow [instructions](#markdown-header-details-of-archives) for specific archives.
#### VCPKG
```shell
vcpkg install bitserializer[cpprestjson-archive,rapidjson-archive,pugixml-archive,rapidyaml-archive]:x64-windows
```
In the square brackets enumerated all available formats, install only which you need.
#### Conan
The recipe of BitSerializer is available on [Conan-center](https://github.com/conan-io/conan-center-index), just add package to your "conanfile.txt" or 
install via below command:
```shell
conan install bitserializer/0.10@
```
One note for current status: the recipe includes all archives which exists in the BitSerializer, but by default it installs only one dependency - **RapidJson**.
When you need to use **cpprestjson-archive** or **pugixml-archive**, you need to add reference to these packages explicitly. This approach will change, when **components** feature will be released in the Conan.

### Hello world
Let's get started with traditional and simple "Hello world!" example.
```cpp
#include <cassert>
#include <iostream>
#include "bitserializer/bit_serializer.h"
#include "bitserializer/cpprestjson_archive.h"

using JsonArchive = BitSerializer::Json::CppRest::JsonArchive;

int main()
{
	std::string expected = "Hello world!";
	auto json = BitSerializer::SaveObject<JsonArchive>(expected);
	std::string result;
	BitSerializer::LoadObject<JsonArchive>(result, json);

	assert(result == expected);
	std::cout << result << std::endl;

	return EXIT_SUCCESS;
}
```
[See full sample](samples/hello_world/hello_world.cpp)
There is no mistake as JSON format supported any type (object, array, number or string) at root level.

### Unicode support
Besides multiple input and output UTF-formats that BitSerializer supports, it also allows to serialize any of std::basic_string types, under the hood, they are transcoding to output format. You also free to use any string type as keys (with using MakeAutoKeyValue()), but remember that transcoding takes additional time and of course it is better to use string types which are natively supported by a particular archive, usually std::string (UTF-8). In the example below, we show how BitSerializer allow to play with string types:
```cpp
class TestUnicodeClass
{
public:
	template <class TArchive>
	void Serialize(TArchive& archive)
	{
		// Serialize UTF-8 string with key in UTF-16
		archive << MakeAutoKeyValue(u"Utf16Key", mUtf8StringValue);

		// Serialize UTF-16 string with key in UTF-32
		archive << MakeAutoKeyValue(U"Utf32Key", mUtf16StringValue);

		// Serialize UTF-32 string with key in UTF-8
		archive << MakeAutoKeyValue(u8"Utf8Key", mUtf32StringValue);
	};

private:
	std::string mUtf8StringValue;
	std::u16string mUtf16StringValue;
	std::u32string mUtf32StringValue;
};
```

### Serializing class
There are two ways to serialize a class:

  * Internal public method `Serialize()` - good way for your own classes.
  * External static function `SerializeObject()` - used for third party class (no access to sources).

Below example demonstrates how to implement internal serialization method:
```cpp
#include "bitserializer/bit_serializer.h"
#include "bitserializer/rapidjson_archive.h"

using JsonArchive = BitSerializer::Json::RapidJson::JsonArchive;

class TestSimpleClass
{
public:
	TestSimpleClass()
		: testBool(true)
		, testString(L"Hello world!")
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t k = 0; k < 2; k++) {
				testTwoDimensionArray[i][k] = i * 10 + k;
			}
		}
	}

	template <class TArchive>
	void Serialize(TArchive& archive)
	{
		using namespace BitSerializer;
		archive << MakeKeyValue("TestBool", testBool);
		archive << MakeKeyValue("TestString", testString);
		archive << MakeKeyValue("TestTwoDimensionArray", testTwoDimensionArray);
	};

private:
	bool testBool;
	std::wstring testString;
	size_t testTwoDimensionArray[3][2];
};

int main()
{
	auto simpleObj = TestSimpleClass();
	auto result = BitSerializer::SaveObject<JsonArchive>(simpleObj);
    return 0;
}
```
Returns result
```json
{
	"TestBool": true,
	"TestString": "Hello world!",
	"TestTwoDimensionArray": [
		[0, 1],
		[10, 11],
		[20, 21]
	]
}
```
For serializing a named object please use helper method `MakeKeyValue(key, value)`. The type of key should be supported by archive, but there also exists method `MakeAutoKeyValue(key, value)` which automatically converts to the preferred by archive key type. The good place for using this method is some common serialization code that can be used with various types of archives.

### Serializing base class
To serialize the base class, use the helper method `BaseObject()`, as in the next example.
```cpp
template <class TArchive>
void Serialize(TArchive& archive)
{
	archive << BaseObject<MyBaseClass>(*this);
	archive << MakeKeyValue(L"TestInt", TestInt);
};
```

### Serializing third party class
As alternative for internal Serialize() method also exists aproach with defining global functions, it will be usufull in next cases:
 - Sources of serializing class cannot be modified (for example from third party library).
 - When class represents list of some values (such as std::vector), currently there is no alternative with internal method.
 - When you would like to override internal serialization, globally defined functions have higher priority.
 - When you strongly follow single responsibility principle and wouldn't like to include serialization code into class.

You need to implement one of below functions in any namespace:
 - SerializeObject() - when you would like to represent your class as object in a target format (e.g. JSON object).
 - SerializeArray() - when you would like to represent your class as array in a target format (e.g. JSON array).

The example below shows how to implement the most commonly used external function SerializeObject().
```cpp
#include <iostream>
#include "bitserializer/bit_serializer.h"
#include "bitserializer/rapidjson_archive.h"

using namespace BitSerializer;
using JsonArchive = BitSerializer::Json::RapidJson::JsonArchive;

class TestThirdPartyClass
{
public:
	TestThirdPartyClass(int x, int y)
		: x(x), y(y)
	{ }

	int x;

	int GetY() const noexcept { return y; }
	void SetY(int y) noexcept { this->y = y; }

private:
	int y;
};

template<typename TArchive>
void SerializeObject(TArchive& archive, TestThirdPartyClass& testThirdPartyClass)
{
	// Serialize public property
	archive << MakeAutoKeyValue("x", testThirdPartyClass.x);

	// Serialize private property
	if constexpr (TArchive::IsLoading()) {
		int y = 0;
		archive << MakeAutoKeyValue("y", y);
		testThirdPartyClass.SetY(y);
	}
	else {
		const int y = testThirdPartyClass.GetY();
		archive << MakeAutoKeyValue("y", y);
	}
}


int main()
{
	auto testObj = TestThirdPartyClass(100, 200);
	const auto result = BitSerializer::SaveObject<JsonArchive>(testObj);
	std::cout << result << std::endl;
	return 0;
}
```
[See full sample](samples/serialize_third_party_class/serialize_third_party_class.cpp)

### Serializing enum types
To be able to serialize enum types, you must register a map with string equivalents in the your HEADER file.
```cpp
// file HttpMethods.h
#pragma once
#include "bitserializer\string_conversion.h"

enum class HttpMethod {
	Delete = 1,
	Get = 2,
	Head = 3
};

REGISTER_ENUM_MAP(HttpMethod)
{
	{ HttpMethod::Delete,   "delete" },
	{ HttpMethod::Get,      "get" },
	{ HttpMethod::Head,     "head" }
} END_ENUM_MAP()
```

### Serialize to multiple formats
One of the advantages of BitSerializer is the ability to serialize in several formats via one interface. In the following example shows saving an object to JSON and XML:
```cpp
class CPoint
{
public:
	CPoint(int x, int y)
		: x(x), y(y)
	{ }

	template <class TArchive>
	void Serialize(TArchive& archive)
	{
		archive << MakeAutoKeyValue("x", x);
		archive << MakeAutoKeyValue("y", y);
	}

	int x, y;
};

int main()
{
	auto testObj = CPoint(100, 200);

	const auto jsonResult = BitSerializer::SaveObject<JsonArchive>(testObj);
	std::cout << "JSON: " << jsonResult << std::endl;

	const auto xmlResult = BitSerializer::SaveObject<XmlArchive>(testObj);
	std::cout << "XML: " << xmlResult << std::endl;
	return 0;
}
```
The output result of this code:
```
JSON: {"x":100,"y":200}
XML: <?xml version="1.0"?><root><x>100</x><y>200</y></root>
```
The code for serialization has difference only in template parameter - **JsonArchive** and **XmlArchive**.
But here are some moments which need comments. As you can see in the XML was created node with name "root". This is auto generated name when it was not specified explicitly for root node. The library does this just to smooth out differences in the structure of formats. But you are free to set name of root node if needed:
```cpp
const auto xmlResult = BitSerializer::SaveObject<XmlArchive>(MakeAutoKeyValue("Point", testObj));
```
The second thing which you would like to customize is default structure of output XML. In this example it does not looks good from XML perspective, as it has specific element for this purpose which known as "attribute". The BitSerializer also alow to customize the serialization behaviour for different formats:
```cpp
	template <class TArchive>
	void Serialize(TArchive& archive)
	{
		// Serialize as attributes when archive type is XML
		if constexpr (TArchive::archive_type == ArchiveType::Xml)
		{
			archive << MakeAutoAttributeValue("x", x);
			archive << MakeAutoAttributeValue("y", y);
		}
		else
		{
			archive << MakeAutoKeyValue("x", x);
			archive << MakeAutoKeyValue("y", y);
		}
	}
```
With these changes, the result of this code will look like this:
```
JSON: {"x":100,"y":200}
XML: <?xml version="1.0"?><Point x="100" y="200"/>
```
[See full sample](samples/multiformat_customization/multiformat_customization.cpp)

### Serialization STD types
BitSerializer has on board serialization for all STD containers. Serialization of other STD types will be implemented in future. For add support of required STD type just need to include related header file.
```cpp
#include "bitserializer/types/std/array.h"
#include "bitserializer/types/std/vector.h"
#include "bitserializer/types/std/deque.h"
#include "bitserializer/types/std/bitset.h"
#include "bitserializer/types/std/list.h"
#include "bitserializer/types/std/forward_list.h"
#include "bitserializer/types/std/queue.h"
#include "bitserializer/types/std/stack.h"
#include "bitserializer/types/std/set.h"
#include "bitserializer/types/std/unordered_set.h"
#include "bitserializer/types/std/map.h"
#include "bitserializer/types/std/unordered_map.h"
#include "bitserializer/types/std/pair.h"
```
### Specifics of serialization STD map
Due to the fact that the map key is used as a key (in JSON for example), it must be convertible to a string (by default supported all of fundamental types).
```cpp
std::map<std::string, int> testMap = 
	{ { "One", 1 }, { "Two", 2 }, { "Three", 3 }, { "Four", 4 }, { "Five", 5 } };
auto jsonResult = BitSerializer::SaveObject<JsonArchive>(testMap);
```
Returns result
```json
{
	"Five": 5,
	"Four": 4,
	"One": 1,
	"Three": 3,
	"Two": 2
}
```

Below is a more complex example, where loading a vector of maps from JSON.
```json
[{
	"One": 1,
	"Three": 3,
	"Two": 2
}, {
	"Five": 5,
	"Four": 4
}]
```
Code:
```cpp
std::vector<std::map<std::string, int>> testVectorOfMaps;
const std::wstring inputJson = L"[{\"One\":1,\"Three\":3,\"Two\":2},{\"Five\":5,\"Four\":4}]";
BitSerializer::LoadObject<JsonArchive>(testVectorOfMaps, inputJson);
```

If you want to use your own class as a key, you should add conversion methods to it. There are several options with internal and external functions, please look details [here](docs\bitserializer_convert.md). You also can override serialization function `SerializeObject()` for your type of map. For example, you can implement two internal methods in your type:
```cpp
class YourCustomKey
{
	std::string ToString() const { }
	void FromString(const std::string_view& str)
}
```

### Conditions for checking the serialization mode
To check the current serialization mode, use two static methods - `IsLoading()` and `IsSaving()`. As they are «constexpr», you will not have any overhead.
```cpp
class Foo
public:
	template <class TArchive>
	inline void Serialize(TArchive& archive)
	{
		if constexpr (TArchive::IsLoading()) {
			// Code which executes in loading mode
		}
		else {
			// Code which executes in saving mode
		}
	
		if constexpr (TArchive::IsSaving()) {
			// Code which executes in saving mode
		}
		else {
			// Code which executes in loading mode
		}
	}
}
```

### Validation of deserialized values
BitSerializer allows to add an arbitrary number of validation rules to the named values, the syntax is quite simple:
```cpp
archive << MakeKeyValue("testFloat", testFloat, Required(), Range(-1.0f, 1.0f));
```
After deserialize, you can check the status in context and get errors:
```cpp
if (!Context.IsValid())
{
    const auto& validationErrors = Context.GetValidationErrors();
}
```
Basically implemented few validators: `Required`, `Range`, `MinSize`, `MaxSize`.
Validator 'Range' can be used with all types which have operators '<' and '>'.
Validators `MinSize` and `MaxSize` can be applied to all values which have `size()` method.
This list will be extended in future.
```cpp
using JsonArchive = BitSerializer::Json::RapidJson::JsonArchive;

class UserModel
{
public:
	template <class TArchive>
	void Serialize(TArchive& archive)
	{
		using namespace BitSerializer;

		archive << MakeKeyValue("Id", mId, Required());
		archive << MakeKeyValue("Age", mAge, Required(), Range(0, 150));
		archive << MakeKeyValue("FirstName", mFirstName, Required(), MaxSize(16));
		archive << MakeKeyValue("LastName", mLastName, Required(), MaxSize(16));
		// Custom validation with lambda
		archive << MakeKeyValue("NickName", mNickName, [](const std::string& value, const bool isLoaded) -> std::optional<std::string>
		{
			if (!isLoaded || value.find_first_of(' ') == std::string::npos)
				return std::nullopt;
			return "The field must not contain spaces";
		});
	}

private:
	uint64_t mId = 0;
	uint16_t mAge = 0;
	std::string mFirstName;
	std::string mLastName;
	std::string mNickName;
};

int main()
{
	UserModel user;
	const char* json = R"({ "Id": 12420, "Age": 500, "FirstName": "John Smith-Cotatonovich", "NickName": "Smith 2000" })";
	BitSerializer::LoadObject<JsonArchive>(user, json);
	if (!BitSerializer::Context.IsValid())
	{
		std::wcout << L"Validation errors: " << std::endl;
		const auto& validationErrors = BitSerializer::Context.GetValidationErrors();
		for (const auto& keyErrors : validationErrors)
		{
			std::cout << "Path: " << keyErrors.first << std::endl;
			for (const auto& err : keyErrors.second)
			{
				std::cout << "\t" << err << std::endl;
			}
		}
	}

	return EXIT_SUCCESS;
}
```
[See full sample](samples/validation/validation.cpp)

The result of execution this code:
```text
Validation errors:
Path: /Age
        Value must be between 0 and 150
Path: /FirstName
        The maximum size of this field should be not greater than 16
Path: /LastName
        This field is required
Path: /NickName
        The field must not contain spaces
```
Returned paths for invalid values is dependent to archive type, in this sample it's JSON Pointer (RFC 6901).

### Serialization to streams and files
All archives in the BitSerializer support streams, usually it's a sequence of bytes in UTF encoding (with the exception of binary formats).
Also supported detect/writing BOM ([Byte order mark](https://en.wikipedia.org/wiki/Byte_order_mark)).

```cpp
class CPoint
{
public:
	CPoint() = default;
	CPoint(int x, int y)
		: x(x), y(y)
	{ }

	template <class TArchive>
	void Serialize(TArchive& archive)
	{
		archive << MakeAutoKeyValue("x", x);
		archive << MakeAutoKeyValue("y", y);
	}

	int x = 0, y = 0;
};

int main()
{
	auto testObj = CPoint(100, 200);

	SerializationOptions serializationOptions;
	serializationOptions.streamOptions.encoding = Convert::UtfType::Utf8;
	serializationOptions.streamOptions.writeBom = false;

	// Save to string stream
	std::stringstream outputStream;
	BitSerializer::SaveObject<JsonArchive>(testObj, outputStream, serializationOptions);
	std::cout << outputStream.str() << std::endl;

	// Load from string stream
	CPoint loadedObj;
	BitSerializer::LoadObject<JsonArchive>(loadedObj, outputStream);

	assert(loadedObj.x == testObj.x && loadedObj.y == testObj.y);
	return 0;
}
```
[See full sample](samples/validation/serialize_to_stream.cpp)

Two other API methods are used for serialization to files:
```cpp
	BitSerializer::SaveObjectToFile<JsonArchive>(testObj, "D:\test_obj.json");
	BitSerializer::LoadObjectFromFile<JsonArchive>(testObj, "D:\test_obj.json");
```
They are just wrappers of serialization methods into streams.

### Compile time checking
The new C++ 17 ability «if constexpr» helps to generate clear error messages.
If you try to serialize an object that is not supported at the current level of the archive, you will receive a simple error message.
```cpp
template <class TArchive>
inline void Serialize(TArchive& archive)
{
    // Error    C2338	BitSerializer. The archive doesn't support serialize fundamental type without key on this level.
    archive << testBool;
    // Proper use
	archive << MakeKeyValue(L"testString", testString);
};
```

### Error handling
```cpp
try
{
	int testInt;
	BitSerializer::LoadObject<JsonArchive>(testInt, L"10 ?");
}
catch (const BitSerializer::SerializationException& ex)
{
	// Parsing error: Malformed token
	std::string message = ex.what();
}
```

Thanks
----
- Artsiom Marozau for developing an archive with support YAML.
- Andrey Mazhyrau for help with cmake scripts, fix GCC and Linux related issues.
- Alexander Stepaniuk for support and participation in technical discussions.
- Evgeniy Gorbachov for help with implementation STD types serialization.
- Mateusz Pusz for code review and useful advices.

License
----
MIT, Copyright (C) 2018-2021 by Pavel Kisliak
