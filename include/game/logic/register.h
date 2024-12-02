#pragma once
#include <iostream>
#include "game/logic/mainlogic.h"
#include "game/logic/cards.h"
#include "nanogui/screen.h"
#include <set>

class Register {
public:
	std::vector<std::pair<std::string,int>> regJsonFile;

	std::map<std::string, int> regValue;
	std::map<std::string, Attribute*> regAttribute;
	std::map<std::string, Advancement*> regAdvancement;
	std::map<std::string, int> advancementStatus;

	std::map<std::string, std::vector<std::string>> regArrayElements;
	//        数组注册名称      数组元素的索引

	std::map<std::string, bool> cardAttained;
	std::map<std::string, bool> formulaAttained;
	std::map<std::string, bool> itemAttained;

	std::map<CardSet, std::string> cardSetMap;
	std::map<std::string, double> cardSetTimeNeeded;
	std::map<std::string, std::vector<std::string>> cardSetLostCard;

	std::vector<std::string> allCardType;
	std::map<std::string, nanogui::Color> cardTypeColor;
	std::map<std::string, std::set<std::string>> allCard;
	std::vector<std::string> allAttribute;

	std::map<std::string, std::vector<std::string>> cardSetToFormula;

	std::map<std::string, Formula*> formulaPtr;
	std::map<std::string, Reward*> rewardPtr;
	
	void outputAttribute();
};

extern Register reg;