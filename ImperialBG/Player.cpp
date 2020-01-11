#include "Player.h"

const std::string Player::_playerSettingsFilePath = "PlayerSettings";

Player::Player()
{}

Player::Player(int playerPos, std::string name, TupleInt objectPos) :
	_playerPos(playerPos), _name(name), GameBoardObject(objectPos, NULL, _imageLayerValue)
{	
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
	GameBoardObject(player._graphicalPos, player._image, _imageLayerValue)
{
	CopyPlayer(player);
}

void Player::CopyPlayer(const Player& player)
{
	CopyGameBoardObject(player);
	_playerPos = player._playerPos;
	_name = player._name;
	_money = player._money;
}

Player::~Player()
{}

void Player::DrawObject() const
{
	if (_isInvestor)
	{
		_g->DrawRectangle(_graphicalPos.GetX(), _graphicalPos.GetY(), 100, 20, GraphicsNS::Graphics::Color::WHITE);
		_g->PrintText(_name, _graphicalPos.GetX(), _graphicalPos.GetY(), 
			GraphicsNS::Graphics::Color::BLACK, GraphicsNS::Graphics::FontSize::font15);
	}
	else
	{
		_g->PrintText(_name, _graphicalPos.GetX(), _graphicalPos.GetY(), 
			GraphicsNS::Graphics::Color::WHITE, GraphicsNS::Graphics::FontSize::font15);
	}
	_g->PrintText("Millions: " + std::to_string(_money), _graphicalPos.GetX() + _moneyGraphicalPos.GetX(), 
		_graphicalPos.GetY(), GraphicsNS::Graphics::Color::WHITE, GraphicsNS::Graphics::FontSize::font15);

	UpdateBondsGraphicalPos();
}

void Player::UpdateBondsGraphicalPos() const //TODO denna funktion måste uppdateras för att man ska kunna ha flera bonds med samma värde, annars kommer de att rites ut över varandra 
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

bool Player::ExtractPlayerData(NodeParserNS::ListNode* playerData, std::string &name, TupleInt& objectPos)
{
	bool humanPlayer = false;
	
	NodeParserNS::ListNode* tempData;
	playerData->GetChild(&playerData);

	do {
		if (playerData->GetData().compare("Name") == 0)
		{
			playerData->GetChild(&tempData);
			name = tempData->GetData();
		}
		else if (playerData->GetData().compare("PlayerType") == 0)
		{
			playerData->GetChild(&tempData);
			const std::string playerTypeStr = tempData->GetData();
			if (playerTypeStr.compare("Human") == 0)
			{
				humanPlayer = true;
			}
		}
		else if (playerData->GetData().compare("ObjectPos") == 0)
		{
			objectPos = ExtractPos(playerData);
		}
		else
		{
			throw "Unvalid type in " + name + ".dmd"; // TODO är detta rätt sätt att göra det på?
		}
	} while (!playerData->GetNext(&playerData));

	return humanPlayer;
}

bool Player::IsHuman()
{
	return false;
}