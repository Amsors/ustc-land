#include <fstream>
#include <map>
#include <json/json.h>

#include <game/logic/mainlogic.h>
#include <game/logic/cards.h>
#include <game/logic/register.h>
#include <game/logic/readjson.h>

#include "spdlog/spdlog.h"

std::string tryReadString(Json::Value &v, std::string key) {
	if(v.isMember(key)) {
		return v[key].asString();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asString();
}

int tryReadInt(Json::Value &v, std::string key) {
	if(v.isMember(key)) {
		return v[key].asInt();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asInt();
}

bool tryReadBool(Json::Value &v, std::string key) {
	if(v.isMember(key)) {
		return v[key].asBool();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asBool();
}

double tryReadDouble(Json::Value &v, std::string key) {
	if(v.isMember(key)) {
		return v[key].asDouble();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asDouble();
}

Json::Value tryReadArray(Json::Value &v, int i) {
	if(v.size() > i) {
		return v[i];
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "invalid index: {} for Value array", i);
	return v[i];
}

Json::Value tryReadValue(Json::Value &v, std::string key) {
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
	for(int i = 0; i < attributeValueSum; i++) {
		Json::Value jSingleAttributeValue = tryReadArray(jAttributeValue, i);
		Attribute *newAttribute = new Attribute();

		newAttribute->isArray = 0;
		newAttribute->name = tryReadString(jSingleAttributeValue, "name");
		newAttribute->visibility = tryReadBool(jSingleAttributeValue, "visibility");
		newAttribute->max = tryReadDouble(jSingleAttributeValue, "max");
		newAttribute->initial = tryReadDouble(jSingleAttributeValue, "initial");

		SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read attribute successfully. kind: {}, index: {}, name: {}",
			"Value", i, jSingleAttributeValue["name"].asString());
		//newAttribute->showAttribute();

		reg.regAttribute.emplace(std::pair(newAttribute->name, newAttribute));
	}

	int attributeArraySum = tryReadInt(root, "attributeArraySum");
	Json::Value jAttributeArray = tryReadValue(root, "attributeArray");
	for(int i = 0; i < attributeArraySum; i++) {
		Json::Value jSingleAttributeArray = tryReadArray(jAttributeArray, i);

		std::string arrayLengthStrategy = tryReadString(jSingleAttributeArray, "arrayLengthStrategy");
		if(arrayLengthStrategy == "match") {
			std::string keyTemp = tryReadString(jSingleAttributeArray, "arrayLengthMatchKey");
			int length = 0;
			if(reg.regValue.find(keyTemp) == reg.regValue.end()) {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found in reg.regValue", keyTemp);
			} else {
				length = reg.regValue[keyTemp];
			}

			Attribute *newAttribute = new Attribute();

			newAttribute->isArray = true;
			newAttribute->name = tryReadString(jSingleAttributeArray, "name");
			newAttribute->visibility = tryReadBool(jSingleAttributeArray, "visibility");

			for(int j = 0; j < length; j++) {
				newAttribute->attributeArray.push_back(0);
			}
			newAttribute->attributeArrayMatch = tryReadString(jSingleAttributeArray, "arrayLengthMatchKey");

			SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read attribute successfully. kind: {}, index: {}, name: {}",
				"Array", i, jSingleAttributeArray["name"].asString());
			//newAttribute->showAttribute();

			reg.regAttribute.emplace(std::pair(newAttribute->name, newAttribute));
		}
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
	Json::Value jAdcancement = tryReadValue(root, "advancement");
	for(int i = 0; i < advancementSum; i++) {
		Json::Value singleAdvancement = tryReadArray(jAdcancement, i);
		Advancement *newAdvancement = new Advancement();

		newAdvancement->name = tryReadString(singleAdvancement, "name");

		int formulaNeededSum = tryReadInt(singleAdvancement, "formulaNeededSum");
		Json::Value jFormulaNeeded = tryReadValue(singleAdvancement, "formulaNeeded");
		for(int j = 0; j < formulaNeededSum; j++) {
			Json::Value jSingleFormulaNeeded = tryReadArray(jFormulaNeeded, j);
			std::string needName = tryReadString(jSingleFormulaNeeded, "name");
			newAdvancement->formulaNeeded.push_back(needName);
		}

		int cardNeededSum = tryReadInt(singleAdvancement, "cardNeededSum");
		Json::Value jCardNeeded = tryReadValue(singleAdvancement, "cardNeeded");
		for(int j = 0; j < cardNeededSum; j++) {
			Json::Value jSingleCardNeeded = tryReadArray(jCardNeeded, j);
			std::string needName = tryReadString(jSingleCardNeeded, "name");
			newAdvancement->cardNeeded.push_back(needName);
		}

		int itemNeededSum = tryReadInt(singleAdvancement, "itemNeededSum");
		Json::Value jItemNeeded = tryReadValue(singleAdvancement, "itemNeeded");
		for(int j = 0; j < itemNeededSum; j++) {
			Json::Value jSingleItemNeeded = tryReadArray(jItemNeeded, j);
			std::string needName = tryReadString(jSingleItemNeeded, "name");
			newAdvancement->itemNeeded.push_back(needName);
		}

		int attributeValueNeededSum = tryReadInt(singleAdvancement, "attributeValueNeededSum");
		Json::Value jAttributeValueNeeded = tryReadValue(singleAdvancement, "attributeValueNeeded");
		for(int j = 0; j < attributeValueNeededSum; j++) {
			Json::Value singleAttribute = tryReadArray(jAttributeValueNeeded, j);
			double upper = tryReadDouble(singleAttribute, "upper");
			double lower = tryReadDouble(singleAttribute, "lower");
			std::pair<double, double> rangetmp(upper, lower);
			std::string stmp = tryReadString(singleAttribute, "attributeName");
			newAdvancement->attributeValueNeeded.emplace(std::pair(
				stmp, rangetmp));
		}

		int attributeArrayNeededSum = tryReadInt(singleAdvancement, "attributeArrayNeededSum");
		Json::Value jAttributeArrayNeeded = tryReadValue(singleAdvancement, "attributeArrayNeeded");
		for(int j = 0; j < attributeArrayNeededSum; j++) {
			Json::Value jSingleAttribute = tryReadArray(jAttributeArrayNeeded, j);
			std::string stmp = tryReadString(jSingleAttribute, "attributeName");

			if(reg.regAttribute.find(stmp) == reg.regAttribute.end()) {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found in reg.regAttribute", stmp);
			}
			int restrictionSum = tryReadInt(jSingleAttribute, "restrictionSum");
			Json::Value jrestriction = tryReadValue(jSingleAttribute, "restriction");
			for(int k = 0; k < restrictionSum; k++) {
				Json::Value singlerestriction = tryReadArray(jrestriction, k);
				double tryupper = tryReadDouble(singlerestriction, "upper");
				double trylower = tryReadDouble(singlerestriction, "lower");
				std::pair rangetmp(tryupper, trylower);
				int tryindex = tryReadInt(singlerestriction, "index");
				std::pair detailtmp(tryindex, rangetmp);
				newAdvancement->attributeArrayNeeded.emplace(std::pair(
					stmp, detailtmp
				));
			}
		}

		reg.regAdvancement.emplace(std::pair(newAdvancement->name, newAdvancement));
	}
	inFile.close();
}

void readCardSetJson() {
	std::ifstream inFile("rule/cardset.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int cardSetSum = tryReadInt(root, "cardSetSum");
	Json::Value jcardSet = tryReadValue(root, "cardSet");
	for(int i = 0; i < cardSetSum; i++) {
		Json::Value jSingleCardSet = tryReadArray(jcardSet, i);
		std::string cardSetName = tryReadString(jSingleCardSet, "cardSetName");
		Json::Value jCard = tryReadValue(jSingleCardSet, "card");
		int cardNum = tryReadInt(jSingleCardSet, "cardNum");

		CardSet *newCardSet = new CardSet;

		for(int j = 0; j < cardNum; j++) {
			Json::Value oneCard = tryReadArray(jCard, j);
			std::string oneCardName = tryReadString(oneCard, "name");
			newCardSet->cardSet.emplace(oneCardName);
		}
		reg.cardSetPtr.emplace(std::pair(
			cardSetName, newCardSet));
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
	for(int i = 0; i < formulaSum; i++) {
		Json::Value jSingleFormula = tryReadArray(jFormula, i);

		Formula *newFormula = new Formula;
		std::string formulaName = tryReadString(jSingleFormula, "formulaName");

		reg.formulaAttained.emplace(std::pair(formulaName, false));
		reg.formulaPtr.emplace(std::pair(
			jSingleFormula["formulaName"].asString(), newFormula));
		//TODO reward部分
	}

	inFile.close();
}

void readSpotJSon() {
	std::ifstream inFile("rule/spot.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

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
	for(int i = 0; i < classSum; i++) {
		Json::Value jSingleClass = tryReadArray(jClass, i);
		std::string onename = tryReadString(jSingleClass, "name");
		int oneindex = tryReadInt(jSingleClass, "index");
		reg.classNameToIndex.emplace(std::pair<std::string, int>(
			onename, oneindex));
	}

	inFile.close();
}

void readItemJson() {
	std::ifstream inFile("rule/item.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

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
		} else if(file == "advancement.json") {
			readAdvancementJson();
		} else if(file == "cardset.json") {
			readCardSetJson();
		} else if(file == "spot.json") {
			readSpotJSon();
		} else if(file == "formula.json") {
			readFormulaJson();
		} else if(file == "class.json") {
			readClassJson();
		} else if(file == "item.json") {
			readItemJson();
		} else {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "no match for {}", file);
		}
		SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read json file {} (importance {} ) successfully", file, importance);
	}
}

void readRuleFileJson() {
	std::ifstream inFile("rule/rulefile.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int jsonFileSum = tryReadInt(root, "jsonFileSum");
	Json::Value jJsonFile = tryReadValue(root, "jsonFile");
	for(int i = 0; i < jsonFileSum; i++) {
		Json::Value jSingleJsonFile = tryReadArray(jJsonFile, i);
		std::string oneName = tryReadString(jSingleJsonFile, "name");
		int oneImportance = tryReadInt(jSingleJsonFile, "importance");
		reg.regJsonFile.emplace_back(oneName, oneImportance);
	}

	inFile.close();
}
