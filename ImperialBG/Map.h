#ifndef MAP_H
#define MAP_h

#include "GameBoardObject.h"
#include "UtilityClasses.h"

class Map : public GameBoardObject
{
private:
	static const GameBoardObject::ImageLayer _imageLayerValue = GameBoardObject::ImageLayer::Map;
	static const std::string _imagePath;
	static const TupleInt _imageSize;
	static const TupleInt _imagePos;
	static const float _scaleValue;

public:
	Map();

	Map& operator=(const Map&);
	Map(Map const&);
	~Map();
};

#endif // !MAP_H