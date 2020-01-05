#include "GameBoard.h"

GameBoard::GameBoard()
{
}

GameBoard::~GameBoard()
{
}

void GameBoard::Draw()
{
	GameBoardObject::DrawAllObjects();
}

std::vector<GameBoardObject*> GameBoard::GetGameBoardObjectByPosition(TupleInt position)
{
	std::vector<GameBoardObject*> objectVector;
	std::map<int, GameBoardObject*>::iterator it;
	for (it = GameBoardObject::_gameBoardObjects.begin(); it != GameBoardObject::_gameBoardObjects.end(); it++)
	{
		if (it->second->PosInObject(position))
		{
			int test = 0;
			objectVector.push_back(it->second);
		}
	}
	return objectVector;
}

NodeParserNS::ListNode* GameBoard::Save() const
{
	//TODO
	NodeParserNS::ListNode* gameBoardNode = new NodeParserNS::ListNode("GameBoard");
	
	return gameBoardNode;
}

void GameBoard::Load(NodeParserNS::ListNode* gameBoardNode)
{
	//TODO
	NodeParserNS::ListNode* gameMapNode = NULL;
	gameBoardNode->GetChild(&gameMapNode);
}

void GameBoard::CleanUpGameBoard()
{

}

void GameBoard::Run(Player* player)
{

}