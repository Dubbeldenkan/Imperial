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
	enum class NationGameState { PlacingRondelIndicator, PlayingAction , Done};
	enum class InvestorState { InterestPayout, Investor, SwissBank };

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
	NationGameState _nationGameState = NationGameState::PlacingRondelIndicator;
	InvestorState _investorState = InvestorState::InterestPayout;
	int _numberToImport = 0;

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
	///Producera i den region man klickat på
	void ProductionAction(GameBoardObject*);
	void SetDrawFactorySites();

	int GetNumberOfRegions() const;
	const Region* GetRegion(int regionIndex) const;

	void BuildFactory(GameBoardObject* factoryRegion);
	int GetMoney() const;
	void AddMoney(int addedMoney);
	InvestorState GetInvestorState() const;
	void SetInvestorState();
	Bond* GetUnboughtBond(GameBoardObject* gbo);
	void SetToDone();
	void SetToImport();
	int GetNumberToImport() const;

private:
	void DrawObject() const;

};

#endif // !NATION_H
