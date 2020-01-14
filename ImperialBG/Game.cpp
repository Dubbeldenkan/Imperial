#include "Game.h"

Nation* Game::_currentNation = NULL;
bool Game::_initNewGame = false;
bool Game::_saveGame = false;
bool Game::_loadGame = false;

std::map<Nation*, Player*> Game::_govermentMap;

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
		//TODO starta om ett spel h�r..
	}
	else if (_loadGame)
	{
		LoadGame();
	}
	//TODO �ndra till ett l�mpligt villkor tex n�r n�got land n�r 25 i tabellen
	else if (true)
	{
		if (_currentNation->GetNationState() == Nation::NationGameState::playingAction)
		{
			PlayingPassiveAction();
		}
		else if(_currentNation->GetNationState() == Nation::NationGameState::done)
		{
			SetNextNation();
		}
		
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
		std::string name;
		TupleInt objectPos;
		const bool humanPlayer = Player::ExtractPlayerData(playerData, name, objectPos);
		int playerPos = static_cast<int>(_players.size());
		if (humanPlayer)
		{
			_humanPlayers.insert(std::pair<int, HumanPlayer>(playerPos, HumanPlayer(playerPos, name, objectPos)));
			_players.push_back(&_humanPlayers[playerPos]);
		}
		else
		{
			_aiPlayers.insert(std::pair<int, AIPlayer>(playerPos, AIPlayer(playerPos, name, objectPos)));
			_players.push_back(&_aiPlayers[playerPos]);
		}
	} while (!playerData->GetNext(&playerData));
	
	
	for (int playerCounter = 0; playerCounter < static_cast<int>(_players.size()); playerCounter++)
	{
		_players[playerCounter]->SetStartMoney(_players.size());
	}

	//Setup nations
	for (int nationCounter = 0; nationCounter < _numberOfNations; nationCounter++)
	{
		_nations.push_back(Nation(static_cast<int>(nationCounter)));
	}

	//Set current Nation
	SetCurrentNation(&_nations[0]);

	//Assign start bonds
	std::vector<Player*> tempPlayerVector;
	for (int playerCounter = 0; playerCounter < static_cast<int>(_players.size()); playerCounter++)
	{
		tempPlayerVector.push_back(_players[playerCounter]);
	}

	int nationCounter = 0;
	while(!tempPlayerVector.empty()) //TODO fixa s� att det �r random �ven vilken nation som v�ljs f�rst
	{
		const int randomPlayer = rand() % tempPlayerVector.size();
		tempPlayerVector[randomPlayer]->BuyBond(_nations[nationCounter].SellBond(Bond::startBondValueLarge));
		const std::string smallBondNationName = _nations[nationCounter].GetStartBondSmallNationName();
		Nation* smallBondNation = NULL;
		int smallBondNationCounter = 0;
		while (smallBondNation == NULL)
		{
			const std::string smallBondNationName = _nations[smallBondNationCounter].GetName();
			const std::string largeNationSmallBondName = _nations[nationCounter].GetStartBondSmallNationName();
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

	//Define goverments
	for (int nationCounter = 0; nationCounter < _numberOfNations; nationCounter++)
	{
		_govermentMap[&_nations[nationCounter]] = NULL;
		int bestPlayerValue = 0;
		for (int playerCounter = 0; playerCounter < _numberOfPlayers; playerCounter++)
		{
			if (bestPlayerValue < _players[playerCounter]->GetBondNationValue(_nations[nationCounter].GetBondNation()))
			{
				_govermentMap[&_nations[nationCounter]] = _players[playerCounter];
				bestPlayerValue = _players[playerCounter]->GetBondNationValue(_nations[nationCounter].GetBondNation());
			}
		}
	}

	//Set investor start player
	int firstInvestorPlayer = (_govermentMap[_currentNation]->GetPlayerPos() + 1) % _numberOfPlayers;

	for (int playerCounter = 0; playerCounter < _numberOfPlayers; playerCounter++)
	{
		if (_players[playerCounter]->GetPlayerPos() == firstInvestorPlayer)
		{
			_players[playerCounter]->SetAsInvestor();
		}
	}
}

void Game::MouseClicked(TupleInt mouseClickedPos)
{
	// Om spelaren �r en AI ska man inte forts�tta i denna funktion
	if (!_govermentMap[_currentNation]->IsHuman())
	{
		return;
	}
	const std::vector<GameBoardObject*> clickedObjects = GameBoard::GetGameBoardObjectByPosition(mouseClickedPos);
	for (int vectorCount = 0; vectorCount < static_cast<int>(clickedObjects.size()); vectorCount++)
	{
		GameBoardObject* object = clickedObjects[vectorCount];
		switch (_currentNation->GetNationState())
		{
		case Nation::NationGameState::placingRondelIndicator:
		{
			if (_currentNation->GetRondelIndicatorID() == object->GetObjectID())
			{
				object->Action(mouseClickedPos);
				int maxNumberExtraSteps = _govermentMap[_currentNation]->GetMaxNumberOfRondelSteps();
				const int playerCost = _currentNation->MoveRondelIndicator(maxNumberExtraSteps);
				_govermentMap[_currentNation]->ChangeMoney(-playerCost);
				return; //TODO ska man ha en return s�h�r?
			}
			break;
		}
		case Nation::NationGameState::playingAction:
		{
			PlayingActiveAction(object);
		}
		}
	}
}

void Game::PlayingPassiveAction()
{
	switch (_currentNation->GetRondelState())
	{
	case RondelIndicator::RondelPos::ProductionLeft:
	case RondelIndicator::RondelPos::ProductionRight:
	{
		_currentNation->ProductionAction();
		break;
	}
	default:
		break;
	}
}

void Game::PlayingActiveAction(GameBoardObject* gbo)
{

}

void Game::MouseMoved(TupleInt mousePos)
{
	
}

void Game::SetCurrentNation(Nation* nation)
{
	_currentNation = nation;
	nation->SetAsCurrentNation();
}

void Game::SetNextNation()
{
	for (int index = 0; index < static_cast<int>(_nations.size()); index++) 
	{
		if (_nations[index].GetObjectID() == _currentNation->GetObjectID())
		{
			if (index == (_numberOfNations - 1))
			{
				_currentNation = &_nations[0];
				break;
			}
			else
			{
				_currentNation = &_nations[index + 1];
				break;
			}
		}
	}
	SetCurrentNation(_currentNation);
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
	NodeParserNS::ListNode* currentNode = new NodeParserNS::ListNode(""); //TODO �r detta en poteentiell minnesl�cka?
	NodeParserNS::ListNode* oldNode = NULL;
	for (int playerCount = 0; playerCount < static_cast<int>(_players.size()); playerCount++)
	{
		_players[playerCount]->Save(&currentNode);
		if (oldNode == NULL) //TODO kan man g�ra detta p� ett snyggare s�tt?
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
	//TODO det verkar vara en minnesf�rlust n�r man laddar. Varje g�ng man g�r det �kar ram med 4 MB
	_loadGame = false;
	_gameBoard.CleanUpGameBoard();
	_players.clear();

	NodeParserNS::ListNode* loadedGame = LoadFromFile("savedGame_29-09-2019_22-44-33"); //TODO g�r s� att man kan v�lja fil

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
		//_players.push_back(Player(player)); //TODO fixa f�r human och AI players
		//_players[playerCounter++].Load(player, _gameBoard.GetGameMap());
	} while (!player->GetNext(&player));
}

NodeParserNS::ListNode* Game::LoadFromFile(std::string fileName)
{
	return NodeParserNS::NodeParser::ReadSaveFile(fileName);
}