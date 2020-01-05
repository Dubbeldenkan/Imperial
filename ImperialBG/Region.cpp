#include "Region.h"

Region::Region(NodeParserNS::ListNode* regionData)
{
	_name = regionData->GetData();
	regionData->GetChild(&regionData);
	do {
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
				throw "Unvalid type in " + _name + ".dmd"; // TODO �r detta r�tt s�tt att g�ra det p�?
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
				throw "Unvalid type in " + _name + ".dmd"; // TODO �r detta r�tt s�tt att g�ra det p�?
			}
		}
		else
		{
			throw "Unvalid type in " + _name + ".dmd"; // TODO �r detta r�tt s�tt att g�ra det p�?
		}
	} while (!regionData->GetNext(&regionData));
}