#include "Player.h"

const std::string Player::_playerSettingsFilePath = "PlayerSettings";

Player::Player(int playerPos, NodeParserNS::ListNode* playerNode) :
	GameBoardObject()
{
	_playerPos = playerPos;
	NodeParserNS::ListNode* tempData;
	playerNode->GetChild(&playerNode);

	do {
		if (playerNode->GetData().compare("Name") == 0)
		{
			playerNode->GetChild(&tempData);
			_name = tempData->GetData();
		}
		else if (playerNode->GetData().compare("ObjectPos") == 0)
		{
			int xValue;
			int yValue;
			playerNode->GetChild(&tempData);
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
					throw "Unvalid type in " + _name + ".dmd"; // TODO är detta rätt sätt att göra det på?
				}
			} while (!tempData->GetNext(&tempData));
			SetGraphicalPos(TupleInt(xValue, yValue));
		}
		else
		{
			throw "Unvalid type in " + _name + ".dmd"; // TODO är detta rätt sätt att göra det på?
		}
	} while (!playerNode->GetNext(&playerNode));
}

Player::Player(NodeParserNS::ListNode* playerToLoad) :
	GameBoardObject()
{

}

Player& Player::operator=(const Player& player)
{
	CopyPlayer(player);
	return *this;
}

Player::Player(const Player &player) :
	GameBoardObject(player._graphicalPos, player._image, _layerValue)
{
	CopyPlayer(player);
}

void Player::CopyPlayer(const Player& player)
{
	_playerPos = player._playerPos;
	_name = player._name;
	//TODO
}

Player::~Player()
{}

void Player::DrawObject() const
{
	_g->PrintText15(_name, _graphicalPos.GetX(), _graphicalPos.GetY(), GraphicsNS::Graphics::WHITE);
	_g->PrintText15("Millions: " + std::to_string(_money), 
		_graphicalPos.GetX() + _moneyGraphicalPos.GetX(), _graphicalPos.GetY(), GraphicsNS::Graphics::WHITE);

	if (_isInvestor)
	{
		_g->PrintText15("Investor", _graphicalPos.GetX(), _graphicalPos.GetY(), GraphicsNS::Graphics::WHITE);
	}

	UpdateBondsGraphicalPos();
}

void Player::UpdateBondsGraphicalPos() const
{
	bondMapType::const_iterator nationIterator;
	std::map<int, Bond*>::const_iterator bondIterator;

	for (nationIterator = _bonds.begin(); nationIterator != _bonds.end(); nationIterator++)
	{
		for (bondIterator = nationIterator->second.begin(); bondIterator != nationIterator->second.end(); bondIterator++)
		{
			int xPos = _graphicalPos.GetX() + 
				static_cast<int>(bondIterator->second->GetImageSize().GetX()*(bondIterator->second->GetId() - 1));
			int yPos = _graphicalPos.GetY() + static_cast<int>(bondIterator->second->GetImageSize().GetY()*1.5);
			bondIterator->second->SetGraphicalPos(TupleInt(xPos, yPos));
		}
	}
}

void Player::Save(NodeParserNS::ListNode** playerNode) const
{
	
}

void Player::SetStartMoney(int numberOfPlayers)
{
	if (numberOfPlayers > 3)
	{
		_money = 13;
	}
	else if (numberOfPlayers == 3)
	{
		_money = 24;
	}
	else if (numberOfPlayers == 2)
	{
		_money = 35;
	}
	else
	{
		throw "Unvalid number of players";
	}
}

void Player::BuyBond(Bond* bond)
{
	_money = _money - bond->GetValue();
	_bonds[bond->GetBondNation()].insert(std::pair<int, Bond*>(bond->GetId(), bond));
}

int Player::GetBondNationValue(Bond::BondNation bondNation) const
{
	int totalBondValue = 0;
	if (_bonds.find(bondNation) != _bonds.end())
	{
		std::map<int, Bond*>::const_iterator bondIterator;
		bondIterator = _bonds.find(bondNation)->second.begin();
		for (bondIterator = _bonds.find(bondNation)->second.begin(); 
			bondIterator != _bonds.find(bondNation)->second.end(); bondIterator++)
		{
			totalBondValue = +bondIterator->second->GetValue();
		}
	}
	return totalBondValue;
}

int Player::GetPlayerPos() const
{
	return _playerPos;
}

void Player::SetAsInvestor()
{
	_isInvestor = true;
}