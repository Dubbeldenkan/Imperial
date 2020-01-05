#include "Bond.h"

Bond::Bond(int interestValue, Bond::BondNation bondNation, std::string nation) :
	GameBoardObject()
{
	_interestValue = interestValue;
	_interestPercentage = _interestPercentageConverter[_interestValue - 1];
	_value = _valueConverter[_interestValue - 1];
	_nation = bondNation;

	std::string imagePath = "Flags\\" + nation;
	SetImage(imagePath, imageSize);
}

int Bond::GetValue() const
{
	return _value;
}

int Bond::GetInterestPercentage() const
{
	return _interestPercentage;
}

int Bond::GetInterestValue() const
{
	return _interestValue;
}