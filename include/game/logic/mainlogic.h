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
	double getAttributeValue() const;

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
	ratio  ratio_of_rest  add  mult
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
private:
	//for all
	std::string name;

	//for attributeValue & attrbuteArray
	std::string attributeName;
	std::string change;
	double changeValue;

	//for attributeArray
	std::string key;

	//for card
	std::string type;
	std::string cardName;

	friend void readRewardJson();
	friend class MainApplication;
};

class Formula {
private:
	std::string formulaName;
	std::string cardSetName;
	std::vector<std::string> rewardName;


	friend void readFormulaJson();
};