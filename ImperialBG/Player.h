#ifndef PLAYER_H
#define PLAYER_H

#include "Bond.h"
#include "GameBoardObject.h"
#include "NodeParser.h"

class Player : public GameBoardObject
{
public:
	static const std::string _playerSettingsFilePath;

protected:
	int _playerPos;
	std::string _name;
	int _money;
	static int _investorId;
	typedef std::map<Bond::BondNation, std::map<int, Bond*>> bondMapType;
	bondMapType _bonds;
	std::vector<Bond::BondNation> _government;

private:
	TupleInt _moneyGraphicalPos = TupleInt(200, 0);
	static constexpr GameBoardObject::ImageLayer _imageLayerValue = GameBoardObject::ImageLayer::Player;
	
public:
	Player();
	Player(int playerPos, std::string name, TupleInt objectPos);
	Player(NodeParserNS::ListNode*);

	Player& operator=(const Player&);
	Player(Player const&);
	~Player();

	void Save(NodeParserNS::ListNode**) const;
	void SetStartMoney(int);
	void BuyBond(Bond*);
	void SetAsInvestor();

	int GetBondNationValue(Bond::BondNation) const;
	int GetPlayerPos() const;
	virtual bool IsHuman() const;
	int GetMaxNumberOfRondelSteps() const;
	void AddMoney(int moneyChange);
	int GetMoney() const;
	int GetInterestValue(Bond::BondNation nation) const;

	static bool ExtractPlayerData(NodeParserNS::ListNode* playerData, std::string& name, TupleInt& objectPos);

	void ClearGovernments();
	void AddGovernment(Bond::BondNation nation);

protected:
	void CopyPlayer(Player const&);

private:
	void DrawObject() const;
	void UpdateBondsGraphicalPos() const;
};

#endif // !PLAYER_H