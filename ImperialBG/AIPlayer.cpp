#include "AIPlayer.h"

AIPlayer::AIPlayer()
{
}

AIPlayer::AIPlayer(int playerPos, std::string name, TupleInt objectPos) :
	Player(playerPos, name, objectPos)
{

}

AIPlayer::~AIPlayer()
{
}

bool AIPlayer::IsHuman() const
{
	return false;
}