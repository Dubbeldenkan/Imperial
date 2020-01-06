#include "Bond.h"

const GameBoardObject::LayerEnum Bond::_layerValue = GameBoardObject::BondLayer;
const int Bond::startBondValueLarge = 4;
const int Bond::startBondValueSmall = 1;

Bond::Bond()
{}

Bond::Bond(int id, Bond::BondNation bondNation, std::string nation, TupleInt graphicalPos) :
	GameBoardObject()
{
	_id = id;
	_interestPercentage = _interestPercentageConverter[_id - 1];
	_value = _valueConverter[_id - 1];
	_nation = bondNation;

	std::string imagePath = "Flags\\" + nation + ".png";
	SetImage(imagePath, _imageSize);
	
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
	GameBoardObject(bond._graphicalPos, bond._image, _layerValue)
{
	CopyBond(bond);
}

void Bond::CopyBond(const Bond& bond)
{
	_graphicalPos = bond._graphicalPos;
	_id = bond._id;
	_interestPercentage = bond._interestPercentage;
	_value = bond._value;
	_nation = bond._nation;
	_image = bond._image;
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

void Bond::SetToOwnedByPlayer()
{
	_ownedByPlayer = true;
}

void Bond::SetToNotOwnedBuPlayer()
{
	_ownedByPlayer = false;
}

bool Bond::GetOwnedByPlayer() const
{
	return _ownedByPlayer;
}

TupleInt Bond::GetImageSize() const
{
	return _imageSize;
}

Bond::BondNation Bond::GetBondNation() const
{
	return _nation;
}