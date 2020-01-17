#include "RondelIndicator.h"

const TupleInt RondelIndicator::_rondelImagePos = TupleInt(120, 50);

RondelIndicator::RondelIndicator() :
	GameBoardObject(_rondelImagePos, NULL, GameBoardObject::ImageLayer::Nation)
{
	SetImage(_imagePath, _rondelSize);
}

RondelIndicator::RondelIndicator(GraphicsNS::Graphics::Color color) :
	_color{ color }, GameBoardObject(_rondelImagePos, NULL, GameBoardObject::ImageLayer::Nation)
{
	SetImage(_imagePath, _rondelSize);
}

RondelIndicator& RondelIndicator::operator=(const RondelIndicator& rondelIndicator)
{
	CopyRondelIndicator(rondelIndicator);
	return *this;
}

RondelIndicator::RondelIndicator(RondelIndicator const& rondelIndicator) : 
	GameBoardObject(rondelIndicator._graphicalPos, rondelIndicator._image, _imageLayerValue)
{
	CopyRondelIndicator(rondelIndicator);
}

void RondelIndicator::CopyRondelIndicator(RondelIndicator const& rondelIndicator)
{
	CopyGameBoardObject(rondelIndicator);
	_indicatorPos = rondelIndicator._indicatorPos;
	_color = rondelIndicator._color;
}

RondelIndicator::~RondelIndicator()
{}

void RondelIndicator::DrawObject() const
{
	TupleInt indicatorPos;
	const int posmodifier = 30;
	switch (_indicatorPos)
	{
	case RondelIndicator::RondelPos::StartPos:
		indicatorPos = _rondelMidPos;
		break;
	case RondelIndicator::RondelPos::Taxation:
		indicatorPos = TupleInt(_rondelMidPos.GetX() + posmodifier, _rondelMidPos.GetY() - posmodifier * 2);
		break;
	case RondelIndicator::RondelPos::Factory:
		indicatorPos = TupleInt(_rondelMidPos.GetX() + 2*posmodifier, _rondelMidPos.GetY() - posmodifier);
		break;
	case RondelIndicator::RondelPos::ProductionRight:
		indicatorPos = TupleInt(_rondelMidPos.GetX() + 2*posmodifier, _rondelMidPos.GetY() + posmodifier);
		break;
	case RondelIndicator::RondelPos::ManeuverRight:
		indicatorPos = TupleInt(_rondelMidPos.GetX() + posmodifier, _rondelMidPos.GetY() + posmodifier * 2);
		break;
	case RondelIndicator::RondelPos::Investor:
		indicatorPos = TupleInt(_rondelMidPos.GetX() - posmodifier, _rondelMidPos.GetY() + posmodifier * 2);
		break;
	case RondelIndicator::RondelPos::Import:
		indicatorPos = TupleInt(_rondelMidPos.GetX() - 2*posmodifier, _rondelMidPos.GetY() + posmodifier);
		break;
	case RondelIndicator::RondelPos::ProductionLeft:
		indicatorPos = TupleInt(_rondelMidPos.GetX() - 2*posmodifier, _rondelMidPos.GetY() - posmodifier);
		break;
	case RondelIndicator::RondelPos::ManeuverLeft:
		indicatorPos = TupleInt(_rondelMidPos.GetX() - posmodifier, _rondelMidPos.GetY() - posmodifier * 2);
		break;
	default:
		break;
	}
	int xPos = indicatorPos.GetX();
	int yPos = indicatorPos.GetY();
	switch (_color)
	{
	case GraphicsNS::Graphics::Color::RED:
	case GraphicsNS::Graphics::Color::BLUE:
	case GraphicsNS::Graphics::Color::GREEN:
		xPos -= _modificationPos.GetX();
		break;
	case GraphicsNS::Graphics::Color::YELLOW:
	case GraphicsNS::Graphics::Color::PURPLE:
	case GraphicsNS::Graphics::Color::BLACK:
		xPos += _modificationPos.GetX();
		break;
	default:
		break;
	}
	switch (_color)
	{
	case GraphicsNS::Graphics::Color::BLACK:
	case GraphicsNS::Graphics::Color::RED:
		yPos -= _modificationPos.GetY();
		break;
	case GraphicsNS::Graphics::Color::GREEN:
	case GraphicsNS::Graphics::Color::YELLOW:
		yPos += _modificationPos.GetY();
		break;
	default:
		break;
	}
	_g->DrawRectangle(xPos, yPos, _size.GetX(), _size.GetY(), _color);
}

RondelIndicator::RondelPos RondelIndicator::GetRondelPos() const
{
	return _indicatorPos;
}

RondelIndicator::InvestorState RondelIndicator::GetRondelInvestorState() const
{
	return _investorState;
}

int RondelIndicator::GetObjectID() const
{
	return _objectId;
}

void RondelIndicator::Action(TupleInt mouseClickedPos)
{
	TupleInt adjustedPos(mouseClickedPos.GetX() - _rondelMidPos.GetX(), -(mouseClickedPos.GetY() - _rondelMidPos.GetY()));
	bool top = adjustedPos.GetY() > 0;
	bool right = adjustedPos.GetX() > 0;
	bool aboveXEqY = adjustedPos.GetY() > adjustedPos.GetX();
	bool aboveXEqMinusY = adjustedPos.GetY() > -adjustedPos.GetX();
	if (top && right && aboveXEqY && aboveXEqMinusY)
	{
		_proposedIndicatorPos = RondelPos::Taxation;
	}
	else if (top && right && !aboveXEqY && aboveXEqMinusY)
	{
		_proposedIndicatorPos = RondelPos::Factory;
	}
	else if (!top && right && !aboveXEqY && aboveXEqMinusY)
	{
		_proposedIndicatorPos = RondelPos::ProductionRight;
	}
	else if (!top && right && !aboveXEqY && !aboveXEqMinusY)
	{
		_proposedIndicatorPos = RondelPos::ManeuverRight;
	}
	else if (!top && !right && !aboveXEqY && !aboveXEqMinusY)
	{
		_proposedIndicatorPos = RondelPos::Investor;
	}
	else if (!top && !right && aboveXEqY && !aboveXEqMinusY)
	{
		_proposedIndicatorPos = RondelPos::Import;
	}
	else if (top && !right && aboveXEqY && !aboveXEqMinusY)
	{
		_proposedIndicatorPos = RondelPos::ProductionLeft;
	}
	else if (top && !right && aboveXEqY && aboveXEqMinusY)
	{
		_proposedIndicatorPos = RondelPos::ManeuverLeft;
	}
}

int RondelIndicator::GetNumberOfProposedSteps() const
{
	int numberOfSteps = static_cast<int>(_proposedIndicatorPos) - static_cast<int>(_indicatorPos);
	return numberOfSteps;
}

void RondelIndicator::RunProposal()
{
	_indicatorPos = _proposedIndicatorPos;
}

void RondelIndicator::SetInvestorState()
{
	_investorState = static_cast<RondelIndicator::InvestorState>((static_cast<int>(_investorState) + 1) % 3);
}