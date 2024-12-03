#include <map>
#include <fstream>

#include "game/logic/basic.h"
#include "json/json.h"
#include "game/logic/mainlogic.h"
#include "game/logic/readjson.h"
#include "game/logic/registry.h"
#include "spdlog/spdlog.h"

Advancement::Advancement() {
	this->established = 0;
}

bool Advancement::checkAdvancement() {
	for (std::string name : this->formulaNeeded) {
		if (reg.formulaAttained.contains(name) == false) {
			SPDLOG_LOGGER_WARN(spdlog::get("main"), "no formula: {} when trying to check advancement", name);
		}
		if (reg.formulaAttained[name] == false) {
			return false;
		}
	}
	for (std::string name : this->cardNeeded) {
		if (reg.cardAttained.contains(name) == false) {
			SPDLOG_LOGGER_WARN(spdlog::get("main"), "no card: {} when trying to check advancement", name);
		}
		if (reg.cardAttained[name] == false) {
			return false;
		}
	}
	for (const auto &detail : this->attributeValueNeeded) {
		std::string name = detail.name;
		double upper = detail.res.upper;
		double lower = detail.res.lower;
		if (reg.regAttribute.contains(name) == false) {
			SPDLOG_LOGGER_WARN(spdlog::get("main"), "no attribute: {} when trying to check advancement", name);
		}
		if (reg.regAttribute[name]->getAttributeValue() >= upper
			|| reg.regAttribute[name]->getAttributeValue() <= lower) {
			return false;
		}
	}
	for (const auto& detail : this->attributeArrayNeeded) {
		//每个detail对应一种attribute
		std::string name = detail.name;
		if (reg.regAttribute.contains(name) == false) {
			SPDLOG_LOGGER_WARN(spdlog::get("main"), "no attribute: {} when trying to check advancement", name);
		}
		for (int i = 0; i < detail.res.size();i++) {
			double needUpper = detail.res.at(i).second.upper;
			double needLower = detail.res.at(i).second.lower;
			std::string needName = detail.res.at(i).first;

			double realValue = reg.regAttribute[name]->getAttributeArray()[needName];
			if (realValue > needUpper || realValue < needLower) {
				return false;
			}
		}
	}
	return true;
}

//AdvancementMgr advMgr;