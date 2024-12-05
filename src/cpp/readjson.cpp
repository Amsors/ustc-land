#include <fstream>
#include <map>
#include <json/json.h>

#include <game/logic/mainlogic.h>
#include <game/logic/cards.h>
#include <game/logic/registry.h>
#include <game/logic/readjson.h>
#include "nanogui/screen.h"

#include "spdlog/spdlog.h"

static std::string tryReadString(Json::Value &v,std::string key) {
	if(v.isMember(key)) {
		return v[key].asString();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asString();
}

static int tryReadInt(Json::Value &v, std::string key) {
	if(v.isMember(key)) {
		return v[key].asInt();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asInt();
}

static bool tryReadBool(Json::Value &v, std::string key) {
	if(v.isMember(key)) {
		return v[key].asBool();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asBool();
}

static double tryReadDouble(Json::Value &v, std::string key) {
	if(v.isMember(key)) {
		return v[key].asDouble();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asDouble();
}

static float tryReadFloat(Json::Value& v, std::string key) {
	if (v.isMember(key)) {
		return v[key].asFloat();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asFloat();
}

static Json::Value tryReadArray(Json::Value &v, int i) {
	if(v.size() > i) {
		return v[i];
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "invalid index: {} for Value array", i);
	return v[i];
}

static Json::Value tryReadValue(Json::Value &v, std::string key) {
	if(v.isMember(key)) {
		return v[key];
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key];
}

void readAttributeJson() {
	std::ifstream inFile("rule/attribute.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int attributeValueSum = tryReadInt(root, "attributeValueSum");
	Json::Value jAttributeValue = tryReadValue(root, "attributeValue");
	if (jAttributeValue.size() != attributeValueSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for(int i = 0; i < attributeValueSum; i++) {
		Json::Value jSingleAttributeValue = tryReadArray(jAttributeValue, i);
		Attribute *newAttribute = new Attribute();

		newAttribute->isArray = 0;
		newAttribute->name = tryReadString(jSingleAttributeValue, "name");
		newAttribute->visibility = tryReadBool(jSingleAttributeValue, "visibility");
		newAttribute->max = tryReadDouble(jSingleAttributeValue, "max");
		newAttribute->attributeValue = tryReadDouble(jSingleAttributeValue, "initial");

		/*SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read attribute successfully. kind: {}, index: {}, name: {}",
			"Value", i, jSingleAttributeValue["name"].asString());*/

		reg.regAttribute.emplace(std::pair(newAttribute->name, newAttribute));
		reg.allAttribute.push_back(newAttribute->name);
	}

	int attributeArraySum = tryReadInt(root, "attributeArraySum");
	Json::Value jAttributeArray = tryReadValue(root, "attributeArray");
	if (jAttributeArray.size() != attributeArraySum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for(int i = 0; i < attributeArraySum; i++) {
		Json::Value jSingleAttributeArray = tryReadArray(jAttributeArray, i);

		Attribute* newAttribute = new Attribute();

		newAttribute->isArray = 1;
		newAttribute->name = tryReadString(jSingleAttributeArray, "name");
		newAttribute->visibility = tryReadBool(jSingleAttributeArray, "visibility");
		
		std::string match = tryReadString(jSingleAttributeArray, "match");
		newAttribute->attributeMatchKey = match;
		/*for (int j = 0; j < reg.regArrayElements[match].size(); j++) {
			newAttribute->attributeArray.emplace(
				std::pair<std::string, double>(reg.regArrayElements[match].at(j), 0.0)
				);
		}*/

		/*SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read attribute successfully. kind: {}, index: {}, name: {}",
			"Array", i, jSingleAttributeArray["name"].asString());*/

		reg.regAttribute.emplace(std::pair<std::string, Attribute*>(newAttribute->name, newAttribute));
		reg.allAttribute.push_back(newAttribute->name);
	}

	inFile.close();
	return;
}

void readAdvancementJson() {
	std::ifstream inFile("rule/advancement.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int advancementSum = tryReadInt(root, "advancementSum");
	reg.regValue["advancementSum"] = advancementSum;
	Json::Value jAdcancement = tryReadValue(root, "advancement");
	if (jAdcancement.size() != advancementSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for(int i = 0; i < advancementSum; i++) {
		Json::Value singleAdvancement = tryReadArray(jAdcancement, i);
		Advancement *newAdvancement = new Advancement();

		newAdvancement->name = tryReadString(singleAdvancement, "name");

		int formulaNeededSum = tryReadInt(singleAdvancement, "formulaNeededSum");
		Json::Value jFormulaNeeded = tryReadValue(singleAdvancement, "formulaNeeded");
		if (jFormulaNeeded.size() != formulaNeededSum) {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
		}
		for(int j = 0; j < formulaNeededSum; j++) {
			Json::Value jSingleFormulaNeeded = tryReadArray(jFormulaNeeded, j);
			std::string needName = tryReadString(jSingleFormulaNeeded, "name");
			newAdvancement->formulaNeeded.push_back(needName);
		}

		int cardNeededSum = tryReadInt(singleAdvancement, "cardNeededSum");
		Json::Value jCardNeeded = tryReadValue(singleAdvancement, "cardNeeded");
		if (jCardNeeded.size() != cardNeededSum) {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
		}
		for(int j = 0; j < cardNeededSum; j++) {
			Json::Value jSingleCardNeeded = tryReadArray(jCardNeeded, j);
			std::string needName = tryReadString(jSingleCardNeeded, "name");
			newAdvancement->cardNeeded.push_back(needName);
		}

		//int itemNeededSum = tryReadInt(singleAdvancement, "itemNeededSum");
		//Json::Value jItemNeeded = tryReadValue(singleAdvancement, "itemNeeded");
		//for(int j = 0; j < itemNeededSum; j++) {
		//	Json::Value jSingleItemNeeded = tryReadArray(jItemNeeded, j);
		//	std::string needName = tryReadString(jSingleItemNeeded, "name");
		//	newAdvancement->itemNeeded.push_back(needName);
		//}

		int attributeValueNeededSum = tryReadInt(singleAdvancement, "attributeValueNeededSum");
		Json::Value jAttributeValueNeeded = tryReadValue(singleAdvancement, "attributeValueNeeded");
		if (jAttributeValueNeeded.size() != attributeValueNeededSum) {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
		}
		for(int j = 0; j < attributeValueNeededSum; j++) {
			Json::Value singleAttribute = tryReadArray(jAttributeValueNeeded, j);
			double upper = tryReadDouble(singleAttribute, "upper");
			double lower = tryReadDouble(singleAttribute, "lower");

			std::string attributeName = tryReadString(singleAttribute, "attributeName");

			AttributeValueNeeded tmp;
			tmp.name = attributeName;
			tmp.res.lower = lower;
			tmp.res.upper = upper;
			newAdvancement->attributeValueNeeded.push_back(tmp);
		}

		int attributeArrayNeededSum = tryReadInt(singleAdvancement, "attributeArrayNeededSum");
		Json::Value jAttributeArrayNeeded = tryReadValue(singleAdvancement, "attributeArrayNeeded");
		if (jAttributeArrayNeeded.size() != attributeArrayNeededSum) {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
		}
		for(int j = 0; j < attributeArrayNeededSum; j++) {
			Json::Value jSingleAttribute = tryReadArray(jAttributeArrayNeeded, j);
			std::string attributeName = tryReadString(jSingleAttribute, "attributeName");
			std::string key = reg.regAttribute[attributeName]->getAttributeMatchKey();

			AttributeArrayNeeded tmp;
			tmp.name = attributeName;

			int restrictionSum = tryReadInt(jSingleAttribute, "restrictionSum");
			Json::Value jrestriction = tryReadValue(jSingleAttribute, "restriction");
			if (jrestriction.size() != restrictionSum) {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
			}
			for (int k = 0; k < restrictionSum; k++) {
				Json::Value singleRestriction = tryReadArray(jrestriction, k);
				double upper = tryReadDouble(singleRestriction, "upper");
				double lower = tryReadDouble(singleRestriction, "lower");
				std::string name = tryReadString(singleRestriction, "name");

				if (reg.regAttribute[attributeName]->getAttributeArray().contains(name) == false) {
					SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "no element: {} in ArrayAttribute: {}", name, attributeName);
					continue;
				}
				std::pair<std::string, Range> tmp2;
				tmp2.first = name;
				tmp2.second.lower = lower;
				tmp2.second.upper = upper;
				tmp.res.push_back(tmp2);
			}
		}

		reg.regAdvancement.emplace(std::pair(newAdvancement->name, newAdvancement));
		reg.advancementStatus.emplace(std::pair(newAdvancement->name, Advancement::AdvancementStatus::SHOWN_N));
	}
	inFile.close();
}

void readCardSetJson() {
	std::ifstream inFile("rule/cardset.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int cardSetSum = tryReadInt(root, "cardSetSum");
	reg.regValue.emplace(std::pair("cardSetSum", cardSetSum));

	Json::Value jCardSet = tryReadValue(root, "cardSet");
	if (jCardSet.size() != cardSetSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for (int i = 0; i < cardSetSum; i++) {
		
		Json::Value jSingleCardSet = tryReadArray(jCardSet, i);
		std::string cardSetName = tryReadString(jSingleCardSet, "cardSetName");
		int cardNum = tryReadInt(jSingleCardSet, "cardNum");
		Json::Value jCard = tryReadValue(jSingleCardSet, "card");
		int lostCardNum = tryReadInt(jSingleCardSet, "lostCardNum");
		Json::Value jLostCard = tryReadValue(jSingleCardSet, "lostCard");

		CardSet newCardSet;

		double timeNeeded = tryReadDouble(jSingleCardSet, "timeNeeded");
		reg.cardSetTimeNeeded[cardSetName] = timeNeeded;

		if (jCard.size() != cardNum) {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
		}
		for(int j = 0; j < cardNum; j++) {
			Json::Value oneCard = tryReadArray(jCard, j);
			std::string oneCardName = tryReadString(oneCard, "name");
			newCardSet.cardSet.emplace(oneCardName);
		}

		std::vector<std::string> tmp;
		if (jLostCard.size() != lostCardNum) {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
		}
		for (int j = 0; j < lostCardNum; j++) {
			Json::Value oneLostCard = tryReadArray(jLostCard, j);
			std::string oneLostCardName = tryReadString(oneLostCard, "name");
			tmp.emplace_back(oneLostCardName);
		}
		reg.cardSetLostCard.emplace(std::pair<std::string, std::vector<std::string>>(cardSetName, tmp));

		newCardSet.cardSum = cardNum;

		/*reg.cardSetPtr.emplace(std::pair(
			cardSetName, newCardSet));*/
		reg.cardSetAttained[cardSetName] = false;
		reg.regArrayElements["cardSet"].push_back(cardSetName);
		reg.cardSetMap.emplace(std::pair<CardSet, std::string>(newCardSet, cardSetName));
	}

	inFile.close();
}

void readRewardJson() {
	std::ifstream inFile("rule/reward.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int rewardSum = tryReadInt(root, "rewardSum");
	Json::Value jReward = tryReadValue(root, "reward");

	if (jReward.size() != rewardSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}

	for (int i = 0; i < rewardSum; i++) {
		Reward* newReward = new Reward;

		Json::Value jSingleReward = tryReadArray(jReward, i);

		std::string type = tryReadString(jSingleReward, "type");
		std::string name = tryReadString(jSingleReward, "name");

		bool isOnce = tryReadBool(jSingleReward, "once");
		reg.rewardIsOnce[name] = isOnce;
		reg.rewardAttained[name] = false;

		newReward->type = type;
		newReward->name = name;

		if (type == "attributeValue") {	
			std::string attributeName = tryReadString(jSingleReward, "attributeName");
			std::string change = tryReadString(jSingleReward, "change");
			double changeValue = tryReadDouble(jSingleReward, "changeValue");

			if (reg.regAttribute.contains(attributeName) == false) {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "no value attribute {}", attributeName);
			}
			if (change != "ratio_of_rest" && change != "add" && change != "mult") {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "invalid change type {}", change);
			}

			newReward->attributeName = attributeName;
			newReward->change = change;
			newReward->changeValue = changeValue;
		}
		else if (type == "attributeArray") {
			std::string attributeName = tryReadString(jSingleReward, "attributeName");
			std::string change = tryReadString(jSingleReward, "change");
			double changeValue = tryReadDouble(jSingleReward, "changeValue");
			bool isVague = tryReadBool(jSingleReward, "isVague");

			if (reg.regAttribute.contains(attributeName) == false) {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "no value attribute {}", attributeName);
			}
			if (change != "ratio_of_rest" && change != "add" && change != "mult") {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "invalid change type {}", change);
			}

			newReward->attributeName = attributeName;
			newReward->change = change;
			newReward->changeValue = changeValue;
			newReward->isVague = isVague;

			if (isVague == false) {
				std::string key = tryReadString(jSingleReward, "key");
				newReward->key = key;
			}
		}
		else if (type == "card") {
			std::string cardName = tryReadString(jSingleReward, "cardName");
			if (reg.cardAttained.contains(cardName) == false) {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "no card {}", cardName);
			}
			newReward->cardName = cardName;
		}
		else {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "no match type for {}",type);
		}

		reg.rewardPtr.emplace(std::pair<std::string, Reward*>(name, newReward));
	}

	inFile.close();
}

void readFormulaJson() {
	std::ifstream inFile("rule/formula.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int formulaSum = tryReadInt(root, "formulaSum");
	Json::Value jFormula = tryReadValue(root, "formula");
	//SPDLOG_LOGGER_ERROR(spdlog::get("readjson"), "want length {}",formulaSum);
	//SPDLOG_LOGGER_ERROR(spdlog::get("readjson"), "real length {}",jFormula.size());
	if (jFormula.size() != formulaSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for(int i = 0; i < formulaSum; i++) {
		Json::Value jSingleFormula = tryReadArray(jFormula, i);

		Formula* newFormula = new Formula;
		std::string tryCardSet = tryReadString(jSingleFormula, "cardSetName");
		std::string  tryFormulaName = tryReadString(jSingleFormula, "formulaName");
		int tryRewardSum = tryReadInt(jSingleFormula, "rewardSum");

		std::string tryCardSetVagueMatch = tryReadString(jSingleFormula, "cardSetVagueMatch");
		std::string tryRewardVagueMatch = tryReadString(jSingleFormula, "rewardVagueMatch");
		newFormula->cardSetVagueMatch = tryCardSetVagueMatch;
		newFormula->rewardVagueMatch = tryRewardVagueMatch;

		newFormula->formulaName = tryFormulaName;
		newFormula->cardSetName = tryFormulaName;

		Json::Value jReward = tryReadValue(jSingleFormula, "reward");
		if (jReward.size() != tryRewardSum) {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
		}
		for (int j = 0; j < tryRewardSum; j++) {
			Json::Value jSingleReward = tryReadArray(jReward, j);
			std::string tryRewardName = tryReadString(jSingleReward, "name");

			if (reg.rewardPtr.contains(tryRewardName) == false) {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "no reward {} when trying to add formula {}", tryRewardName, tryFormulaName);
			}
			newFormula->rewardName.push_back(tryRewardName);
			
			int trySet = tryReadInt(jSingleReward, "set");
			newFormula->rewardSet[tryRewardName] = trySet;
			double tryPossibility = tryReadDouble(jSingleReward, "possibility");
			newFormula->rewardPossibility[tryRewardName] = tryPossibility;
		}
		
		reg.cardSetToFormula[tryCardSet].push_back(tryFormulaName);
		reg.formulaAttained.emplace(std::pair<std::string, bool>(tryFormulaName, false));
		reg.formulaPtr.emplace(std::pair<std::string, Formula*>(tryFormulaName, newFormula));

	}

	inFile.close();
}

void readSpotJSon() {
	std::ifstream inFile("rule/spot.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int spotSum = tryReadInt(root, "spotSum");
	reg.regValue.emplace(std::pair<std::string, int>("spotSum", spotSum));
	reg.allCardType.push_back("spot");
	Json::Value jSpot = tryReadValue(root, "spot");
	if (jSpot.size() != spotSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for (int i = 0; i < spotSum; i++) {
		Json::Value jSingleSpot = tryReadArray(jSpot, i);
		std::string onename = tryReadString(jSingleSpot, "name");

		reg.regArrayElements["spot"].push_back(onename);
		reg.cardAttained.emplace(std::pair<std::string, bool>(onename, false));
		reg.allCard["spot"].insert(onename);
	}
	
	inFile.close();
}

void readClassJson() {
	std::ifstream inFile("rule/class.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int classSum = tryReadInt(root, "classSum");
	reg.regValue.emplace(std::pair<std::string, int>("classSum", classSum));
	Json::Value jClass = tryReadValue(root, "class");
	if (jClass.size() != classSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for(int i = 0; i < classSum; i++) {
		Json::Value jSingleClass = tryReadArray(jClass, i);
		std::string onename = tryReadString(jSingleClass, "name");
		reg.regArrayElements["class"].push_back(onename);
	}

	inFile.close();
}

void readItemJson() {
	std::ifstream inFile("rule/item.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	reg.allCardType.push_back("item");
	int itemSum = tryReadInt(root, "itemSum");
	reg.regValue.emplace(std::pair<std::string, int>("itemSum", itemSum));
	Json::Value jItem = tryReadValue(root, "item");
	if (jItem.size() != itemSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for (int i = 0; i < itemSum; i++) {
		Json::Value jSingleItem = tryReadArray(jItem, i);
		std::string onename = tryReadString(jSingleItem, "name");

		reg.regArrayElements["item"].push_back(onename);
		reg.cardAttained.emplace(std::pair<std::string, bool>(onename, false));
		reg.allCard["item"].insert(onename);
	}

	inFile.close();
}

void readRoleJson() {
	std::ifstream inFile("rule/role.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	reg.allCardType.push_back("role");

	int roleSum = tryReadInt(root, "roleSum");
	reg.regValue.emplace(std::pair<std::string, int>("roleSum", roleSum));

	Json::Value jRole = tryReadValue(root, "role");
	if (jRole.size() != roleSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for (int i = 0; i < roleSum; i++) {
		Json::Value jSingleRole = tryReadArray(jRole, i);
		std::string onename = tryReadString(jSingleRole, "name");

		reg.regArrayElements["role"].push_back(onename);
		reg.cardAttained.emplace(std::pair<std::string, bool>(onename, false));
		reg.allCard["role"].insert(onename);
	}

	inFile.close();
}

void readCardTypeJson() {
	std::ifstream inFile("rule/cardtype.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	
	int cardTypeSum = tryReadInt(root, "cardTypeSum");
	reg.regValue.emplace(std::pair<std::string, int>("cardTypeSum", cardTypeSum));
	Json::Value jCardType = tryReadValue(root, "cardType");
	if (jCardType.size() != cardTypeSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for (int i = 0; i < cardTypeSum; i++) {
		Json::Value jSingleCardType = tryReadArray(jCardType, i);
		std::string onename = tryReadString(jSingleCardType, "name");
		float oneR = tryReadFloat(jSingleCardType, "R");
		float oneG = tryReadFloat(jSingleCardType, "G");
		float oneB = tryReadFloat(jSingleCardType, "B");
		float oneA = tryReadFloat(jSingleCardType, "A");

		if (std::find(reg.allCardType.begin(), reg.allCardType.end(), onename) == reg.allCardType.end()) {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "cardtype: {} not identified", onename);
			continue;
		}

		nanogui::Color color = nanogui::Color(oneR, oneG, oneB, oneA);
		reg.cardTypeColor.emplace(std::pair<std::string, nanogui::Color>(onename, color));

	}

	inFile.close();
}

void readJson() {
	readRuleFileJson();
	sort(reg.regJsonFile.begin(), reg.regJsonFile.end(),
		[](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
			return a.second > b.second;
		});
	for(auto &[file, importance]: reg.regJsonFile) {
		if(file == "attribute.json") {
			readAttributeJson();
		}
		else if (file == "advancement.json") {
			readAdvancementJson();
		}
		else if (file == "cardset.json") {
			readCardSetJson();
		}
		else if (file == "spot.json") {
			readSpotJSon();
		}
		else if (file == "formula.json") {
			readFormulaJson();
		}
		else if (file == "class.json") {
			readClassJson();
		}
		else if (file == "item.json") {
			readItemJson();
		}
		else if (file == "reward.json") {
			readRewardJson();
		}
		else if (file == "role.json") {
			readRoleJson();
		}
		else if (file == "cardtype.json") {
			readCardTypeJson();
		}
		else{
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "no match for {}", file);
		}
		SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read json file {} (importance {} )", file, importance);
	}
}

void readRuleFileJson() {
	std::ifstream inFile("rule/rulefile.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int jsonFileSum = tryReadInt(root, "jsonFileSum");
	Json::Value jJsonFile = tryReadValue(root, "jsonFile");
	if (jJsonFile.size() != jsonFileSum) {
		SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "array length not match");
	}
	for(int i = 0; i < jsonFileSum; i++) {
		Json::Value jSingleJsonFile = tryReadArray(jJsonFile, i);
		std::string oneName = tryReadString(jSingleJsonFile, "name");
		int oneImportance = tryReadInt(jSingleJsonFile, "importance");
		reg.regJsonFile.emplace_back(oneName, oneImportance);
	}

	inFile.close();
}
