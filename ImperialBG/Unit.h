#ifndef UNIT_H
#define UNIT_H

#include "GameBoardObject.h"

class Unit : public GameBoardObject
{
public:
	enum class UnitNation {Austria_Hungary, Italy, France, Great_Britain, Germany, Russia};
	enum class UnitType {Sea, Land};

private:
	const UnitType _unitType;
	const UnitNation _unitNation;
	const TupleInt _imageSize = TupleInt(20, 20);
	const GraphicsNS::Graphics::Color _color;
	bool _hasMoved = true;
	bool _unitSelected = false;

public:
	Unit();
	Unit(TupleInt pos, UnitType unitType, UnitNation unitNation, GraphicsNS::Graphics::Color color);

	Unit& operator=(const Unit&);
	Unit(Unit const&);
	~Unit();

	void SetPos(TupleInt);
	void SetToUnMoved();
	int GetHasMoved() const;
	UnitType GetUnitType() const;
	void SetUnitToSelected();

private:
	void CopyUnit(Unit const& unit);

	void DrawObject() const;
};

#endif // !UNIT_H

