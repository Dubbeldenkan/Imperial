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
public:
	enum class ImageLayer { Map, Player, Nation, Region, Unit, Bond };

protected:
	TupleInt _graphicalPos;
	float _scale = 1.0;
	GraphicsNS::Image* _image;
	const int _objectId;
	static GraphicsNS::Graphics* _g;
	
	const ImageLayer _layer;
	static GameBoardObject* _selectedObject;
	static constexpr int _numberOfLayers = 6; //TODO sätt denna beroende på antalet i LayerEnum

private:
	static int _latestObjectId;
	static std::map<int, GameBoardObject*> _gameBoardObjects;
	static constexpr float _selectecObjectScalingFactor = 1.15f;

public:
	static void DrawAllObjects();
	void DrawSelectedObject();

	static void SetGraphics(HWND* hWnd);

	TupleInt GetGraphicalPos() const;
	int GetObjectID() const;

	void SetGraphicalPos(TupleInt pos);
	void SetImageScale(float);
	void Save(NodeParserNS::ListNode**) const;

	virtual void Action(TupleInt);

	//TODO ska denna funktion vara så lätt att nå? Behövs funktionen?
	//static void CleanUpGameObjectMap(); 

protected:
	GameBoardObject();
	GameBoardObject(TupleInt, GraphicsNS::Image*, ImageLayer);
	GameBoardObject(TupleInt, TupleInt, std::string, ImageLayer);
	virtual ~GameBoardObject();

	void SetImage(std::string, TupleInt);
	virtual void DrawObject() const;
	static TupleInt ExtractPos(NodeParserNS::ListNode* &nodeData);
	void CopyGameBoardObject(const GameBoardObject& gameboardObject);

	friend bool operator==(const GameBoardObject& lhs, const GameBoardObject& rhs);
	friend bool operator!=(const GameBoardObject& lhs, const GameBoardObject& rhs);

private:
	bool PosInObject(TupleInt);

	friend class GameBoard;
};

#endif // ! GAME_BOARD_OBJECT_H