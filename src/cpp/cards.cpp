#include <vector>
#include <map>
#include <queue>

#include "game/logic/cards.h"
#include "game/logic/register.h"

bool CardSet::check(CardSet a, CardSet b) {
	if(a.getCardSum() != b.getCardSum()) {
		return false;
	}
	std::priority_queue<std::string> qa = a.getCardset();
	std::priority_queue<std::string> qb = b.getCardset();

	//TODO 增强鲁棒性
	for(int i = 0; i < a.getCardSum(); i++) {
		std::string qas = qa.top();
		std::string qbs = qb.top();
		if(qas != qbs) {
			return false;
		}
		qa.pop();
		qb.pop();
	}
	return true;
}
