#pragma once
#include <queue>

#include "json/json.h"
#include "basic.h"

class RoleCard: public Card {
public:
	RoleCard();

private:
	std::string name;
};

class SpotCard : public Card {
public:
	SpotCard();

private:
	std::string name;
};

class ItemCard :public Card {
public:
	ItemCard();

private:
	std::string name;
};

class CardSet {
public:
	std::priority_queue<std::string>& getCardset() {
		return cardSet;
	}
	int getCardSum() {
		return cardSum;
	}
private:
	int cardSum;
	std::string careSetName;
	std::priority_queue<std::string> cardSet;

public:
	bool check(CardSet a, CardSet b);
	friend void readCardSetJson();
};