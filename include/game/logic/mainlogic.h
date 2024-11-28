#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>

#include "game/logic/cards.h"
#include "game/main_application.h"

class Attribute {
public:
	Attribute();
	//void showAttribute();
	double& getAttributeValue() {
		return this->attributeValue;
	}
	double getMax() const {
		return this->max;
	}
	bool getIsArray() const {
		return this->isArray;
	}
	bool getVisibility() const {
		return this->visibility;
	}
	std::map<std::string, double>& getAttributeArray() {
		return this->attributeArray;
	}
	std::string getName() {
		return this->name;
	}
	std::string getAttributeMatchKey() {
		return this->attributeMatchKey;
	}
	
private:
	//for all
	std::string name;
	bool visibility;
	bool isArray;

	//for attributeValue
	double max;
	double attributeValue;

	//for attributeArray
	std::map<std::string,double> attributeArray;
	std::string attributeMatchKey;

	friend void readAttributeJson();
};


class AttributeChange {
private:
	bool isArray;
	std::string name;
	std::string changeType;
	/*
	changeType:
	ratio_of_rest  add  mult
	*/
	int change;
	int index;
};

class Advancement {
public:
	Advancement();

private:
	std::string name;
	std::map<std::string, std::pair<double,double>> attributeValueNeeded;
	std::map<std::string, std::pair<std::string,std::pair<double, double>>> attributeArrayNeeded;
	std::vector<std::string> formulaNeeded;
	std::vector<std::string> itemNeeded;
	std::vector<std::string> cardNeeded;
	int established;

	friend void readAdvancementJson();
};

class Reward {
public:
	std::string getName() {
		return this->name;
	}
	std::string getAttributeName() {
		return this->attributeName;
	}
	std::string getChange() {
		return this->change;
	}
	double getChangeValue() {
		return this->changeValue;
	}
	std::string getKey() {
		return this->key;
	}
	std::string getType() const {
		std::string tmps = this->type;
		return tmps;
	}
	std::string getCardName() {
		return this->cardName;
	}
private:
	//for all
	std::string name;
	std::string type;

	//for attributeValue & attrbuteArray
	std::string attributeName;
	std::string change;
	//ratio_of_rest  add  mult
	double changeValue;

	//for attributeArray
	std::string key;

	//for card
	std::string cardName;

	friend void readRewardJson();
	//friend class MainApplication;
};

class Formula {
public:
	std::vector<std::string>& getRewardName() {
		return this->rewardName;
	}
private:
	std::string formulaName;
	std::string cardSetName;
	std::vector<std::string> rewardName;


	friend void readFormulaJson();
};