#include "Region.h"

GraphicsNS::Image* Region::_landFactoryImage = NULL;
GraphicsNS::Image* Region::_seaFactoryImage = NULL;

Region::Region(NodeParserNS::ListNode* regionData) :
	GameBoardObject()
{
	_name = regionData->GetData();
	regionData->GetChild(&regionData);
	_image = NULL;
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
}

Region::~Region()
{}

void Region::DrawObject() const
{
	//Draw Factory
	if (_factoryBuilt && (_factoryType == FactoryType::Land))
	{
		_g->Draw(_landFactoryImage, _graphicalPos.GetX(), _graphicalPos.GetY(), _scale);
	}
	else if ((_factoryBuilt && (_factoryType == FactoryType::Sea)))
	{
		_g->Draw(_seaFactoryImage, _graphicalPos.GetX(), _graphicalPos.GetY(), _scale);
	}
	//Draw units
}