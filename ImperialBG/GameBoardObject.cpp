#include "GameBoardObject.h"

int GameBoardObject::_latestObjectId = 0;
std::map<int, GameBoardObject*> GameBoardObject::_gameBoardObjects;
GraphicsNS::Graphics* GameBoardObject::_g = NULL;
GameBoardObject* GameBoardObject::_selectedObject = NULL;

//Objects graphical position

GameBoardObject::GameBoardObject() : _layer(ImageLayer::Map), _objectId(++_latestObjectId)
{
	_gameBoardObjects[_objectId] = this;
}

GameBoardObject::GameBoardObject(TupleInt graphicalPos, GraphicsNS::Image* image, ImageLayer layer) :
	_layer(layer), _objectId(++_latestObjectId)
{
	_graphicalPos = graphicalPos;
	_image = image;
	_gameBoardObjects[_objectId] = this;
}

GameBoardObject::GameBoardObject(TupleInt graphicalPos, TupleInt imageSize, std::string imagePath, ImageLayer layer) :
	_layer(layer), _objectId(++_latestObjectId)
{
	_graphicalPos = graphicalPos;
	_image = _g->LoadImageFromFile(imagePath, imageSize.GetX(), imageSize.GetY());
	_gameBoardObjects[_objectId] = this;
}

GameBoardObject::~GameBoardObject()
{
	_gameBoardObjects.erase(_objectId);
}

void GameBoardObject::DrawObject() const
{
	_g->Draw(_image, _graphicalPos.GetX(), _graphicalPos.GetY(), _scale);
}

void GameBoardObject::DrawAllObjects()
{
	_g->Clear();
	_g->StartDrawing();

	std::vector<GameBoardObject*> layerVector[_numberOfLayers];
	std::map<int, GameBoardObject*>::iterator it;
	for (it = _gameBoardObjects.begin(); it != _gameBoardObjects.end(); it++)
	{
		if(_selectedObject == NULL || it->second->GetObjectID() != _selectedObject->GetObjectID())
		{
			layerVector[static_cast<int>(it->second->_layer)].push_back(it->second);
		}
	}
	for (int layerCount = 0; layerCount < _numberOfLayers; layerCount++)
	{
		for (int vectorCount = 0; vectorCount < static_cast<int>(layerVector[layerCount].size()); vectorCount++)
		{
			layerVector[layerCount][vectorCount]->DrawObject();
		}
		if (_selectedObject != NULL && static_cast<int>(_selectedObject->_layer) == layerCount)
		{
			_selectedObject->DrawSelectedObject();
		}
	}

	_g->StopDrawing();
	_g->Flip();
}

void GameBoardObject::SetGraphics(HWND* hWnd)
{
	_g = new GraphicsNS::Graphics(*hWnd);
}

TupleInt GameBoardObject::GetGraphicalPos() const
{
	return _graphicalPos;
}

int GameBoardObject::GetObjectID() const
{
	return _objectId;
}

void GameBoardObject::Action(TupleInt mouseClickedPos)
{}

void GameBoardObject::Save(NodeParserNS::ListNode** savedGame) const
{}

void GameBoardObject::SetGraphicalPos(TupleInt pos)
{
	_graphicalPos = pos;
}

void GameBoardObject::SetImageScale(float scale)
{
	_scale = scale;
}

/// imagePath and imageSize
void GameBoardObject::SetImage(std::string imagePath, TupleInt imageSize)
{
	_image = _g->LoadImageFromFile(imagePath, imageSize.GetX(), imageSize.GetY());
}

bool GameBoardObject::PosInObject(TupleInt pos)
{
	bool result = false;
	if (_image != NULL)
	{
		const bool withInX = ((_graphicalPos.GetX() <= pos.GetX()) &&
			(pos.GetX() <= (_graphicalPos.GetX() + _image->GetXSize())));
		const bool withInY = ((_graphicalPos.GetY() <= pos.GetY()) &&
			(pos.GetY() <= (_graphicalPos.GetY() + _image->GetYSize())));
		result = withInX && withInY;
	}
	return (result);
}

void GameBoardObject::DrawSelectedObject()
{
	// TODO, ändra pekare till vector av pekare för att kunna markera flera
	//flytta x till vänster skillnaden mellan skalad storlek och normal storlek delat på 2
	const int xPos = _graphicalPos.GetX() - 
		static_cast<int>(((_image->GetXSize() * _selectecObjectScalingFactor) - (_image->GetXSize())) / 2);
	const int yPos = _graphicalPos.GetY() - 
		static_cast<int>(((_image->GetYSize() * _selectecObjectScalingFactor) - (_image->GetYSize())) / 2);
	_g->DrawWithColor(_image, xPos, yPos, GraphicsNS::Graphics::Color::BLUE, _selectecObjectScalingFactor);
	DrawObject();
}

TupleInt GameBoardObject::ExtractPos(NodeParserNS::ListNode* &nodeData)
{
	NodeParserNS::ListNode* tempData;
	int xValue;
	int yValue;
	nodeData->GetChild(&tempData);
	NodeParserNS::ListNode* valueData;
	do {
		if (tempData->GetData().compare("X") == 0)
		{
			tempData->GetChild(&valueData);
			xValue = stoi(valueData->GetData());
		}
		else if (tempData->GetData().compare("Y") == 0)
		{
			tempData->GetChild(&valueData);
			yValue = stoi(valueData->GetData());
		}
		else
		{
			throw "Unvalid type in a dmd-file"; // TODO är detta rätt sätt att göra det på?
		}
	} while (!tempData->GetNext(&tempData));
	return TupleInt(xValue, yValue);
}

void GameBoardObject::CopyGameBoardObject(const GameBoardObject& gameBoardObject)
{
	_graphicalPos = gameBoardObject._graphicalPos;
	_scale = gameBoardObject._scale;
	_image = gameBoardObject._image;
}

/*void GameBoardObject::CleanUpGameObjectMap()
{
	//TODO ta bort funktionen, den ska inte användas. 
	while (_gameBoardObjects.size() > 0)
	{
		std::map<int, GameBoardObject*>::iterator it;
		it = _gameBoardObjects.begin();
		it->second->~GameBoardObject();
	}

}*/