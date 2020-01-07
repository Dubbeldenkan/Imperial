#ifndef GAME_BOARD_OBJECT_H
#define GAME_BOARD_OBJECT_H

#include "Graphics.h"
#include "Image.h"
#include "NodeParser.h"
#include "UtilityClasses.h"

#include <map>
#include <string>

class GameBoardObject
{

protected:
	enum LayerEnum { MapLayer, PlayerLayer, NationLayer, RegionLayer, BondLayer };
	TupleInt _graphicalPos;
	float _scale = 1.0;
	GraphicsNS::Image* _image;
	const int _objectId;
	static GraphicsNS::Graphics* _g;

	static GameBoardObject* _selectedObject;
	static const int _numberOfLayers; //TODO sätt denna beroende på antalet i LayerEnum
	const LayerEnum _layer;

	//Objects graphical position TODO

private:
	static int _latestObjectId;
	static std::map<int, GameBoardObject*> _gameBoardObjects;
	const float _selectecObjectScalingFactor = 1.15f;

public:
	static void DrawAllObjects();
	void DrawSelectedObject();

	static void SetGraphics(HWND* hWnd);

	TupleInt GetGraphicalPos() const;
	int GetObjectID() const;

	void SetGraphicalPos(TupleInt);
	void SetImageScale(float);
	void Save(NodeParserNS::ListNode**) const;

	//TODO ska denna funktion vara så lätt att nå? Behövs funktionen?
	//static void CleanUpGameObjectMap(); 

protected:
	GameBoardObject();
	GameBoardObject(TupleInt, GraphicsNS::Image*, LayerEnum);
	GameBoardObject(TupleInt, TupleInt, std::string, LayerEnum);
	virtual ~GameBoardObject();

	void SetImage(std::string, TupleInt);
	virtual void DrawObject() const;

private:
	bool PosInObject(TupleInt);

	friend class GameBoard;
};

#endif // ! GAME_BOARD_OBJECT_H