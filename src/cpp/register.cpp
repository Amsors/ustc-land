#include <map>
#include <fstream>

#include "game/logic/main.h"
#include "game/logic/basic.h"
#include "json/json.h"
#include "game/logic/main.h"

class Advancement {
	Advancement() {
		established = false;
	}
private:
	std::string advancementName;
	bool established;
};

Register reg;

Attribute::Attribute() {
	name = "default";
	visibility = false;
	max = 0;
	initial = 0;
	isArray = false;
}

void Attribute::showAttribute() {
	using namespace std;
	cout << this->name << endl;
	cout << "is array:" << this->isArray << endl;
	cout << "visiable:" << this->visibility << endl;
	cout << "max:" << this->max << " " << "initial:" << this->initial << endl;
	if (this->isArray) {
		cout << "array length:" << this->attributeArray.size() << endl;
	}
	cout << endl;
	return;
}

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
			att->showAttribute();
		}
	}
}