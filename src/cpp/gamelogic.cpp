#include <map>
#include <fstream>

#include "game/logic/basic.h"
#include "json/json.h"
#include "game/logic/mainlogic.h"

Attribute::Attribute() {
	name = "default";
	visibility = false;
	max = 0;
	isArray = false;
}

//void Attribute::showAttribute() {
//	using namespace std;
//	cout << this->name << endl;
//	cout << "is array:" << this->isArray << endl;
//	cout << "visiable:" << this->visibility << endl;
//	if (this->isArray) {
//		cout << "array length:" << this->attributeArray.size() << endl;
//	}
//	cout << endl;
//	return;
//}

double Attribute::getAttributeValue() const {
	return this->attributeValue;
}