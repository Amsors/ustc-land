#include "i18n.h"

#include <filesystem>
#include <fstream>

#include "spdlog/spdlog.h"

static std::shared_ptr<spdlog::logger> i18nLogger;
static const std::filesystem::path &LANG_EXTENSION = ".lang";
static std::map<std::string, std::map<std::string, std::string>> translations;
std::string curr;

bool i18n::initTranslator() {
    if(i18nLogger == nullptr) {
        i18nLogger = spdlog::get("i18n");
    }
    SPDLOG_LOGGER_TRACE(i18nLogger, "Initialising translator...");

    const std::filesystem::path langFolder = "assets/lang";
    if(!std::filesystem::exists(langFolder) || !std::filesystem::is_directory(langFolder)) {
        SPDLOG_LOGGER_ERROR(i18nLogger, "Language folder 'lang' does not exist.");
        return false;
    }

    bool flag = false;
    for(const auto &entry: std::filesystem::directory_iterator(langFolder)) {
        if(entry.is_regular_file()) {
            if(
                const std::filesystem::path &file = entry.path();
                file.extension() == LANG_EXTENSION
            ) {
                SPDLOG_LOGGER_INFO(i18nLogger, "Loading language file: {}", file.filename().string());
                const std::string &lang = file.stem().string();
                if(curr.empty() || (curr != lang && lang == "zh_cn")) {
                    curr = lang;
                    flag = true;
                }
                std::ifstream in(file);
                if(!in.is_open()) {
                    SPDLOG_LOGGER_WARN(i18nLogger, "Failed to open language file: {}", file.filename().string());
                    continue;
                }
                std::map<std::string, std::string> langMap;
                std::string line;
                while(std::getline(in, line)) {
                    const std::string::size_type pos = line.find('=');
                    const std::string &key = line.substr(0, pos);
                    const std::string &value = line.substr(pos + 1);
                    SPDLOG_LOGGER_INFO(i18nLogger, value);
                    if(langMap.contains(key)) {
                        SPDLOG_LOGGER_WARN(i18nLogger, "Duplicate key '{}' in language file '{}'. Overriding it.", key, lang);
                    }
                    langMap[key] = value;
                }
                translations[lang] = langMap;
            } else {
                SPDLOG_LOGGER_WARN(i18nLogger, "File '{}' is not a LANG file. Ignoring it.", file.filename().string());
            }
        }
    }
    return flag;
}

bool i18n::setLanguage(const std::string &lang) {
    if(!translations.contains(lang)) {
        SPDLOG_LOGGER_WARN(i18nLogger, "Language {} does not exist.", lang);
        return false;
    }
    SPDLOG_LOGGER_INFO(i18nLogger, "Setting language to {}.", lang);
    curr = lang;
    return true;
}

const std::string &i18n::translated(const std::string &key) {
    return translations[curr].contains(key) ? translations[curr][key] : key;
}
