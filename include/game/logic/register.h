#pragma once
#include <iostream>
#include "game/logic/mainlogic.h"
#include "game/logic/cards.h"

class Register {
public:
	std::map<std::string, int> regValue;

	std::map<std::string, Attribute*> regAttribute;
	std::map<std::string, Advancement*> regAdvancement;
	std::map<std::string, int> classNameToIndex;

	std::map<std::string, bool> cardAttained;//�Ƿ��ȡ������
	//TODO ����card���캯���еĳ�ʼ��
	std::map<std::string, bool> formulaAttained;//�Ƿ�ϳɹ��䷽
	std::map<std::string, bool> itemAttained;
	
	std::map<std::string, CardSet*> cardSetPtr;
	std::map<std::string, Formula*> formulaPtr;

	std::map<std::string, std::vector<SpotCard*>> spotCardPtr;//��¼���еص㿨�Ƶ�ָ�룬��ͬ
	std::map<std::string, std::vector<RoleCard*>> roleCardPtr;
	std::map<std::string, std::vector<ItemCard*>> ItemCardPtr;

	
};

extern Register reg;