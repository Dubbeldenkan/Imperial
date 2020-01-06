#include "Game.h"

Player* Game::_currentNation = 0;
bool Game::_initNewGame = false;
bool Game::_saveGame = false;
bool Game::_loadGame = false;

Game::Game()
{
	InitGame();
}

void Game::Run()
{
	if (_saveGame)
	{
		SaveGame();
	}

	if (_initNewGame)
	{
		//TODO starta om ett spel här..
	}
	else if (_loadGame)
	{
		LoadGame();
	}

	//TODO ändra till ett lämpligt villkor
	else if (true)
	{
		//_gameBoard.Run(_currentPlayer); TODO
		DrawScreen();
	}
	else
	{
		_gameOver = true;
	}
}

void Game::DrawScreen()
{
	_gameBoard.Draw();
}

bool Game::GetGameOver()
{
	return _gameOver;
}

void Game::SetInitNewGame()
{
	_initNewGame = true;
}

void Game::SetSaveGame()
{
	_saveGame = true;
}

void Game::SetLoadGame()
{
	_loadGame = true;
}

void Game::InitGame()
{
	//Setup players
	NodeParserNS::ListNode* playerData = NodeParserNS::NodeParser::ReadDataFile(Player::_playerSettingsFilePath);
	playerData->GetChild(&playerData);
	do{
		_players.push_back(Player(static_cast<int>(_players.size()), playerData));

	} while (!playerData->GetNext(&playerData));
	
	
	for (int playerCounter = 0; playerCounter < static_cast<int>(_players.size()); playerCounter++)
	{
		_players[playerCounter].SetStartMoney(_players.size());
	}

	//Setup nations
	for (int nationCounter = 0; nationCounter < _numberOfNations; nationCounter++)
	{
		_nations.push_back(Nation(static_cast<int>(nationCounter)));
	}

	//Assign start bonds
	std::vector<Player*> tempPlayerVector;
	for (int playerCounter = 0; playerCounter < static_cast<int>(_players.size()); playerCounter++)
	{
		tempPlayerVector.push_back(&_players[playerCounter]);
	}

	int nationCounter = 0;
	while(!tempPlayerVector.empty()) //TODO fixa så att det är random även vilken nation som väljs först
	{
		int randomPlayer = rand() % tempPlayerVector.size();
		tempPlayerVector[randomPlayer]->BuyBond(_nations[nationCounter].SellBond(Bond::startBondValueLarge));
		std::string smallBondNationName = _nations[nationCounter].GetStartBondSmallNationName();
		Nation* smallBondNation = NULL;
		int smallBondNationCounter = 0;
		while (smallBondNation == NULL)
		{
			std::string smallBondNationName = _nations[smallBondNationCounter].GetName();
			std::string largeNationSmallBondName = _nations[nationCounter].GetStartBondSmallNationName();
			int test = _nations[smallBondNationCounter].GetName().compare(_nations[nationCounter].GetStartBondSmallNationName()) == 0;
			if (_nations[smallBondNationCounter].GetName().compare(_nations[nationCounter].GetStartBondSmallNationName()) == 0)
			{
				smallBondNation = &_nations[smallBondNationCounter];
			}
			smallBondNationCounter++;
		}
		tempPlayerVector[randomPlayer]->BuyBond(smallBondNation->SellBond(Bond::startBondValueSmall));
		tempPlayerVector.erase(tempPlayerVector.begin() + randomPlayer);
		nationCounter++;
	}

	//Set investor start player

}

void Game::MouseClicked(TupleInt mouseClickedPos)
{
	std::vector<GameBoardObject*> clickedObjects = GameBoard::GetGameBoardObjectByPosition(mouseClickedPos);
	for (int vectorCount = 0; vectorCount < static_cast<int>(clickedObjects.size()); vectorCount++)
	{
		GameBoardObject* object = clickedObjects[vectorCount];
		/*if (_currentPlayer->Action(object, mouseClickedPos))
		{
			break;
		}*/
	}
}

void Game::MouseMoved(TupleInt mousePos)
{
	
}

void Game::SaveGame()
{
	_saveGame = false;
	NodeParserNS::ListNode gameToSave = NodeParserNS::ListNode("Game");
	CreateSaveNode(&gameToSave);
	SaveToFile(&gameToSave);
}

void Game::CreateSaveNode(NodeParserNS::ListNode* gameNode)
{
	//Gameboard
	NodeParserNS::ListNode* gameBoardNode = _gameBoard.Save();
	gameNode->SetChild(gameBoardNode);

	// Players
	NodeParserNS::ListNode* playersNode = new NodeParserNS::ListNode("Players");
	gameBoardNode->SetNext(playersNode);
	NodeParserNS::ListNode* currentNode = new NodeParserNS::ListNode(""); //TODO är detta en poteentiell minnesläcka?
	NodeParserNS::ListNode* oldNode = NULL;
	for (int playerCount = 0; playerCount < static_cast<int>(_players.size()); playerCount++)
	{
		_players[playerCount].Save(&currentNode);
		if (oldNode == NULL) //TODO kan man göra detta på ett snyggare sätt?
		{
			playersNode->SetChild(currentNode);
		}
		else
		{
			oldNode->SetNext(currentNode);
		}
		oldNode = currentNode;
	}
}

void Game::SaveToFile(NodeParserNS::ListNode* gameToSave)
{
	time_t rawtime = time(NULL);
	struct tm timeInfo;
	char buffer[80];

	time(&rawtime);
	localtime_s(&timeInfo, &rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", &timeInfo);

	std::string fileName = "savedGame_";
	fileName = fileName + buffer;

	NodeParserNS::NodeParser::WriteToFile(gameToSave, fileName);
}

void Game::LoadGame()
{
	//TODO det verkar vara en minnesförlust när man laddar. Varje gång man gör det ökar ram med 4 MB
	_loadGame = false;
	_gameBoard.CleanUpGameBoard();
	_players.clear();

	NodeParserNS::ListNode* loadedGame = LoadFromFile("savedGame_29-09-2019_22-44-33"); //TODO gör så att man kan välja fil

	//Gameboard
	NodeParserNS::ListNode* gameBoardNode = NULL;
	loadedGame->GetChild(&gameBoardNode);
	_gameBoard.Load(gameBoardNode);
	
	// Players
	NodeParserNS::ListNode* players = NULL;
	NodeParserNS::ListNode* player = NULL;
	gameBoardNode->GetNext(&players);
	
	players->GetChild(&player);
	int playerCounter = 0;
	do
	{
		_players.push_back(Player(player));
		//_players[playerCounter++].Load(player, _gameBoard.GetGameMap());
	} while (!player->GetNext(&player));
}

NodeParserNS::ListNode* Game::LoadFromFile(std::string fileName)
{
	return NodeParserNS::NodeParser::ReadSaveFile(fileName);
}