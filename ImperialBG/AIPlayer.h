#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "Player.h"

class AIPlayer : public Player
{
public:
	//TODO lägg till copyconstruktorer
	AIPlayer();
	AIPlayer(int playerPos, std::string name, TupleInt objectPos);

	~AIPlayer();
};

#endif // !AI_PLAYER_H

