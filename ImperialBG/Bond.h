#ifndef BOND_H
#define BOND_H

#include "GameBoardObject.h"

class Bond : public GameBoardObject
{
public:
	enum class BondNation { Austria_Hungary, Italy, France, Britain, Germany, Russia };
	static const int startBondValueLarge;
	static const int startBondValueSmall;

private:
	int _id;
	int _interestPercentage;
	int _value;
	BondNation _nation;
	bool _ownedByPlayer = false;

	static constexpr int _valueConverter[9] = { 2, 4, 6, 9, 12, 16, 20, 25, 30 };
	static constexpr int _interestPercentageConverter[9] = { 50, 50, 50, 44, 42, 38, 35, 32, 30 };
	const TupleInt _imageSize = TupleInt(40, 20);
	static const GameBoardObject::ImageLayer _imageLayerValue;
	static constexpr int _bondsInAColumn = 6;

public:
	Bond();
	Bond(int, BondNation, std::string, TupleInt);

	Bond& operator=(const Bond&);
	Bond(Bond const&);
	void CopyBond(const Bond&);
	~Bond();

	int GetValue() const;
	int GetInterestPercentage() const;
	int GetId() const;
	TupleInt GetImageSize() const;
	Bond::BondNation GetBondNation() const;

	void SetToOwnedByPlayer();
	void SetToNotOwnedBuPlayer();
	bool GetOwnedByPlayer() const;

	GraphicsNS::Image* GetImage() const;

private:
	void DrawObject() const;
};

#endif // !1
