//#include<temp/temp.h>
//#include<json/json.h>
//#include<fstream>
//#include<iostream>
//
//#include<spdlog/spdlog.h>
//
//
//std::string tryReadS(Json::Value v,std::string key) {
//	if (v.isMember(key)) {
//		return v[key].asString();
//	}
//	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
//	return v[key].asString();
//}
//
//int tryReadI(Json::Value &v, std::string key) {
//	if (v.isMember(key)) {
//		return v[key].asInt();
//	}
//	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
//	return v[key].asInt();
//}
//
//bool tryReadB(Json::Value v, std::string key) {
//	if (v.isMember(key)) {
//		return v[key].asBool();
//	}
//	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
//	return v[key].asBool();
//}
//
//double tryReadD(Json::Value v, std::string key) {
//	if (v.isMember(key)) {
//		return v[key].asDouble();
//	}
//	SPDLOG_LOGGER_WARN(spdlog::get("readjson"), "key: {} not found", key);
//	return v[key].asDouble();
//}
//
//
//void test() {
//	std::ifstream inFile("rule/test.json");
//
//	std::cout << std::endl << std::endl << std::endl;
//	int a;
//
//	Json::Reader reader;
//	Json::Value root;
//	reader.parse(inFile, root);
//
//	a = tryReadI(root, "test1");
//	std::cout << a;
//
//	inFile.close();
//	return;
//}