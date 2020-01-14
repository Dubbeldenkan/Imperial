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
				throw "Unvalid type in " + _name + ".dmd"; // TODO �r detta r�tt s�tt att g�ra det p�?
			}
		}
		else if (nationData->GetData().compare("ObjectPos") == 0)
		{
			SetGraphicalPos(ExtractPos(nationData));
		}
		else
		{
			throw "Unvalid type in " + _name + ".dmd"; // TODO �r detta r�tt s�tt att g�ra det p�?
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
	CopyGameBoardObject(nation);
	_bonds = nation._bonds;
	_name = nation._name;
	_startBondSmallNationName = nation._startBondSmallNationName;
	_bondNation = nation._bondNation;
	_regions = nation._regions;
	_color = nation._color;
	_rondelIndicator = nation._rondelIndicator;
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
			rectSize.GetX(), rectSize.GetY(), rectWidth, GraphicsNS::Graphics::Color::BLACK);
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

RondelIndicator::RondelPos Nation::GetRondelState() const
{
	return _rondelIndicator.GetRondelPos();
}

Nation::NationGameState Nation::GetNationState() const
{
	return _nationGameState;
}

int Nation::GetRondelIndicatorID() const
{
	return _rondelIndicator.GetObjectID();
}

int Nation::MoveRondelIndicator(int maxNumberExtraSteps)
{
	int playerCost = 0;
	constexpr int numberOfPos = 8;
	const int numberOfSteps = (_rondelIndicator.GetNumberOfProposedSteps() + numberOfPos) % numberOfPos;
	if (_rondelIndicator.GetRondelPos() == RondelIndicator::RondelPos::StartPos)
	{
		playerCost = 0;
		_rondelIndicator.RunProposal();
		_nationGameState = Nation::NationGameState::playingAction;
	}
	else if ((numberOfSteps - RondelIndicator::_rondelMaxExtraSteps) <= maxNumberExtraSteps) //TODO fastnar man om man g�r till samma som man st�r p�?
	{
		_rondelIndicator.RunProposal();
		playerCost = RondelIndicator::_rondelStepCost*max(numberOfSteps - RondelIndicator::_rondelMaxExtraSteps, 0);
		_nationGameState = Nation::NationGameState::playingAction;
	}

	return playerCost;
}

void Nation::ProductionAction()
{
	for (int vectorIndex = 0; vectorIndex < static_cast<int>(_regions.size()); vectorIndex++)
	{
		//TODO g�r s� att man inte kan bygga f�r m�nga gubbar
		Region* region = &_regions[vectorIndex];
		if (!region->GetFactoryBuilt())
		{
			//g�r inget
		}
		else if (region->GetFactoryType() == Region::FactoryType::Land)
		{
			_units.push_back(Unit(TupleInt(), Unit::UnitType::Land, static_cast<Unit::UnitNation>(_bondNation), _color));
			_regions[vectorIndex].AddUnit(_units[_units.size() - 1]);
		}
		else
		{
			//TODO l�gg till bygge av b�tar
		}
	}
	_nationGameState = Nation::NationGameState::done;
}