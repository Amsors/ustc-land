#include <map>
#include <fstream>

#include "game/logic/basic.h"
#include "json/json.h"
#include "game/logic/mainlogic.h"

Advancement::Advancement() {
	this->established = 0;
}

//AdvancementMgr advMgr;