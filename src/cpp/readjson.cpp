#include <fstream>
#include <map>
#include <json/json.h>

#include <game/logic/mainlogic.h>
#include <game/logic/cards.h>
#include <game/logic/register.h>
#include <game/logic/readjson.h>

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

Json::Value tryReadArray(Json::Value &v, int i) {
	if (v.size()>i) {
		return v[i];
	}
	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "invalid index: {} for Value array", i);
	return v[i];
}

Json::Value tryReadValue(Json::Value &v, std::string key) {
	if (v.isMember(key)) {
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

	int attributeValueSum = tryReadInt(root,"attributeValueSum");
	Json::Value jAttributeValue = tryReadValue(root, "attributeValue");
	for (int i = 0; i < attributeValueSum; i++) {
		Json::Value jSingleAttributeValue = tryReadArray(jAttributeValue, i);//jAttributeValue[i];
		Attribute* newAttribute = new Attribute();

		newAttribute->isArray = 0;
		newAttribute->name = tryReadString(jSingleAttributeValue, "name");//jSingleAttributeValue["name"].asString();
		newAttribute->visibility = tryReadBool(jSingleAttributeValue, "visibility");//jSingleAttributeValue["visibility"].asBool();
		newAttribute->max = tryReadDouble(jSingleAttributeValue, "max");//jSingleAttributeValue["max"].asDouble();
		newAttribute->initial = tryReadDouble(jSingleAttributeValue, "initial");//jSingleAttributeValue["initial"].asDouble();

		SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read attribute successfully. kind: {}, index: {}, name: {}",
			"Value", i, jSingleAttributeValue["name"].asString());
		//newAttribute->showAttribute();

		reg.regAttribute.emplace(std::pair<std::string, Attribute*>(newAttribute->name, newAttribute));
	}

	int attributeArraySum = tryReadInt(root, "attributeArraySum");//root["attributeArraySum"].asInt();
	Json::Value jAttributeArray = tryReadValue(root, "attributeArray");//root["attributeArray"];
	for (int i = 0; i < attributeArraySum; i++) {
		Json::Value jSingleAttributeArray = tryReadArray(jAttributeArray, i); //jAttributeArray[i];

		std::string arrayLengthStrategy = tryReadString(jSingleAttributeArray, "arrayLengthStrategy");
		if (arrayLengthStrategy == "match") {
			std::string keyTemp = tryReadString(jSingleAttributeArray, "arrayLengthMatchKey");//jSingleAttributeArray["arrayLengthMatchKey"].asString();
			int length = 0;
			if (reg.regValue.find(keyTemp) == reg.regValue.end()) {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found in reg.regValue", keyTemp);
			}
			else {
				length = reg.regValue[keyTemp];
			}

			Attribute* newAttribute = new Attribute();

			newAttribute->isArray = 1;
			newAttribute->name = tryReadString(jSingleAttributeArray, "name");//jSingleAttributeArray["name"].asString();
			newAttribute->visibility = tryReadBool(jSingleAttributeArray, "visibility");//jSingleAttributeArray["visibility"].asBool();

			for (int j = 0; j < length; j++) {
				newAttribute->attributeArray.push_back(0);
			}
			newAttribute->attributeArrayMatch = tryReadString(jSingleAttributeArray, "arrayLengthMatchKey"); //jSingleAttributeArray["arrayLengthMatchKey"].asString();

			SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read attribute successfully. kind: {}, index: {}, name: {}",
				"Array", i, jSingleAttributeArray["name"].asString());
			//newAttribute->showAttribute();

			reg.regAttribute.emplace(std::pair<std::string, Attribute*>(newAttribute->name, newAttribute));
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

	int advancementSum = tryReadInt(root, "advancementSum");//root["advancementSum"].asInt();
	Json::Value j = tryReadValue(root, "advancement");//root["advancement"];
	for (int i = 0; i < advancementSum; i++) {
		Json::Value singleAdvancement = tryReadArray(j, i);//j[i];
		Advancement* newAdvancement = new Advancement();

		newAdvancement->name = tryReadString(singleAdvancement, "name");//singleAdvancement["name"].asString();

		int formulaNeededSum = tryReadInt(singleAdvancement, "formulaNeededSum");//singleAdvancement["formulaNeededSum"].asInt();
		Json::Value jFormulaNeeded = tryReadValue(singleAdvancement, "formulaNeeded");//singleAdvancement["formulaNeeded"];
		for (int j = 0; j < formulaNeededSum; j++) {
			Json::Value jSingleFormulaNeeded = tryReadArray(jFormulaNeeded, j);
			std::string needName = tryReadString(jSingleFormulaNeeded, "name");
			newAdvancement->formulaNeeded.push_back(needName/*jFormulaNeeded[j].asString()*/);
		}

		int cardNeededSum = tryReadInt(singleAdvancement, "cardNeededSum");//singleAdvancement["cardNeededSum"].asInt();
		Json::Value jCardNeeded = tryReadValue(singleAdvancement, "cardNeeded");//singleAdvancement["cardNeeded"];
		for (int j = 0; j < cardNeededSum; j++) {
			Json::Value jSingleCardNeeded = tryReadArray(jCardNeeded, j);
			std::string needName = tryReadString(jSingleCardNeeded, "name");
			newAdvancement->cardNeeded.push_back(needName/*jCardNeeded[j].asString()*/);
		}

		int itemNeededSum = tryReadInt(singleAdvancement, "itemNeededSum");//singleAdvancement["itemNeededSum"].asInt();
		Json::Value jItemNeeded = tryReadValue(singleAdvancement, "itemNeeded");//singleAdvancement["itemNeeded"];
		for (int j = 0; j < itemNeededSum; j++) {
			Json::Value jSingleItemNeeded = tryReadArray(jItemNeeded, j);
			std::string needName = tryReadString(jSingleItemNeeded, "name");
			newAdvancement->itemNeeded.push_back(needName/*jItemNeeded[j].asString()*/);
		}

		int attributeValueNeededSum = tryReadInt(singleAdvancement, "attributeValueNeededSum");//singleAdvancement["attributeValueNeededSum"].asInt();
		Json::Value jAttributeValueNeeded = tryReadValue(singleAdvancement, "attributeValueNeeded");//singleAdvancement["attributeValueNeeded"];
		for (int j = 0; j < attributeValueNeededSum; j++) {
			Json::Value singleAttribute = tryReadArray(jAttributeValueNeeded, j);//jAttributeValueNeeded[j];
			double tryupper = tryReadDouble(singleAttribute, "upper");
			double trylower = tryReadDouble(singleAttribute, "lower");
			std::pair<double, double> rangetmp(tryupper,trylower/*singleAttribute["upper"].asDouble(), singleAttribute["lower"].asDouble()*/);
			std::string stmp = tryReadString(singleAttribute, "attributeName");//singleAttribute["attributeName"].asString();
			newAdvancement->attributeValueNeeded.emplace(std::pair<std::string, std::pair<double, double>>(
				stmp, rangetmp));
		}

		int attributeArrayNeededSum = tryReadInt(singleAdvancement, "attributeArrayNeededSum");//singleAdvancement["attributeArrayNeededSum"].asInt();
		Json::Value jAttributeArrayNeeded = tryReadValue(singleAdvancement, "attributeArrayNeeded");//singleAdvancement["attributeArrayNeeded"];
		for (int j = 0; j < attributeArrayNeededSum; j++) {
			Json::Value jSingleAttribute = tryReadArray(jAttributeArrayNeeded, j);//jAttributeArrayNeeded[j];
			std::string stmp = tryReadString(jSingleAttribute, "attributeName");// jSingleAttribute["attributeName"].asString();

			std::string keytmp;
			if (reg.regAttribute.find(stmp) == reg.regAttribute.end()) {
				SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found in reg.regAttribute", stmp);
			}
			else {
				keytmp = reg.regAttribute[stmp]->attributeArrayMatch;
			}
			int restrictionSum = tryReadInt(jSingleAttribute, "restrictionSum");//jSingleAttribute["restrictionSum"].asInt();
			Json::Value jrestriction = tryReadValue(jSingleAttribute, "restriction");//jSingleAttribute["restriction"];
			for (int k = 0; k < restrictionSum; k++) {
				Json::Value singlerestriction = tryReadArray(jrestriction, k);//jrestriction[k];
				double tryupper = tryReadDouble(singlerestriction, "upper");
				double trylower = tryReadDouble(singlerestriction, "lower");
				std::pair<double, double> rangetmp(tryupper,trylower/*singlerestriction["upper"].asDouble(), singlerestriction["lower"].asDouble()*/);
				int tryindex = tryReadInt(singlerestriction, "index");
				std::pair<int, std::pair<double, double>> detailtmp(tryindex/*singlerestriction["index"].asInt()*/, rangetmp);
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

	int cardSetSum = tryReadInt(root, "cardSetSum");//root["cardSetSum"].asInt();
	Json::Value jcardSet = tryReadValue(root, "cardSet");//root["cardSet"];
	for (int i = 0; i < cardSetSum; i++) {
		
		Json::Value jSingleCardSet = tryReadArray(jcardSet, i);//jcardSet[i];
		std::string cardSetName = tryReadString(jSingleCardSet, "cardSetName");
		Json::Value jCard = tryReadValue(jSingleCardSet, "card"); //jSingleCardSet["card"];
		int cardNum = tryReadInt(jSingleCardSet, "cardNum");

		CardSet* newCardSet = new CardSet;
		
		for (int j = 0; j < cardNum/*root["cardNum"].asInt()*/; j++) {
			//cs.getCardset().push(jCS[i].asString());
			Json::Value oneCard = tryReadArray(jCard, j);
			std::string oneCardName = tryReadString(oneCard, "name");
			newCardSet->cardSet.emplace(oneCardName/*jCard[i].asString()*/);
		}
		reg.cardSetPtr.emplace(std::pair<std::string, CardSet*>(
			cardSetName/*jSingleCardSet["cardSetName"].asString()*/, newCardSet));

	}

	inFile.close();
	return;
}

void readFormulaJson() {
	std::ifstream inFile("rule/formula.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int formulaSum = tryReadInt(root, "formulaSum");//root["formulaSum"].asInt();
	Json::Value jFormula = tryReadValue(root, "formula");//root["formula"];
	for (int i = 0; i < formulaSum; i++) {
		Json::Value jSingleFormula = tryReadArray(jFormula, i);//jFormula[i];

		Formula* newFormula = new Formula;
		std::string formulaName = tryReadString(jSingleFormula, "formulaName");
		
		reg.formulaAttained.emplace(std::pair<std::string, bool>(formulaName, false));
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
	std::ifstream inFile("rule/class.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int classSum = tryReadInt(root, "classSum");
	reg.regValue.emplace(std::pair<std::string, int>("classSum", classSum/*root["classSum"].asInt()*/));
	Json::Value jClass = tryReadValue(root, "class");//root["class"];
	for (int i = 0; i < classSum/*root["classSum"].asInt()*/; i++) {
		Json::Value jSingleClass = tryReadArray(jClass, i);//jClass["class"];
		std::string onename = tryReadString(jSingleClass, "name");
		int oneindex = tryReadInt(jSingleClass, "index");
		reg.classNameToIndex.emplace(std::pair<std::string, int>(
			onename,oneindex/*jSingleClass["name"].asString(), jSingleClass["index"].asInt()*/));
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
		[](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
			return a.second > b.second;
		});
	for (int i = 0; i < reg.regJsonFile.size(); i++) {
		std::string s = reg.regJsonFile.at(i).first;
		int imp = reg.regJsonFile.at(i).second;
		
		if (s == "attribute.json") {
			readAttributeJson();
		}else if (s == "advancement.json") {
			readAdvancementJson();
		}else if (s == "cardset.json") {
			readCardSetJson();
		}else if (s == "spot.json") {
			readSpotJSon();
		}else if (s == "formula.json") {
			readFormulaJson();
		}else if (s == "class.json") {
			readClassJson();
		}else if (s == "item.json") {
			readItemJson();
		}else {
			SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "no match for {}", s);
		}

		SPDLOG_LOGGER_TRACE(spdlog::get("readjson"), "read json file {} (importance {} ) successfully", s, imp);
	}
}

void readRuleFileJson() {
	std::ifstream inFile("rule/rulefile.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);

	int jsonFileSum = tryReadInt(root, "jsonFileSum");
	Json::Value jjsonFile = tryReadValue(root, "jsonFile");
	for (int i = 0; i < jsonFileSum; i++) {
		Json::Value jSingleJsonFile = tryReadArray(jjsonFile, i);
		std::string oneName = tryReadString(jSingleJsonFile, "name");
		int oneImportance = tryReadInt(jSingleJsonFile, "importance");
		reg.regJsonFile.push_back(std::pair<std::string, int>(oneName, oneImportance));
	}

	inFile.close();
}