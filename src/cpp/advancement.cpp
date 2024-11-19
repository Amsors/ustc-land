#include <map>
#include <fstream>

#include "game/logic/main.h"
#include "game/logic/basic.h"
#include "json/json.h"
#include "game/logic/main.h"

class Advancement {
	Advancement() {
		established = false;
	}
private:
	std::string advancementName;
	bool established;
};