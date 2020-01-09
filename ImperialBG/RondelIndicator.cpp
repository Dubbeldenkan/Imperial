#include "RondelIndicator.h"

const TupleInt RondelIndicator::_startPos = TupleInt(200, 130);

RondelIndicator::RondelIndicator() :
	GameBoardObject(_startPos, NULL, GameBoardObject::ImageLayer::Nation)
{}

RondelIndicator::RondelIndicator(GraphicsNS::Graphics::Color color) :
	_color{ color }, GameBoardObject(_startPos, NULL, GameBoardObject::ImageLayer::Nation)
{}

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
	_rondelPos = rondelIndicator._rondelPos;
	_color = rondelIndicator._color;
	_graphicalPos = rondelIndicator._graphicalPos;
}

RondelIndicator::~RondelIndicator()
{}

void RondelIndicator::DrawObject() const
{
	int xPos = _graphicalPos.GetX();
	int yPos = _graphicalPos.GetY();
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