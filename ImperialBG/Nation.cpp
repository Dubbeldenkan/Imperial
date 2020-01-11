#include "Nation.h"

int Nation::_currentNation = 0;

Nation::Nation(int nationId) :
	_id(nationId), GameBoardObject(TupleInt(), NULL, _imageLayerValue)
{
	std::string nationName[6] = { "Austria-Hungary", "Italy", "France", "Britain", "Germany", "Russia" };

	_name = nationName[nationId];
	NodeParserNS::ListNode* nationData = NodeParserNS::NodeParser::ReadDataFile(_name);
	nationData->GetChild(&nationData);
	_bondNation = static_cast<Bond::BondNation>(nationId);
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
		else if (nationData->GetData().compare("Color") == 0)
		{
			nationData->GetChild(&tempData);
			const std::string colorName = tempData->GetData();
			if (colorName.compare("Black") == 0)
			{
				_color = GraphicsNS::Graphics::Color::BLACK;
			}
			else if (colorName.compare("Blue") == 0)
			{
				_color = GraphicsNS::Graphics::Color::BLUE;
			}
			else if (colorName.compare("Green") == 0)
			{
				_color = GraphicsNS::Graphics::Color::GREEN;
			}
			else if (colorName.compare("Red") == 0)
			{
				_color = GraphicsNS::Graphics::Color::RED;
			}
			else if (colorName.compare("Yellow") == 0)
			{
				_color = GraphicsNS::Graphics::Color::YELLOW;
			}
			else if (colorName.compare("Purple") == 0)
			{
				_color = GraphicsNS::Graphics::Color::PURPLE;
			}
			else 
			{
				throw "Unvalid type in " + _name + ".dmd"; // TODO är detta rätt sätt att göra det på?
			}
		}
		else if (nationData->GetData().compare("ObjectPos") == 0)
		{
			SetGraphicalPos(ExtractPos(nationData));
		}
		else
		{
			throw "Unvalid type in " + _name + ".dmd"; // TODO är detta rätt sätt att göra det på?
		}
	} while (!nationData->GetNext(&nationData));

	for (int id = 0; id < _numberOfStartBonds; id++)
	{
		_bonds[id + 1] = Bond(id + 1, _bondNation, _name, _graphicalPos);
	}

	_rondelIndicator = RondelIndicator(_color);
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
	_color = nation._color;
	_rondelIndicator = nation._rondelIndicator;
	_image = _image;
	//TODO flytta image och graphicsPos till gameboardObject
	//TODO
}

Nation::~Nation()
{}

void Nation::DrawObject() const
{
	if (_currentNation == _id)
	{
		const TupleInt rectPosModification = TupleInt(-5, -145);
		const TupleInt rectSize = TupleInt(100, 175);
		constexpr int rectWidth = 10;
		_g->DrawUnfilledRectangle(_graphicalPos.GetX() + rectPosModification.GetX(), _graphicalPos.GetY() + rectPosModification.GetY(), 
			rectSize.GetX(), rectSize.GetY(), rectWidth, GraphicsNS::Graphics::Color::WHITE);
	}
	_g->PrintText("Millions: " + std::to_string(_money), _graphicalPos.GetX(), _graphicalPos.GetY(), 
		GraphicsNS::Graphics::Color::BLACK, GraphicsNS::Graphics::FontSize::font15);
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

void Nation::SetAsCurrentNation()
{
	_currentNation = _id;
}