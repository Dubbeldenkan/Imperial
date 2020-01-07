#ifndef REGION_H
#define REGION_H

#include "GameBoardObject.h"
#include "NodeParser.h"

#include <string>

class Region : public GameBoardObject
{
	enum FactoryType { Land, Sea };

private:
	std::string _name;
	FactoryType _factoryType;
	bool _factoryBuilt;

	static GraphicsNS::Image* _landFactoryImage;
	static GraphicsNS::Image* _seaFactoryImage;

	const std::string _landFactoryImagePath = "LandFactory.png";
	const std::string _seaFactoryImagePath = "SeaFactory.png";
	const TupleInt _factorySize = TupleInt(30, 20);
	static constexpr GameBoardObject::LayerEnum _layerValue = GameBoardObject::RegionLayer;

public:
	Region(NodeParserNS::ListNode*);

	Region& operator=(const Region&);
	Region(Region const&);
	void CopyRegion(const Region&);
	~Region();

private:
	void DrawObject() const;
};

#endif // !REGION_H
