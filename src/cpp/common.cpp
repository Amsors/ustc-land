#include "game/common.h"

#include <fstream>

#include "i18n.h"
#include "nanogui/opengl.h"
#include "spdlog/spdlog.h"

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

nanogui::Theme *createBarTheme(NVGcontext *ctx) {
    static nanogui::Theme *barTheme = nullptr;
    if(barTheme == nullptr) {
        barTheme = new nanogui::Theme(ctx);
        // TODO: 设置Bar的基础样式
    }
    return barTheme;
}
