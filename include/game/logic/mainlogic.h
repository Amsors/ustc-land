#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>

#include "game/logic/cards.h"

class Attribute {
public:
	Attribute();
	void showAttribute();

private:
	std::string name;
	bool visibility;
	double max;
	double attributeValue;
	std::map<std::string,double> attributeArray;
	std::string attributeMatchKey;
	bool isArray;

	friend void readAttributeJson();
	//friend void readAdvancementJson();
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

//class AdvancementMgr {
//private:
//
//
//public:
//
//	friend void readAdvancementJson();
//};
//
//extern AdvancementMgr advMgr;

class Reward {

	friend void readRewardJson();
};

class Formula {
private:
	std::string formulaName;
	std::string cardSetName;
	std::vector<std::string> rewardName;


	friend void readFormulaJson();
};