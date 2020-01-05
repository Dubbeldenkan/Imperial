#include "Bond.h"

const GameBoardObject::LayerEnum Bond::_layerValue = GameBoardObject::BondLayer;

Bond::Bond(int id, Bond::BondNation bondNation, std::string nation, TupleInt graphicalPos) :
	_id(id), GameBoardObject()
{
	_interestPercentage = _interestPercentageConverter[_id - 1];
	_value = _valueConverter[_id - 1];
	_nation = bondNation;

	std::string imagePath = "Flags\\" + nation + ".png";
	SetImage(imagePath, imageSize);
	
	TupleInt posDiff = TupleInt(50, 25);
	TupleInt BondPos;
	if (_id < _bondsInAColumn)
	{
		BondPos = TupleInt(graphicalPos.GetX(), graphicalPos.GetY() - _id * posDiff.GetY());
	}
	else
	{
		BondPos = TupleInt(graphicalPos.GetX() + posDiff.GetX(), 
			graphicalPos.GetY() - (_id - _bondsInAColumn + 1) * posDiff.GetY());
	}
	SetGraphicalPos(BondPos);
}

Bond& Bond::operator=(const Bond& bond)
{
	CopyBond(bond);
	return *this;
}

Bond::Bond(const Bond &bond) :
	_id(bond._id), GameBoardObject(bond._graphicalPos, bond._image, _layerValue)
{
	CopyBond(bond);
}

void Bond::CopyBond(const Bond& bond)
{
	_interestPercentage = bond._interestPercentage;
	_value = bond._value;
	_nation = bond._nation;
	//TODO
}

Bond::~Bond()
{}

int Bond::GetValue() const
{
	return _value;
}

int Bond::GetInterestPercentage() const
{
	return _interestPercentage;
}

int Bond::GetId() const
{
	return _id;
}

void Bond::DrawObject() const
{
	_g->Draw(_image, _graphicalPos.GetX(), _graphicalPos.GetY(), _scale);
	_g->PrintText15(_id, _graphicalPos.GetX(), _graphicalPos.GetY(), GraphicsNS::Graphics::BLACK);
}