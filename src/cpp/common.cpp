#include "game/common.h"

#include <fstream>

#include "i18n.h"
#include "json/json_features.h"
#include "json/reader.h"
#include "nanogui/opengl.h"
#include "spdlog/spdlog.h"

std::map<std::string, std::string> texturePaths;

bool loadShaders(const std::string &name, std::string &vs, std::string &fs) {
    using namespace std::string_literals;
    std::ifstream file;
    file.open("shaders/" + name + ".vert", std::ios::in);
    if(!file.is_open()) {
        SPDLOG_LOGGER_ERROR(spdlog::get("render"), fmt::runtime(i18n::translated("shader.load.fail.vertex")), name);
        return false;
    }
    vs = std::move(std::string(std::istreambuf_iterator(file), std::istreambuf_iterator<char>()));
    file.close();
    file.open("shaders/" + name + ".frag", std::ios::in);
    if(!file.is_open()) {
        SPDLOG_LOGGER_ERROR(spdlog::get("render"), fmt::runtime(i18n::translated("shader.load.fail.fragment")), name);
        return false;
    }
    fs = std::move(std::string(std::istreambuf_iterator(file), std::istreambuf_iterator<char>()));
    file.close();
    return true;
}

void initTexturePaths() {
    std::ifstream in(R"(textures/info.json)", std::ios::in);
    if(!in.is_open()) {
        SPDLOG_LOGGER_ERROR(spdlog::get("main"), "Failed to open texture info file");
        return;
    }
    Json::Reader reader;
    if(Json::Value root; reader.parse(in, root)) {
        for(auto &member: root.getMemberNames()) {
            SPDLOG_LOGGER_TRACE(spdlog::get("main"), "Reading texture info file: {} -> {}", member, root[member].asString());
            texturePaths[member] = root[member].asString();
        }
    } else {
        SPDLOG_LOGGER_ERROR(spdlog::get("main"), "Failed to parse texture info file");
    }
}

std::string &getTexturePath(const std::string &name) {
    #ifndef __USTC_LAND_RELEASE__
    assert(texturePaths.contains(name));
    #endif
    return texturePaths[name];
}
