#pragma once
#include <iostream>
#include "game/logic/mainlogic.h"
#include "game/logic/cards.h"

class Register {
public:
	std::vector<std::pair<std::string,int>> regJsonFile;

	std::map<std::string, int> regValue;

	std::map<std::string, Attribute*> regAttribute;
	std::map<std::string, Advancement*> regAdvancement;
	std::map<std::string, int> classNameToIndex;

	std::map<std::string, bool> cardAttained;//是否获取过卡牌
	//TODO 增加card构造函数中的初始化
	std::map<std::string, bool> formulaAttained;//是否合成过配方
	std::map<std::string, bool> itemAttained;

	std::map<std::string, CardSet*> cardSetPtr;
	std::map<std::string, Formula*> formulaPtr;
};

extern Register reg;