#ifndef NATION_H
#define NATION_H

#include "Bond.h"
#include "GameBoardObject.h"
#include "NodeParser.h"
#include "Region.h"
#include "RondelIndicator.h"
#include "Unit.h"

class Nation : public GameBoardObject
{
public:
	enum class NationGameState { placingRondelIndicator, playingAction , done};
private:
	std::map<int, Bond> _bonds;
	std::string _name;
	const int _id;
	std::vector<Region> _regions;
	std::vector<Unit> _units;
	std::string _imagePath;
	int _money = 0;
	std::string _startBondSmallNationName;
	Bond::BondNation _bondNation;
	GraphicsNS::Graphics::Color _color; //TODO gör denna const och inkludera i initieringslistan
	RondelIndicator _rondelIndicator;
	NationGameState _nationGameState = NationGameState::placingRondelIndicator;

	static int _currentNation;

	static constexpr int _factoryCost = 5;
	static constexpr int _numberOfStartBonds = 9;
	static constexpr GameBoardObject::ImageLayer _imageLayerValue = GameBoardObject::ImageLayer::Nation;

public:
	Nation(int);

	Nation& operator=(const Nation&);
	Nation(Nation const&);
	void CopyNation(const Nation&);
	~Nation();

	Bond* SellBond(int);

	std::string GetStartBondSmallNationName() const;
	std::string GetName() const;
	Bond::BondNation GetBondNation() const;

	void SetAsCurrentNation();

	RondelIndicator::RondelPos GetRondelState() const;
	NationGameState GetNationState() const;
	int GetRondelIndicatorID() const;

	int MoveRondelIndicator(int maxNumberExtraSteps);
	void ProductionAction();
	void SetDrawFactorySites();

	int GetNumberOfRegions() const;
	const Region* GetRegion(int regionIndex) const;

	void BuildFactory(GameBoardObject* factoryRegion);

private:
	void DrawObject() const;

};

#endif // !NATION_H
