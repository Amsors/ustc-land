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

	std::map<std::string, std::vector<std::string>> regArrayElements;

	std::map<std::string, bool> cardAttained;//是否获取过卡牌
	std::map<std::string, bool> formulaAttained;//是否合成过配方
	std::map<std::string, bool> itemAttained;
	
	std::map<std::string, CardSet*> cardSetPtr;
	std::map<std::string, Formula*> formulaPtr;

	std::map<std::string, std::vector<SpotCard*>> spotCardPtr;//记录所有地点卡牌的指针，下同
	std::map<std::string, std::vector<RoleCard*>> roleCardPtr;
	std::map<std::string, std::vector<ItemCard*>> ItemCardPtr;

	
};

extern Register reg;