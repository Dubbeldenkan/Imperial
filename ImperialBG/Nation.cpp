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
				_regions.push_back(Region(tempData));
			}while(!tempData->GetNext(&tempData));
		}
		else if (nationData->GetData().compare("SecondStartBond") == 0)
		{
			nationData->GetChild(&tempData);
			_startBondSmallNationName = tempData->GetData();
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
					throw "Unvalid type in " + _name + ".dmd"; // TODO är detta rätt sätt att göra det på?
				}
			}while (!tempData->GetNext(&tempData));
			SetGraphicalPos(TupleInt(xValue, yValue));
		}
		else
		{
			throw "Unvalid type in " + _name + ".dmd"; // TODO är detta rätt sätt att göra det på?
		}
	} while (!nationData->GetNext(&nationData));

	_bondNation = static_cast<Bond::BondNation>(nationId);
	for (int id = 0; id < _numberOfStartBonds; id++)
	{
		_bonds[id + 1] = Bond(id + 1, _bondNation, _name, _graphicalPos);
	}
}

Nation& Nation::operator=(const Nation& nation)
{
	CopyNation(nation);
	return *this;
}

Nation::Nation(const Nation &nation) :
	_id(nation._id), GameBoardObject(nation._graphicalPos, nation._image, _imageLayerValue)
{
	CopyNation(nation);
}

void Nation::CopyNation(const Nation& nation)
{
	_bonds = nation._bonds;
	_name = nation._name;
	_startBondSmallNationName = nation._startBondSmallNationName;
	_bondNation = nation._bondNation;
	_regions = nation._regions;
	//TODO
}

Nation::~Nation()
{}

void Nation::DrawObject() const
{
	_g->PrintText15("Millions: " + std::to_string(_money), _graphicalPos.GetX(), _graphicalPos.GetY(), GraphicsNS::Graphics::BLACK);
}

Bond* Nation::SellBond(int bondId)
{
	_bonds[bondId].SetToOwnedByPlayer();
	return &_bonds[bondId];
}

std::string Nation::GetStartBondSmallNationName() const
{
	return _startBondSmallNationName;
}

std::string Nation::GetName() const
{
	return _name;
}

Bond::BondNation Nation::GetBondNation() const
{
	return _bondNation;
}