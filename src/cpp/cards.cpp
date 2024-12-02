#include <vector>
#include <map>
#include <queue>

#include "game/logic/cards.h"
#include "game/logic/register.h"
#include "spdlog/spdlog.h"

CardSet::CardSet(const std::vector<std::shared_ptr<Card>>& stack, std::string& vagueMatch) {
	int cardSum = stack.size();
	this->cardSum = cardSum;
	bool mark = false;
	for (int i = 0; i < cardSum; i++) {
		std::string nametmp = stack.at(i)->getName();
		std::string addtmp;
		if (nametmp[0] == '@') {
			if (mark) {
				SPDLOG_LOGGER_WARN(spdlog::get("main"),"can not have more than one vague match card");
			}
			vagueMatch.clear();
			for (int j = 0; j < nametmp.length(); j++) {
				if (nametmp[j] >= 'A' && nametmp[j] <= 'Z') {
					vagueMatch.push_back(nametmp[j]);
				}
				else if (nametmp[j] == '@') {
					continue;
				}
				else {
					addtmp.push_back(nametmp[j]);
				}
			}
			mark = true;
			addtmp = "@" + addtmp;
		}
		else {
			addtmp = nametmp;
		}
		this->cardSet.emplace(addtmp);
	}
	if (mark == false) {
		vagueMatch = '#';
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
	std::cout << "\nthere are " << this->cardSum << " cards\n";
	CardSet tmp = *this;
	for (int i = 0; i < this->cardSum; i++) {
		std::cout << tmp.cardSet.top() << " ";
		tmp.cardSet.pop();
	}
	std::cout << "\n";
}