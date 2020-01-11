#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "Player.h"

class HumanPlayer : public Player
{
public:
	HumanPlayer();
	HumanPlayer(int playerPos, std::string name, TupleInt objectPos);

	HumanPlayer& operator=(const HumanPlayer& humanPlayer);
	HumanPlayer(HumanPlayer const& humanPlayer);
	~HumanPlayer();

	bool IsHuman() const;

private:
	void CopyHumanPlayer(const HumanPlayer& humanPlayer);
};

#endif // !HUMAN_PLAYER_H