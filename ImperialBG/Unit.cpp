#include "Unit.h"

Unit::Unit() :
	_unitType(Unit::UnitType::Land), _unitNation(Unit::UnitNation::Germany), _color(GraphicsNS::Graphics::Color::BLACK),
	GameBoardObject()
{}

Unit::Unit(TupleInt pos, Unit::UnitType unitType, Unit::UnitNation unitNation, GraphicsNS::Graphics::Color color) :
	_unitType(unitType), _unitNation(unitNation), _color(color), 
	GameBoardObject(pos, NULL, GameBoardObject::ImageLayer::Unit)
{
	if (_unitType == UnitType::Land)
	{
		SetImage("LandUnit.png", _imageSize);
	}
	else
	{
		SetImage("SeaUnit.png", _imageSize);
	}
}

Unit& Unit::operator=(const Unit& unit)
{
	CopyUnit(unit);
	return *this;
}

Unit::Unit(Unit const& unit) :
	_unitType(unit._unitType), _unitNation(unit._unitNation), _color(unit._color),
	GameBoardObject(unit._graphicalPos, unit._image, GameBoardObject::ImageLayer::Unit)
{
	CopyUnit(unit);
}

void Unit::CopyUnit(const Unit& unit)
{
	CopyGameBoardObject(unit);
}

Unit::~Unit()
{}

void Unit::SetPos(TupleInt tupleInt)
{
	SetGraphicalPos(tupleInt);
}

void Unit::DrawObject() const
{
	if (_unitSelected)
	{
		int frameThickness = 3;
		_g->DrawUnfilledRectangle(_graphicalPos.GetX(), _graphicalPos.GetY(),
			_image->GetXSize(), _image->GetYSize(), frameThickness, GraphicsNS::Graphics::Color::BLACK);
	}
	_g->DrawWithColor(_image, _graphicalPos.GetX(), _graphicalPos.GetY(), _color);
}

void Unit::SetToUnMoved()
{
	_hasMoved = false;
}

int Unit::GetHasMoved() const
{
	return _hasMoved;
}

Unit::UnitType Unit::GetUnitType() const
{
	return _unitType;
}

void Unit::SetUnitToSelected()
{
	_unitSelected = true;
}