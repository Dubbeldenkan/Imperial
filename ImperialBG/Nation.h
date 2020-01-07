#ifndef NATION_H
#define NATION_H

#include "Bond.h"
#include "GameBoardObject.h"
#include "NodeParser.h"
#include "Region.h"

class Nation : public GameBoardObject
{
private:
	std::map<int, Bond> _bonds;
	std::string _name;
	const int _id;
	std::vector<Region> _regions;
	std::string _imagePath;
	int _money = 0;
	std::string _startBondSmallNationName;
	Bond::BondNation _bondNation;

	static constexpr int _numberOfStartBonds = 9;
	static constexpr GameBoardObject::LayerEnum _layerValue = GameBoardObject::NationLayer;

public:
	Nation(int);

	Nation& operator=(const Nation&);
	Nation(Nation const&);
	void CopyNation(const Nation&);
	~Nation();

	Bond* SellBond(int);

	std::string GetStartBondSmallNationName() const;
	std::string GetName() const;
	Bond::BondNation GetBondNation() const;

private:
	void DrawObject() const;

};

#endif // !NATION_H
