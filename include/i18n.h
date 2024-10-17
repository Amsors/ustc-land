#pragma once

#include <string>

namespace i18n {
    bool initTranslator();
    bool setLanguage(const std::string &lang);
    const std::string &translated(const std::string &key);
}
