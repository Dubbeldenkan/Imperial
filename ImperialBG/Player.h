#ifndef PLAYER_H
#define PLAYER_H

#include "NodeParser.h"
#include "GameBoardObject.h"

class Player : public GameBoardObject
{
private:
	static const GameBoardObject::LayerEnum _layerValue = GameBoardObject::PlayerLayer;
	int _playerPos;

public:
	Player(int);
	Player(NodeParserNS::ListNode*);

	Player& operator=(const Player&);
	Player(Player const&);
	~Player();

	void Save(NodeParserNS::ListNode**) const;

private:
	void CopyPlayer(Player const&);
	
	void DrawObject() const;
};

#endif // !PLAYER_H