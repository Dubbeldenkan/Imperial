#include "Player.h"

Player::Player(int playerPos) :
	GameBoardObject()
{
	_playerPos = playerPos;
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
	//TODO
}

Player::~Player()
{}

void Player::DrawObject()
{

}

void Player::Save(NodeParserNS::ListNode** playerNode) const
{
	
}