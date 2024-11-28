#include "game/logic/register.h"

Register reg;

void Register::outputAttribute() {
	std::cout << "---AttributeBegin---\n";
	for (int i = 0; i < this->allAttribute.size(); i++) {
		Attribute* a = this->regAttribute[this->allAttribute.at(i)];
		if (a->getVisibility()==false) {
			continue;
		}
		if (a->getIsArray()) {
			std::cout << a->getName();
			std::cout << a->getAttributeMatchKey() << "\n";
			for (int j = 0; j < reg.regArrayElements[a->getAttributeMatchKey()].size(); j++) {
				std::string tmp = reg.regArrayElements[a->getAttributeMatchKey()].at(j);
				std::cout << "{ key:" << tmp << ",value:" << a->getAttributeArray()[tmp] << "},";
			}
			std::cout << "\n";
		}
		else {
			std::cout << a->getName() << ":     " << a->getAttributeValue() << "\n";
		}
	}
	std::cout << "---AttributeEnd---\n";
}
