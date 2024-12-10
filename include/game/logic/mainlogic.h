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
	//void showAttribute();
	double& getAttributeValue() {
		return this->attributeValue;
	}
	double getAttributeValueRound() {
		int tmp = this->attributeValue*100;
		return (double)tmp / 100.0;
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
	std::string name = "default";
	bool visibility = false;
	bool isArray = false;

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

class Range {
protected:
	double upper;
	double lower;

	friend class Advancement;
	friend class AttributeValueNeeded;
	friend class AttributeArrayNeeded;
	friend void readAdvancementJson();
};

class AttributeValueNeeded {
protected:
	std::string name;
	Range res;

	friend class Advancement;
	friend void readAdvancementJson();
};

class AttributeArrayNeeded {
protected:
	std::string name;
	std::vector<std::pair<std::string,Range>> res;

	friend class Advancement;
	friend void readAdvancementJson();
};

class Advancement {
public:
	enum AdvancementStatus {
		LOCKED_P, LOCKED_N, SHOWN_P, SHOWN_N
	};

	const std::string& getName() const {
		return this->name;
	}
	bool getEstablished() const {
		return this->established;
	}
	
	Advancement() {};
	bool checkAdvancement();

private:
	std::string name;
	std::vector<AttributeValueNeeded> attributeValueNeeded;
	std::vector<AttributeArrayNeeded> attributeArrayNeeded;

	std::vector<std::string> formulaNeeded;
	std::vector<std::string> itemNeeded;
	std::vector<std::string> cardNeeded;
	bool established = false;

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
	bool getIsVague() {
		return this->isVague;
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
	bool isVague = false;

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
	std::map<std::string, int>& getRewardSet() {
		return this->rewardSet;
	}
	std::map<std::string, double>& getRewardPossibility() {
		return this->rewardPossibility;
	}
	std::string& getRewardVagueMatch(){
		return this->rewardVagueMatch;
	}
	std::string& getCardSetVagueMatch() {
		return this->cardSetVagueMatch;
	}
private:
	std::string formulaName;
	std::string cardSetName;
	std::vector<std::string> rewardName;
	std::map<std::string, int> rewardSet;
	std::map<std::string, double> rewardPossibility;
	std::string rewardVagueMatch;
	std::string cardSetVagueMatch;


	friend void readFormulaJson();
};