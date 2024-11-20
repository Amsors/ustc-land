#include <fstream>
#include <map>
#include <json/json.h>

#include <game/logic/mainlogic.h>
#include <game/logic/cards.h>
#include <game/logic/register.h>

void readAttributeJson() {
	std::ifstream inFile("rule/attribute.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inFile, root);
	int attributeValueSum = root["attributeValueSum"].asInt();
	Json::Value jValue = root["attributeValue"];
	for (int i = 0; i < attributeValueSum; i++) {
		Json::Value single = jValue[i];
		Attribute* att = new Attribute();

		att->isArray = 0;
		att->name = single["name"].asString();
		att->visibility = single["visibility"].asBool();
		att->max = single["max"].asDouble();
		att->initial = single["initial"].asDouble();

		att->showAttribute();

		reg.regAttribute.emplace(std::pair<std::string, Attribute*>(att->name, att));
	}

	int attributeArraySum = root["attributeArraySum"].asInt();
	Json::Value jArray = root["attributeArray"];
	for (int i = 0; i < attributeArraySum; i++) {
		Json::Value single = jArray[i];
		if (single["arrayLengthStrategy"].asString() == "match") {
			std::string keyTemp(single["arrayLengthMatchKey"].asString());
			int length = reg.regValue[keyTemp];
			Attribute* att = new Attribute();

			att->isArray = 1;
			att->name = single["name"].asString();
			att->visibility = single["visibility"].asBool();

			for (int j = 0; j < length; j++) {
				att->attributeArray.push_back(0);
			}
			att->attributeArrayMatch = single["arrayLengthMatchKey"].asString();

			att->showAttribute();
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
		Json::Value single = j[i];
		Advancement* adv = new Advancement();

		adv->name = single["name"].asString();

		int formulaNeededSum = single["formulaNeededSum"].asInt();
		Json::Value jFormulaNeeded = single["formulaNeeded"];
		for (int j = 0; j < formulaNeededSum; j++) {
			adv->formulaNeeded.push_back(jFormulaNeeded[j].asString());
		}
		
		int attributeValueNeededSum = single["attributeValueNeededSum"].asInt();
		Json::Value jAttributeValueNeeded = single["attributeValueNeeded"];
		for (int j = 0; j < attributeValueNeededSum; j++) {
			Json::Value jtmp = jAttributeValueNeeded[j];
			std::pair<double, double> ptmp(jtmp["upper"].asDouble(), jtmp["lower"].asDouble());
			std::string stmp = jtmp["attributeName"].asString();
			adv->attributeValueNeeded.emplace(std::pair<std::string, std::pair<double, double>>(
				stmp, ptmp));
		}

		int attributeArrayNeededSum = single["attributeArrayNeededSum"].asInt();
		Json::Value jAttributeArrayNeeded = single["attributeArrayNeeded"];
		for (int j = 0; j < attributeArrayNeededSum; j++) {
			Json::Value jtmp = jAttributeArrayNeeded[j];
			std::string stmp = jtmp["attributeName"].asString();
			std::string keytmp = reg.regAttribute[stmp]->attributeArrayMatch;
			int restrictionSum = jtmp["restrictionSum"].asInt();
			Json::Value res = jtmp["restriction"];
			for (int k = 0; k < restrictionSum; k++) {
				Json::Value singleres = res[k];
				std::pair<double, double> resvalue(singleres["upper"].asDouble(), singleres["lower"].asDouble());
				std::pair<int, std::pair<double, double>> resdetail(singleres["index"].asInt(), resvalue);
				adv->attributeArrayNeeded.emplace(std::pair<std::string, std::pair<int, std::pair<double, double>>>(
					stmp, resdetail
				));
			}
		}
		
		reg.regAdvancement.emplace(std::pair<std::string, Advancement*>(adv->name, adv));
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
	Json::Value jf = root["formula"];
	for (int i = 0; i < formulaSum; i++) {
		Json::Value jfsingle = jf[i];

		if (reg.cardSetRegistered[jfsingle["cardSetName"].asString()] == false) {
			reg.cardSetRegistered[jfsingle["cardSetName"].asString()] = true;
			CardSet* newCardSet = new CardSet;
			readCardSetJson(jfsingle, *newCardSet);
			reg.cardSetPtr.emplace(std::pair<std::string, CardSet*>(
				jfsingle["cardSetName"].asString(), newCardSet));
		}

		Formula* newformula = new Formula;
		reg.formulaAttained[jfsingle["formulaName"].asString()] = false;
		reg.formulaPtr.emplace(std::pair<std::string, Formula*>(
			jfsingle["formulaName"].asString(), newformula));
		//TODO reward²¿·Ö

	}

	inFile.close();
	return;
}

void readSpotJSon() {

}

void readClassJson() {

}

void readItemJson() {

}