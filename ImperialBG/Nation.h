#ifndef NATION_H
#define NATION_H

#include "Bond.h"
#include "GameBoardObject.h"
#include "NodeParser.h"
#include "Region.h"

class Nation : public GameBoardObject
{
private:
	std::vector<Bond> _bondVector;
	std::string _name;
	const int _id;
	std::vector<Region> regions;
	std::string _imagePath;
	int _money = 0;

	const int _numberOfStartBonds = 9;
	static const GameBoardObject::LayerEnum _layerValue = GameBoardObject::NationLayer;

public:
	Nation(int);

	Nation& operator=(const Nation&);
	Nation(Nation const&);
	void CopyNation(const Nation&);
	~Nation();

private:
	void DrawObject() const;

};

#endif // !NATION_H
