#include "Map.h"

const std::string Map::_imagePath = "Map.png";
const TupleInt Map::_imageSize = TupleInt(1200, 900);
const TupleInt Map::_imagePos = TupleInt(0, 0);
const float Map::_scaleValue = 0.85f;

Map::Map() : 
GameBoardObject(_imagePos, _imageSize, _imagePath, _layerValue)
{
	_scale = _scaleValue;
}

Map& Map::operator=(const Map& player)
{
	return *this;
}

Map::Map(const Map&player) :
	GameBoardObject()
{
}

Map::~Map()
{
	int tesp = 1;
}