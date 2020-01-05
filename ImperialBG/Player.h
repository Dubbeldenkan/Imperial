#ifndef PLAYER_H
#define PLAYER_H

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

private:
	void CopyPlayer(Player const&);
	
	void DrawObject() const;
};

#endif // !PLAYER_H