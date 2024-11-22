#include <fstream>
#include <map>
#include <json/json.h>

#include <game/logic/mainlogic.h>
#include <game/logic/cards.h>
#include <game/logic/register.h>

#include "spdlog/spdlog.h"

std::string tryReadString(Json::Value &v,std::string key) {
	if (v.isMember(key)) {
		return v[key].asString();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asString();
}

int tryReadInt(Json::Value &v, std::string key) {
	if (v.isMember(key)) {
		return v[key].asInt();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asInt();
}

bool tryReadBool(Json::Value &v, std::string key) {
	if (v.isMember(key)) {
		return v[key].asBool();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asBool();
}

double tryReadDouble(Json::Value &v, std::string key) {
	if (v.isMember(key)) {
		return v[key].asDouble();
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
	return v[key].asDouble();
}

void readAttributeJson() {
	std::ifstream inFile("rule/attribute.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int attributeValueSum = root["attributeValueSum"].asInt();
	Json::Value jAttributeValue = root["attributeValue"];
	for (int i = 0; i < attributeValueSum; i++) {
		Json::Value jSingleAttributeValue = jAttributeValue[i];
		Attribute* newAttribute = new Attribute();

		newAttribute->isArray = 0;
		newAttribute->name = jSingleAttributeValue["name"].asString();
		newAttribute->visibility = jSingleAttributeValue["visibility"].asBool();
		newAttribute->max = jSingleAttributeValue["max"].asDouble();
		newAttribute->initial = jSingleAttributeValue["initial"].asDouble();

		SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read attribute. kind: {}, index: {}, name: {}",
			"Value", i, jSingleAttributeValue["name"].asString());
		//newAttribute->showAttribute();

		reg.regAttribute.emplace(std::pair<std::string, Attribute*>(newAttribute->name, newAttribute));
	}

	int attributeArraySum = root["attributeArraySum"].asInt();
	Json::Value jAttributeArray = root["attributeArray"];
	for (int i = 0; i < attributeArraySum; i++) {
		Json::Value jSingleAttributeArray = jAttributeArray[i];
		if (jSingleAttributeArray["arrayLengthStrategy"].asString() == "match") {
			std::string keyTemp(jSingleAttributeArray["arrayLengthMatchKey"].asString());
			int length = reg.regValue[keyTemp];
			Attribute* newAttribute = new Attribute();

			newAttribute->isArray = 1;
			newAttribute->name = jSingleAttributeArray["name"].asString();
			newAttribute->visibility = jSingleAttributeArray["visibility"].asBool();

			for (int j = 0; j < length; j++) {
				newAttribute->attributeArray.push_back(0);
			}
			newAttribute->attributeArrayMatch = jSingleAttributeArray["arrayLengthMatchKey"].asString();

			SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read attribute. kind: {}, index: {}, name: {}",
				"Array", i, jSingleAttributeArray["name"].asString());
			//newAttribute->showAttribute();
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
	int advancementSum = root["advancementSum"].asInt();
	Json::Value j = root["advancement"];
	for (int i = 0; i < advancementSum; i++) {
		Json::Value singleAdvancement = j[i];
		Advancement* newAdvancement = new Advancement();

		newAdvancement->name = singleAdvancement["name"].asString();

		int formulaNeededSum = singleAdvancement["formulaNeededSum"].asInt();
		Json::Value jFormulaNeeded = singleAdvancement["formulaNeeded"];
		for (int j = 0; j < formulaNeededSum; j++) {
			newAdvancement->formulaNeeded.push_back(jFormulaNeeded[j].asString());
		}

		int cardNeededSum = singleAdvancement["cardNeededSum"].asInt();
		Json::Value jCardNeeded = singleAdvancement["cardNeeded"];
		for (int j = 0; j < cardNeededSum; j++) {
			newAdvancement->cardNeeded.push_back(jCardNeeded[j].asString());
		}

		int itemNeededSum = singleAdvancement["itemNeededSum"].asInt();
		Json::Value jItemNeeded = singleAdvancement["itemNeeded"];
		for (int j = 0; j < itemNeededSum; j++) {
			newAdvancement->itemNeeded.push_back(jItemNeeded[j].asString());
		}

		int attributeValueNeededSum = singleAdvancement["attributeValueNeededSum"].asInt();
		Json::Value jAttributeValueNeeded = singleAdvancement["attributeValueNeeded"];
		for (int j = 0; j < attributeValueNeededSum; j++) {
			Json::Value singleAttribute = jAttributeValueNeeded[j];
			std::pair<double, double> rangetmp(singleAttribute["upper"].asDouble(), singleAttribute["lower"].asDouble());
			std::string stmp = singleAttribute["attributeName"].asString();
			newAdvancement->attributeValueNeeded.emplace(std::pair<std::string, std::pair<double, double>>(
				stmp, rangetmp));
		}

		int attributeArrayNeededSum = singleAdvancement["attributeArrayNeededSum"].asInt();
		Json::Value jAttributeArrayNeeded = singleAdvancement["attributeArrayNeeded"];
		for (int j = 0; j < attributeArrayNeededSum; j++) {
			Json::Value jSingleAttribute = jAttributeArrayNeeded[j];
			std::string stmp = jSingleAttribute["attributeName"].asString();
			std::string keytmp = reg.regAttribute[stmp]->attributeArrayMatch;
			int restrictionSum = jSingleAttribute["restrictionSum"].asInt();
			Json::Value jrestriction = jSingleAttribute["restriction"];
			for (int k = 0; k < restrictionSum; k++) {
				Json::Value singlerestriction = jrestriction[k];
				std::pair<double, double> rangetmp(singlerestriction["upper"].asDouble(), singlerestriction["lower"].asDouble());
				std::pair<int, std::pair<double, double>> detailtmp(singlerestriction["index"].asInt(), rangetmp);
				newAdvancement->attributeArrayNeeded.emplace(std::pair<std::string, std::pair<int, std::pair<double, double>>>(
					stmp, detailtmp
				));
			}
		}
		
		reg.regAdvancement.emplace(std::pair<std::string, Advancement*>(newAdvancement->name, newAdvancement));
	}
	inFile.close();
	return;
}

void readCardSetJson() {
	std::ifstream inFile("rule/cardset.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int cardSetSum = root["cardSetSum"].asInt();
	Json::Value jcardSet = root["cardSet"];
	for (int i = 0; i < cardSetSum; i++) {
		Json::Value jSingleCardSet = jcardSet[i];
		Json::Value jCard = jSingleCardSet["card"];

		CardSet* newCardSet = new CardSet;
		for (int i = 0; i < root["cardNum"].asInt(); i++) {
			//cs.getCardset().push(jCS[i].asString());
			newCardSet->cardSet.emplace(jCard[i].asString());
		}
		reg.cardSetPtr.emplace(std::pair<std::string, CardSet*>(
			jSingleCardSet["cardSetName"].asString(), newCardSet));

	}

	inFile.close();
	return;
}

void readFormulaJson() {
	std::ifstream inFile("rule/formula.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int formulaSum = root["formulaSum"].asInt();
	Json::Value jFormula = root["formula"];
	for (int i = 0; i < formulaSum; i++) {
		Json::Value jSingleFormula = jFormula[i];

		Formula* newFormula = new Formula;
		reg.formulaAttained[jSingleFormula["formulaName"].asString()] = false;
		reg.formulaPtr.emplace(std::pair<std::string, Formula*>(
			jSingleFormula["formulaName"].asString(), newFormula));
		//TODO reward²¿·Ö

	}

	inFile.close();
	return;
}

void readSpotJSon() {
	std::ifstream inFile("rule/spot.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	
	inFile.close();
}

void readClassJson() {
	std::ifstream inFile("rule/spot.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	reg.regValue.emplace(std::pair<std::string, int>("classSum", root["classSum"].asInt()));
	Json::Value jClass = root["class"];
	for (int i = 0; i < root["classSum"].asInt(); i++) {
		Json::Value jSingleClass = jClass["class"];
		reg.classNameToIndex.emplace(std::pair<std::string, int>(
			jSingleClass["name"].asString(), jSingleClass["index"].asInt()));
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