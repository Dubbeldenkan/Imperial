#include "Nation.h"

Nation::Nation(int nationId) :
	_id(nationId), GameBoardObject()
{
	std::string nationName[6] = { "Austria-Hungary", "Italy", "France", "Britain", "Germany", "Russia" };

	_name = nationName[nationId];
	NodeParserNS::ListNode* nationData = NodeParserNS::NodeParser::ReadDataFile(_name);
	nationData->GetChild(&nationData);
	do {
		NodeParserNS::ListNode* tempData;
		if (nationData->GetData().compare("ImagePath") == 0)
		{
			nationData->GetChild(&tempData);
			_imagePath = tempData->GetData();
		}
		else if (nationData->GetData().compare("Regions") == 0)
		{
			nationData->GetChild(&tempData);
			do {
				regions.push_back(Region(tempData));
			}while(!tempData->GetNext(&tempData));
		}
		else if (nationData->GetData().compare("ObjectPos") == 0)
		{
			int xValue;
			int yValue;
			nationData->GetChild(&tempData);
			NodeParserNS::ListNode* valueData;
			do {
				if (tempData->GetData().compare("X") == 0)
				{
					tempData->GetChild(&valueData);
					xValue = stoi(valueData->GetData());
				}
				else if (tempData->GetData().compare("Y") == 0)
				{
					tempData->GetChild(&valueData);
					yValue = stoi(valueData->GetData());
				}
				else
				{
					throw "Unvalid type in " + _name + ".dmd"; // TODO �r detta r�tt s�tt att g�ra det p�?
				}
			}while (!tempData->GetNext(&tempData));
			SetGraphicalPos(TupleInt(xValue, yValue));
		}
		else
		{
			throw "Unvalid type in " + _name + ".dmd"; // TODO �r detta r�tt s�tt att g�ra det p�?
		}
	} while (!nationData->GetNext(&nationData));

	for (int bondInterestValue = 0; bondInterestValue < _numberOfStartBonds; bondInterestValue++)
	{
		_bondVector.push_back(Bond(bondInterestValue + 1, static_cast<Bond::BondNation>(nationId), _name));
	}
}

Nation& Nation::operator=(const Nation& nation)
{
	CopyNation(nation);
	return *this;
}

Nation::Nation(const Nation &nation) :
	_id(nation._id), GameBoardObject(nation._graphicalPos, nation._image, _layerValue)
{
	CopyNation(nation);
}

void Nation::CopyNation(const Nation& nation)
{
	//TODO
}

Nation::~Nation()
{}

void Nation::DrawObject()
{
	_g->PrintText15("Millions: " + std::to_string(_money), _graphicalPos.GetX(), _graphicalPos.GetY(), GraphicsNS::Graphics::BLACK);
}