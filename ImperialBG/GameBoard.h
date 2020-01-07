#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "GameBoardObject.h"
#include "Map.h"
#include "Player.h"

class GameBoard
{
private:
	Map _map;

public:
	GameBoard();
	~GameBoard();

	void CleanUpGameBoard();
	void Draw();
	void Run(Player*);
	NodeParserNS::ListNode* Save() const;
	void Load(NodeParserNS::ListNode*);


	static std::vector<GameBoardObject*> GetGameBoardObjectByPosition(TupleInt);
};

#endif // !GAME_BOARD_H