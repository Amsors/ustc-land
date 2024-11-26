#pragma once
#include <queue>

#include "json/json.h"
#include "basic.h"

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