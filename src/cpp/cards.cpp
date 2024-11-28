#include <vector>
#include <map>
#include <queue>

#include "game/logic/cards.h"
#include "game/logic/register.h"
#include "spdlog/spdlog.h"

CardSet::CardSet(std::vector<std::shared_ptr<Card>> stack) {
	int cardSum = stack.size();
	this->cardSum = cardSum;
	for (int i = 0; i < cardSum; i++) {
		this->cardSet.emplace(stack.at(i)->getName());
	}
	return;
}

bool CardSet::operator< (const CardSet& cmp) const {
	if (this->cardSum < cmp.cardSum) {
		return true;
	}
	else if (this->cardSum > cmp.cardSum) {
		return false;
	}
	std::priority_queue<std::string> tmp1 = this->cardSet;
	std::priority_queue<std::string> tmp2 = cmp.cardSet;
	for (int i = 0; i < this->cardSum; i++) {
		if (tmp1.top() < tmp2.top()) {
			return true;
		}
		else if (tmp1.top() > tmp2.top()) {
			return false;
		}
		tmp1.pop();
		tmp2.pop();
	}
	//SPDLOG_LOGGER_WARN(spdlog::get("main"),"find two identical cardsets.");
	return false;
}

void CardSet::showCardDetail() {
	std::cout << "\nthere are " << this->cardSum << "cards\n";
	CardSet tmp = *this;
	for (int i = 0; i < this->cardSum; i++) {
		std::cout << tmp.cardSet.top() << " ";
		tmp.cardSet.pop();
	}
	std::cout << "\n";
}