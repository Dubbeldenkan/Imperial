#ifndef PLAYER_H
#define PLAYER_H

#include "Bond.h"
#include "NodeParser.h"
#include "GameBoardObject.h"

class Player : public GameBoardObject
{
public:
	static const std::string _playerSettingsFilePath;

private:
	int _playerPos;
	std::string _name;
	int _money;
	std::vector<Bond*> _bonds;

	TupleInt _moneyGraphicalPos = TupleInt(200, 0);
	static const GameBoardObject::LayerEnum _layerValue = GameBoardObject::PlayerLayer;
	
public:
	Player(int, NodeParserNS::ListNode*);
	Player(NodeParserNS::ListNode*);

	Player& operator=(const Player&);
	Player(Player const&);
	~Player();

	void Save(NodeParserNS::ListNode**) const;
	void SetStartMoney(int);
	void BuyBond(Bond*);

private:
	void CopyPlayer(Player const&);
	
	void DrawObject() const;
	void UpdateBondsGraphicalPos() const;
};

#endif // !PLAYER_H