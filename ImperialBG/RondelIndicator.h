#ifndef RONDEL_INDICATOR_H
#define RONDEL_INDICATOR_H

#include "GameBoardObject.h"

class RondelIndicator : GameBoardObject
{
public:
	enum class RondelPos { StartPos, Taxation, Factory, ProductionRight, ManeuverRight, 
		Investor, Import, ProductionLeft, ManeuverLeft };

	enum class InvestorState {InterestPayout, Investor, SwissBank};

	static constexpr int _rondelStepCost = 2;
	static constexpr int _rondelMaxExtraSteps = 3;

private:
	const std::string _imagePath = "TransparentImage.png";
	const TupleInt _rondelSize = TupleInt(170, 170);
	RondelPos _indicatorPos = RondelPos::StartPos;
	InvestorState _investorState = InvestorState::InterestPayout;
	RondelPos _proposedIndicatorPos;
	GraphicsNS::Graphics::Color _color;

	static const TupleInt _rondelImagePos;

	const TupleInt _size = TupleInt(10, 10);
	const GameBoardObject::ImageLayer _imageLayerValue = GameBoardObject::ImageLayer::Nation;

	const TupleInt _modificationPos = TupleInt(5, 10);
	const TupleInt _rondelMidPos = TupleInt(200, 130);

public:
	RondelIndicator();
	RondelIndicator(GraphicsNS::Graphics::Color color);

	RondelIndicator& operator=(const RondelIndicator& rondelIndicator);
	RondelIndicator(RondelIndicator const& rondelIndicator);
	~RondelIndicator();

	RondelIndicator::RondelPos GetRondelPos() const;
	RondelIndicator::InvestorState GetRondelInvestorState() const;
	void SetInvestorState();

	int GetObjectID() const;
	void Action(TupleInt mouseclickedPos);
	int GetNumberOfProposedSteps() const;
	void RunProposal();

private:
	void CopyRondelIndicator(RondelIndicator const&);

	void DrawObject() const;
};

#endif // !RONDELL_INDICATOR_H

