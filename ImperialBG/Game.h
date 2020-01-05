#ifndef GAME_H
#define GAME_H

#include "GameBoard.h"
#include "Nation.h"
#include "Player.h"

#include <algorithm>
#include <stdlib.h>
#include <time.h> 
#include <vector>

class Game
{
private:
	std::vector<Player> _players;
	std::vector<Nation> _nations;
	GameBoard _gameBoard = GameBoard();

	bool _gameOver = false;

	static Player* _currentNation;

	static bool _initNewGame;
	static bool _saveGame;
	static bool _loadGame;

public:
	Game();
	~Game() {};

	void Run();

	bool GetGameOver();
	static void SetInitNewGame();
	static void SetSaveGame();
	static void SetLoadGame();

	static void MouseClicked(TupleInt);
	static void MouseMoved(TupleInt);
private:
	void DrawScreen();
	void InitGame();

	void SaveGame();
	void CreateSaveNode(NodeParserNS::ListNode*);
	void SaveToFile(NodeParserNS::ListNode*);

	void LoadGame();
	NodeParserNS::ListNode* LoadFromFile(std::string);
};

#endif // !GAME_H