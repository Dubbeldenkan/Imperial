#include "Game.h"

Nation* Game::_currentNation = NULL;
bool Game::_initNewGame = false;
bool Game::_saveGame = false;
bool Game::_loadGame = false;

std::vector<GameBoardObject*> Game::_selectedObjects;
std::map<Nation*, Player*> Game::_governmentMap;
Player* Game::_investorPlayer;

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
	//TODO ändra till ett lämpligt villkor tex när något land når 25 i tabellen
	else if (true)
	{
		if (_currentNation->GetNationState() == Nation::NationGameState::PlayingAction)
		{
			PlayingPassiveAction();
		}
		else if(_currentNation->GetNationState() == Nation::NationGameState::Done)
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
	while(!tempPlayerVector.empty()) //TODO fixa så att det är random även vilken nation som väljs först
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

	//Define governments
	SetGovernment();

	//Set investor start player
	int firstInvestorPlayer = (_governmentMap[_currentNation]->GetPlayerPos() + 1) % _numberOfPlayers;

	for (int playerCounter = 0; playerCounter < _numberOfPlayers; playerCounter++)
	{
		if (_players[playerCounter]->GetPlayerPos() == firstInvestorPlayer)
		{
			_investorPlayer = _players[playerCounter];
			_investorPlayer->SetAsInvestor();
		}
	}
}

void Game::MouseClicked(TupleInt mouseClickedPos)
{
	// Om spelaren är en AI ska man inte fortsätta i denna funktion
	if (!_governmentMap[_currentNation]->IsHuman())
	{
		return;
	}
	const std::vector<GameBoardObject*> clickedObjects = GameBoard::GetGameBoardObjectByPosition(mouseClickedPos);
	for (int vectorCount = 0; vectorCount < static_cast<int>(clickedObjects.size()); vectorCount++)
	{
		GameBoardObject* object = clickedObjects[vectorCount];
		switch (_currentNation->GetNationState())
		{
		case Nation::NationGameState::PlacingRondelIndicator:
		{
			if (_currentNation->GetRondelIndicatorID() == object->GetObjectID())
			{
				object->Action(mouseClickedPos);
				int maxNumberExtraSteps = _governmentMap[_currentNation]->GetMaxNumberOfRondelSteps();
				const int playerCost = _currentNation->MoveRondelIndicator(maxNumberExtraSteps);
				_governmentMap[_currentNation]->AddMoney(-playerCost);
				return; //TODO ska man ha en return såhär?
			}
			break;
		}
		case Nation::NationGameState::PlayingAction:
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
	case RondelIndicator::RondelPos::Factory:
	{
		_currentNation->SetDrawFactorySites();
		break;
	}
	case RondelIndicator::RondelPos::Investor:
	{
		switch (_currentNation->GetInvestorState())
		{
		case Nation::InvestorState::InterestPayout:
		{
			constexpr int investorMoney = 2; //TODO kanske flytta den till en member istället
			InterestPayout();
			_currentNation->SetInvestorState();
			_investorPlayer->AddMoney(investorMoney);
			break;
		}
		case Nation::InvestorState::Investor:
		{
			BuyAsInvestor();
			SetGovernment();
			break;
		}
		default:
		{
			break;
		}
		}
		break;
	}
	case RondelIndicator::RondelPos::Import:
	{
		_currentNation->SetDrawFactorySites();
		for (int gboIndex = 0; gboIndex < static_cast<int>(_selectedObjects.size()); ++gboIndex)
		{
			_currentNation->ProductionAction(_selectedObjects[gboIndex]);
		}
		break;
	}
	case RondelIndicator::RondelPos::ManeuverLeft:
	case RondelIndicator::RondelPos::ManeuverRight:
	{
		if (_currentNation->GetManeuverState() == Nation::ManeuverState::Start)
		{
			_currentNation->StartManeuver();
		}
		break;
	}
	default:
		break;
	}
	_selectedObjects.clear();
}

void Game::PlayingActiveAction(GameBoardObject* gbo)
{
	switch (_currentNation->GetRondelState())
	{
	case RondelIndicator::RondelPos::Factory:
	{
		for (int regionIndex = 0; regionIndex < _currentNation->GetNumberOfRegions(); regionIndex++)
		{
			if (_currentNation->GetRegion(regionIndex) == gbo)
			{
				_currentNation->BuildFactory(gbo);
				break;
			}
		}
		break;
	}
	case RondelIndicator::RondelPos::Investor:
	{
		_selectedObjects.push_back(gbo);
		break;
	}
	case RondelIndicator::RondelPos::Import:
	{
		_selectedObjects.push_back(gbo);
		if (_currentNation->GetNumberToImport() == 0)
		{
			_currentNation->SetToImport();
		}
		break;
	}
	case RondelIndicator::RondelPos::ManeuverLeft:
	case RondelIndicator::RondelPos::ManeuverRight:
	{
		//TODO fortsätt här gör så att man kan välja vilken gubbe att flytta och markera. 
		//Gör så att man kan först flyttar båtar sedan fotfolk
		std::list<Unit>::const_iterator unitIt = _currentNation->GetFirstUnit();
		do
		{
			if (unitIt->GetObjectID() == gbo->GetObjectID())
			{
				Unit* unit = static_cast<Unit*>(gbo);
				unit->SetUnitToSelected();
				break;
			}
		} while (_currentNation->GetNextUnit(unitIt));
		break;
	}
	default:
		break;
	}
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
		if (_nations[index] == *_currentNation)
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

void Game::SetNextInvestor()
{
	for (int index = 0; index < static_cast<int>(_players.size()); index++)
	{
		if (_players[index] == _investorPlayer)
		{
			if (index == (_numberOfNations - 1))
			{
				_investorPlayer	 = _players[0];
				break;
			}
			else
			{
				_investorPlayer = _players[index + 1];
				break;
			}
		}
	}
	_investorPlayer->SetAsInvestor();
}

void Game::InterestPayout()
{
	for (int playerIndex = 0; playerIndex < static_cast<int>(_players.size()); playerIndex++)
	{
		if (_players[playerIndex] == _governmentMap[_currentNation])
		{
			do
			{
				playerIndex = (playerIndex + 1) % _numberOfPlayers;
				const int interest = _players[playerIndex]->GetInterestValue(_currentNation->GetBondNation());
				if (_currentNation->GetMoney() > interest)
				{
					_currentNation->AddMoney(-interest);
					_players[playerIndex]->AddMoney(interest);
				}
				else
				{
					const int missingMoney = interest - _currentNation->GetMoney();
					_currentNation->AddMoney(-_currentNation->GetMoney());
					_governmentMap[_currentNation]->AddMoney(-missingMoney); //TODO lägg in vad som händer om ägaren inte har råd
					_players[playerIndex]->AddMoney(interest);
				}
			} while (_players[playerIndex] != _governmentMap[_currentNation]);
			break;
		}
	}
}

void Game::BuyAsInvestor()
{
	for (int selectedObjectsIndex = 0; selectedObjectsIndex < static_cast<int>(_selectedObjects.size()); ++selectedObjectsIndex)
	{
		Bond* selectedBond = NULL;
		for (int nationIndex = 0; nationIndex < static_cast<int>(_nations.size()); ++nationIndex)
		{
			selectedBond = _nations[nationIndex].GetUnboughtBond(_selectedObjects[selectedObjectsIndex]);
			if (selectedBond != NULL)
			{
				if (_investorPlayer->GetMoney() >= selectedBond->GetValue())
				{
					_investorPlayer->BuyBond(_nations[nationIndex].SellBond(selectedBond->GetId()));
					//TODO gör så att man kan vara swiss bank
					//TODO gör så att man kan växla in egna bonds
					SetNextInvestor();
					_currentNation->SetToDone();
				}
				break;
			}
		}
	}
}

void Game::SetGovernment()
{
	for (int playerCounter = 0; playerCounter < static_cast<int>(_players.size()); ++playerCounter)
	{
		_players[playerCounter]->ClearGovernments();
	}
	for (int nationCounter = 0; nationCounter < _numberOfNations; nationCounter++)
	{
		_governmentMap[&_nations[nationCounter]] = NULL;
		int bestPlayerValue = 0;
		for (int playerCounter = 0; playerCounter < _numberOfPlayers; playerCounter++)
		{
			if (bestPlayerValue < _players[playerCounter]->GetBondNationValue(_nations[nationCounter].GetBondNation()))
			{
				_governmentMap[&_nations[nationCounter]] = _players[playerCounter];
				bestPlayerValue = _players[playerCounter]->GetBondNationValue(_nations[nationCounter].GetBondNation());
			}
		}
	}
	std::map<Nation*, Player*>::iterator it;
	for (it = _governmentMap.begin(); it != _governmentMap.end(); ++it)
	{
		it->second->AddGovernment(it->first->GetBondNation());
	}
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
		_players[playerCount]->Save(&currentNode);
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
		//_players.push_back(Player(player)); //TODO fixa för human och AI players
		//_players[playerCounter++].Load(player, _gameBoard.GetGameMap());
	} while (!player->GetNext(&player));
}

NodeParserNS::ListNode* Game::LoadFromFile(std::string fileName)
{
	return NodeParserNS::NodeParser::ReadSaveFile(fileName);
}