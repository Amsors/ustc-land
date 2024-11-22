#include <vector>
#include <map>
#include <queue>

#include "game/logic/cards.h"
#include "game/logic/register.h"


bool CardSet::check(CardSet a, CardSet b) {
	if (a.getCardSum() != b.getCardSum()) {
		return false;
	}
	std::priority_queue<std::string> qa = a.getCardset();
	std::priority_queue<std::string> qb = b.getCardset();

	//TODO ÔöÇ¿Â³°ôÐÔ
	for (int i = 0; i < a.getCardSum(); i++) {
		std::string qas = qa.top();
		std::string qbs = qb.top();
		if (qas!=qbs) {
			return false;
		}
		qa.pop();
		qb.pop();
	}
	return true;
}

SpotCard::SpotCard() {
	reg.spotCardPtr[this->name].push_back(this);
	reg.cardAttained[this->name] = true;
}
ItemCard::ItemCard() {
	reg.ItemCardPtr[this->name].push_back(this);
}
RoleCard::RoleCard() {
	reg.roleCardPtr[this->name].push_back(this);
}