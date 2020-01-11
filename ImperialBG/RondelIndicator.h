#ifndef RONDEL_INDICATOR_H
#define RONDEL_INDICATOR_H

#include "GameBoardObject.h"

class RondelIndicator : GameBoardObject
{
public:
	enum class RondelPos { StartPos, Taxation, Factory, ProductionRight, ManeuverRight, 
		Investor, Import, ProductionLeft, ManeuverLeft };

private:
	RondelPos _rondelPos = RondelPos::StartPos;
	GraphicsNS::Graphics::Color _color;

	static const TupleInt _startPos;
	const TupleInt _size = TupleInt(10, 10);
	const GameBoardObject::ImageLayer _imageLayerValue = GameBoardObject::ImageLayer::Nation;

	const TupleInt _modificationPos = TupleInt(5, 10);

public:
	RondelIndicator();
	RondelIndicator(GraphicsNS::Graphics::Color color);

	RondelIndicator& operator=(const RondelIndicator& rondelIndicator);
	RondelIndicator(RondelIndicator const& rondelIndicator);
	~RondelIndicator();

	RondelIndicator::RondelPos GetRondelPos() const;

private:
	void CopyRondelIndicator(RondelIndicator const&);

	void DrawObject() const;
};

#endif // !RONDELL_INDICATOR_H

