#include "Region.h"

GraphicsNS::Image* Region::_landFactoryImage = NULL;
GraphicsNS::Image* Region::_seaFactoryImage = NULL;

Region::Region(NodeParserNS::ListNode* regionData) :
	GameBoardObject()
{
	_name = regionData->GetData();
	regionData->GetChild(&regionData);
	SetImage("TransparentImage.png", _factorySize);
	do { //TODO för detta borde man göra en separat funktion eftersom att det används i så många klassar
		NodeParserNS::ListNode* tempData;
		if (regionData->GetData().compare("FactoryType") == 0)
		{
			regionData->GetChild(&tempData);
			std::string factoryType = tempData->GetData();
			if (factoryType.compare("Sea") == 0)
			{
				_factoryType = FactoryType::Sea;
			}
			else if (factoryType.compare("Land") == 0)
			{
				_factoryType = FactoryType::Land;
			}
			else
			{
				throw "Unvalid type in " + _name + ".dmd"; // TODO är detta rätt sätt att göra det på?
			}
		}
		else if (regionData->GetData().compare("Factory") == 0)
		{
			regionData->GetChild(&tempData);
			std::string factory = tempData->GetData();
			if (factory.compare("True") == 0)
			{
				_factoryBuilt = true;
			}
			else if (factory.compare("False") == 0)
			{
				_factoryBuilt = false;
			}
			else
			{
				throw "Unvalid type in " + _name + ".dmd"; // TODO är detta rätt sätt att göra det på?
			}
		}
		else if (regionData->GetData().compare("ObjectPos") == 0)
		{
			SetGraphicalPos(ExtractPos(regionData));
		}
		else if (regionData->GetData().compare("LandUnitPos") == 0)
		{
			_unitLandPos = ExtractPos(regionData);
		}
		else if (regionData->GetData().compare("SeaUnitPos") == 0)
		{
			_unitSeaPos = ExtractPos(regionData);
		}
		else
		{
			throw "Unvalid type in " + _name + ".dmd"; // TODO är detta rätt sätt att göra det på?
		}
	} while (!regionData->GetNext(&regionData));

	if (_landFactoryImage == NULL)
	{
		_landFactoryImage = _g->LoadImageFromFile(_landFactoryImagePath, _factorySize.GetX(), _factorySize.GetY());
	}

	if (_seaFactoryImage == NULL)
	{
		_seaFactoryImage = _g->LoadImageFromFile(_seaFactoryImagePath, _factorySize.GetX(), _factorySize.GetY());
	}
}

Region& Region::operator=(const Region& region)
{
	CopyRegion(region);
	return *this;
}

Region::Region(const Region &region) :
	GameBoardObject(region._graphicalPos, region._image, _imageLayerValue)
{
	CopyRegion(region);
}

void Region::CopyRegion(const Region& region)
{
	CopyGameBoardObject(region);
	_name = region._name;
	_factoryBuilt = region._factoryBuilt;
	_factoryType = region._factoryType;
	_unitLandPos = region._unitLandPos;
	_unitSeaPos = region._unitSeaPos;
}

Region::~Region()
{}

void Region::DrawObject() const
{
	//Draw Factory
	if (!_factoryBuilt && _drawFactorySite)
	{
		const GraphicsNS::Graphics::Color color = (_factoryType == FactoryType::Land) ? 
			GraphicsNS::Graphics::Color::BLACK : GraphicsNS::Graphics::Color::BLUE;
		_g->DrawRectangle(_graphicalPos.GetX(), _graphicalPos.GetY(), 
			_factorySize.GetX(), _factorySize.GetY(), color);
	}
	else if (_factoryBuilt && (_factoryType == FactoryType::Land))
	{
		_g->Draw(_landFactoryImage, _graphicalPos.GetX(), _graphicalPos.GetY(), _scale);
	}
	else if ((_factoryBuilt && (_factoryType == FactoryType::Sea)))
	{
		_g->Draw(_seaFactoryImage, _graphicalPos.GetX(), _graphicalPos.GetY(), _scale);
		if (_seaUnits.size() > 0)
		{
			_g->PrintText(_seaUnits.size(), _unitSeaPos.GetX() + 20, _unitSeaPos.GetY(),
				GraphicsNS::Graphics::Color::BLACK, GraphicsNS::Graphics::FontSize::font15);
		}
	}

	//Rita antalet enheter i regionen
	constexpr int xPosNumberModifier = 20;
	if (_landUnits.size() > 0)
	{
		_g->PrintText(_landUnits.size(), _unitLandPos.GetX() + xPosNumberModifier, _unitLandPos.GetY(),
			GraphicsNS::Graphics::Color::BLACK, GraphicsNS::Graphics::FontSize::font15);
	}
	if (_seaUnits.size() > 0)
	{
		_g->PrintText(_seaUnits.size(), _unitSeaPos.GetX() + xPosNumberModifier, _unitSeaPos.GetY(),
			GraphicsNS::Graphics::Color::BLUE, GraphicsNS::Graphics::FontSize::font15);
	}

	//Rita ut enheter som inte har rört sig än i regionen
	if (_landUnits.size() > 0)
	{
		int numberOfUnMovedUnits = 0;
		for (int unitIndex = 0; unitIndex < static_cast<int>(_landUnits.size()); ++unitIndex)
		{
			if (!_landUnits[unitIndex]->GetHasMoved())
			{
				numberOfUnMovedUnits++;
			}
		}
		if (numberOfUnMovedUnits > 0)
		{
			_g->PrintText(numberOfUnMovedUnits, _unitLandPos.GetX() - xPosNumberModifier, _unitLandPos.GetY(),
				GraphicsNS::Graphics::Color::GREEN, GraphicsNS::Graphics::FontSize::font15);
		}
	}
	if (_seaUnits.size() > 0)
	{
		int numberOfUnMovedUnits = 0;
		for (int unitIndex = 0; unitIndex < static_cast<int>(_seaUnits.size()); ++unitIndex)
		{
			if (!_seaUnits[unitIndex]->GetHasMoved())
			{
				numberOfUnMovedUnits++;
			}
		}
		if (numberOfUnMovedUnits > 0)
		{
			_g->PrintText(numberOfUnMovedUnits, _unitSeaPos.GetX() - xPosNumberModifier, _unitSeaPos.GetY(),
				GraphicsNS::Graphics::Color::GREEN, GraphicsNS::Graphics::FontSize::font15);
		}
	}
}

void Region::AddLandUnit(Unit* unit)
{
	unit->SetPos(_unitLandPos);
	_landUnits.push_back(unit);
}

void Region::AddSeaUnit(Unit* unit)
{
	unit->SetPos(_unitSeaPos);
	_seaUnits.push_back(unit);
}

bool Region::GetFactoryBuilt() const
{
	return _factoryBuilt;
}

Region::FactoryType Region::GetFactoryType() const
{
	return _factoryType;
}

void Region::SetDrawFactorySite(bool drawFactorySite)
{
	_drawFactorySite = drawFactorySite;
}

void Region::BuildFactory()
{
	_factoryBuilt = true;
}