#pragma once
#include <queue>

#include "json/json.h"
#include "basic.h"

class CardSet {
public:
	CardSet() {};
	CardSet(const std::vector<std::shared_ptr<Card>>& stack);
	std::priority_queue<std::string>& getCardset() {
		return cardSet;
	}
	int getCardSum() {
		return cardSum;
	}
	void showCardDetail();
private:
	int cardSum;
	std::priority_queue<std::string> cardSet;

public:
	bool operator< (const CardSet& cmp)const;
	bool operator== (const CardSet& cmp)const;

public:
	friend void readCardSetJson();
};