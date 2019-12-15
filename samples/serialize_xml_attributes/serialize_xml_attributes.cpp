#include <iostream>
#include <vector>
#include "bitserializer/bit_serializer.h"
#include "bitserializer_pugixml/pugixml_archive.h"

using namespace BitSerializer::Xml::PugiXml;
using namespace BitSerializer;

class CRectangle
{
public:
	CRectangle(const int Width, const int Height)
		: mType("Rectangle")
		, mWidth(Width)
		, mHeight(Height)
	{ }

	template <class TArchive>
	void Serialize(TArchive& archive)
	{
		archive << MakeAttributeValue("Type", mType);
		archive << MakeAttributeValue("Width", mWidth);
		archive << MakeAttributeValue("Height", mHeight);
	}

	std::string mType;
	int mWidth;
	int mHeight;
};

int main()
{
	std::vector<CRectangle> Shapes = {
		{ 5, 10 },
		{ 20, 5 },
		{ 50, 25 }
	};
	const auto result = BitSerializer::SaveObject<XmlArchive>(MakeKeyValue("Shapes", Shapes));
	std::cout << result << std::endl;
	return 0;
}