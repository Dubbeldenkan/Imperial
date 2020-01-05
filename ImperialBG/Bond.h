#ifndef BOND_H
#define BOND_H

#include "GameBoardObject.h"

class Bond : public GameBoardObject
{
public:
	enum BondNation { Austria_Hungary, Italy, France, Britain, Germany, Russia };

private:
	int _interestValue;
	int _interestPercentage;
	int _value;
	BondNation _nation;

	const int _valueConverter[9] = { 2, 4, 6, 9, 12, 16, 20, 25, 30 };
	const int _interestPercentageConverter[9] = { 50, 50, 50, 44, 42, 38, 35, 32, 30 };
	const TupleInt imageSize = TupleInt(20, 10);

public:
	Bond(int, BondNation, std::string);

	int GetValue() const;
	int GetInterestPercentage() const;
	int GetInterestValue() const;
};

#endif // !1
