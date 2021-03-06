#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "AIPlayer.h"
#include "GameBoard.h"
#include "HumanPlayer.h"
#include "Nation.h"

#include <algorithm>
#include <stdlib.h>
#include <time.h> 
#include <vector>

class Game
{
private:
	std::vector<Player*> _players;
	std::map<int, HumanPlayer> _humanPlayers;
	std::map<int, AIPlayer> _aiPlayers;
	std::vector<Nation> _nations;
	GameBoard _gameBoard = GameBoard();

	static std::vector<GameBoardObject*> _selectedObjects;

	bool _humanPlayerPlaying = false;

	bool _gameOver = false;

	static Player* _investorPlayer;
	static Nation* _currentNation;
	static std::map<Nation*, Player*> _governmentMap;

	static bool _initNewGame;
	static bool _saveGame;
	static bool _loadGame;
	static constexpr int _numberOfPlayers = 6;
	static constexpr int _numberOfNations = 6;

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
	void PlayingPassiveAction();
	static void PlayingActiveAction(GameBoardObject* gbo);
	void InterestPayout();
	void BuyAsInvestor();

	void SetCurrentNation(Nation* nation);
	void SetNextNation();
	void SetNextInvestor();
	void SetGovernment();

	void SaveGame();
	void CreateSaveNode(NodeParserNS::ListNode*);
	void SaveToFile(NodeParserNS::ListNode*);

	void LoadGame();
	NodeParserNS::ListNode* LoadFromFile(std::string);
};

#endif // !GAME_H