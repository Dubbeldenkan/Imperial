#ifndef REGION_H
#define REGION_H

#include "GameBoardObject.h"
#include "NodeParser.h"
#include "Unit.h"

#include <string>

class Region : public GameBoardObject
{
public:
	enum class FactoryType { Land, Sea };

private:
	std::string _name;
	FactoryType _factoryType;
	bool _factoryBuilt;
	bool _drawFactorySite = false;
	std::vector<Unit*> _landUnits;
	std::vector<Unit*> _seaUnits;
	TupleInt _unitLandPos = TupleInt();
	TupleInt _unitSeaPos = TupleInt();

	static GraphicsNS::Image* _landFactoryImage;
	static GraphicsNS::Image* _seaFactoryImage;

	const std::string _landFactoryImagePath = "LandFactory.png";
	const std::string _seaFactoryImagePath = "SeaFactory.png";
	const TupleInt _factorySize = TupleInt(30, 20);
	static constexpr GameBoardObject::ImageLayer _imageLayerValue = GameBoardObject::ImageLayer::Region;

public:
	Region(NodeParserNS::ListNode*);

	Region& operator=(const Region&);
	Region(Region const&);
	void CopyRegion(const Region&);
	~Region();

	void AddLandUnit(Unit* unit);
	void AddSeaUnit(Unit* unit);
	bool GetFactoryBuilt() const;
	FactoryType GetFactoryType() const;

	void SetDrawFactorySite(bool drawFactorySite);
	void BuildFactory();

private:
	void DrawObject() const;
};

#endif // !REGION_H
