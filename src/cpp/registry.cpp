#include "game/logic/registry.h"

Registry reg;

void Registry::outputAttribute() {
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


void Registry::AttributeOutput() {
	std::cout << "size: " << reg.regAttribute.size() << std::endl;
	for (const auto& attr : reg.regAttribute) {
		auto attrPtr = attr.second;
		if (attrPtr->getIsArray() == true) {
			//输出数组型的属性
			std::cout << "[Array]" << attrPtr->getName() << ":" << std::endl;
			for (const auto& element : attrPtr->getAttributeArray()) {
				std::cout << "element: " << element.first << " value: " << element.second << std::endl;
			}
		}
		else {
			//输出元素型的属性
			std::cout << "[Value]" << attrPtr->getName() << ": " << attrPtr->getAttributeValue() << std::endl;
		}
	}
}

void Registry::AdvancementOutput() {
	
}