#pragma once
#include <queue>

#include "basic.h"
#include "json/json.h"

class CardSet {
public:
	CardSet() {};
	CardSet(const std::vector<std::shared_ptr<Card>>& stack, std::string& valueMatch);
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