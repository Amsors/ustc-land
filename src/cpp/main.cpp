#include <iostream>
#include <stdexcept>
#include <string>

#include "i18n.h"
#include "main_application.h"
#include "glad/glad.h"
#include "nanogui/formhelper.h"
#include "nanogui/screen.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/wincolor_sink.h"

int main() {
    std::vector<spdlog::sink_ptr> sinks = {
        #ifndef __USTC_LAND_RELEASE__
        std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>(),
        #endif
        std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/log.txt", 23, 59)
    };
    // TODO: 添加日志
    // ReSharper disable once CppTooWideScopeInitStatement
    const std::vector loggers = {
        make_shared<spdlog::logger>("i18n", begin(sinks), end(sinks))
    };
    for(const auto &logger: loggers) {
        spdlog::register_logger(logger);
    }
    spdlog::set_pattern("[%H:%M:%S] (%n) [%^%l%$] %v");

    i18n::initTranslator();

    try {
        nanogui::init();
        nanogui::ref app = new MainApplication();
        app->draw_all();
        app->set_visible(true);
        nanogui::mainloop(1 / 60.f * 1000);
        nanogui::shutdown();
    } catch(const std::runtime_error &e) {
        MessageBoxA(nullptr, (std::string("Caught a fatal error: ") + std::string(e.what())).c_str(), nullptr, MB_ICONERROR | MB_OK);
        return -1;
    }
    return 0;
}
