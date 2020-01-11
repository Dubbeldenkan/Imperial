#include "HumanPlayer.h"

HumanPlayer::HumanPlayer()
{}

HumanPlayer::HumanPlayer(int playerPos, std::string name, TupleInt objectPos) :
	Player(playerPos, name, objectPos)
{
}

HumanPlayer& HumanPlayer::operator=(const HumanPlayer& humanPlayer)
{
	CopyHumanPlayer(humanPlayer);
	return *this;
}

HumanPlayer::HumanPlayer(const HumanPlayer &humanPlayer) :
	Player(humanPlayer._playerPos, humanPlayer._name, humanPlayer._graphicalPos)
{
	CopyHumanPlayer(humanPlayer);
}

void HumanPlayer::CopyHumanPlayer(const HumanPlayer& humanPlayer)
{
	CopyPlayer(humanPlayer);
}

HumanPlayer::~HumanPlayer()
{
}

bool HumanPlayer::IsHuman()
{
	return true;
}