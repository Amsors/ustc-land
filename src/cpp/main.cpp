#include <iostream>
#include <stdexcept>
#include <string>

#include "i18n.h"
#include "game/main_application.h"
#include "nanogui/screen.h"
#include "nanovg/stb_image.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/wincolor_sink.h"
#include "game/logic/main.h"

#ifdef __USTC_LAND_RELEASE__
#   pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#else
#   pragma comment(linker, "/SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup")
#endif

int main() {
    reg.regValue.emplace(std::pair<std::string, int>("classSum",5));
    readAttributeJson();

    std::cout << "finished!";
    return 0;

    #ifndef __USTC_LAND_RELEASE__
    system("chcp 65001");
    #endif

    std::vector<spdlog::sink_ptr> sinks = {
        #ifndef __USTC_LAND_RELEASE__
        std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>(),
        #endif
        std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/log.txt", 23, 59)
    };
    // TODO: 添加日志
    const std::vector loggers = {
        std::make_shared<spdlog::logger>("main", begin(sinks), end(sinks)),
        std::make_shared<spdlog::logger>("i18n", begin(sinks), end(sinks)),
        std::make_shared<spdlog::logger>("render", begin(sinks), end(sinks))
    };
    for(const auto &logger: loggers) {
        spdlog::register_logger(logger);
    }
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%H:%M:%S] (%n) [%^%l%$] %v");

    stbi_set_flip_vertically_on_load(true);
    i18n::initTranslator();

    try {
        nanogui::init();
        nanogui::ref app = new MainApplication();
        // app->draw_all();
        app->set_visible(true);
        nanogui::mainloop(1 / 300.f * 1000);
    } catch(const std::runtime_error &e) {
        SPDLOG_LOGGER_ERROR(loggers[0], fmt::runtime(i18n::translated("main.error.exit")), e.what());
        return -1;
    }
    nanogui::shutdown();
    return 0;
}
