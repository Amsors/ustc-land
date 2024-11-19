#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>

class Attribute {
public:
	Attribute();
	void showAttribute();

private:
	std::string name;
	bool visibility;
	double max;
	double initial;
	std::vector<int> attributeArray;
	bool isArray;

	friend void readAttributeJson();
};

void readAttributeJson();

class Register {
public:
	std::map<std::string, int> regValue;
	std::map<std::string, Attribute*> regAttribute;
};

extern Register reg;
