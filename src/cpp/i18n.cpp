#include "i18n.h"

#include <filesystem>
#include <fstream>

#include "spdlog/spdlog.h"

static const std::filesystem::path &LANG_EXTENSION = ".lang";
static std::map<std::string, std::map<std::string, std::string>> translations;
std::string curr;

bool i18n::initTranslator() {
    SPDLOG_LOGGER_TRACE(spdlog::get("i18n"), "Initialising translator...");

    const std::filesystem::path langFolder = "lang";
    if(!std::filesystem::exists(langFolder) || !std::filesystem::is_directory(langFolder)) {
        SPDLOG_LOGGER_WARN(spdlog::get("i18n"), "Language folder 'lang' does not exist.");
        return false;
    }

    bool flag = false;
    for(const auto &entry: std::filesystem::directory_iterator(langFolder)) {
        if(entry.is_regular_file()) {
            if(
                const std::filesystem::path &file = entry.path();
                file.extension() == LANG_EXTENSION
            ) {
                SPDLOG_LOGGER_INFO(spdlog::get("i18n"), "Loading language file: " + file.filename().string());
                const std::string &lang = file.stem().string();
                if(curr.empty() || (curr != lang && lang == "zh_cn")) {
                    curr = lang;
                    flag = true;
                }
                std::ifstream in(file);
                if(!in.is_open()) {
                    SPDLOG_LOGGER_WARN(spdlog::get("i18n"), "Failed to open language file: " + file.filename().string());
                    continue;
                }
                std::map<std::string, std::string> langMap;
                std::string line;
                while(std::getline(in, line)) {
                    std::string::size_type pos = line.find_first_of('=');
                    if(pos == std::string::npos) {
                        continue;
                    }
                    const std::string &key = line.substr(0, pos);
                    std::string value = std::move(line.substr(pos + 1));
                    SPDLOG_LOGGER_TRACE(spdlog::get("i18n"), ("Found key '" + key).append("' with value '").append(value).append("' in language file '").append(lang).append("'.lang"));
                    if(langMap.contains(key)) {
                        SPDLOG_LOGGER_WARN(spdlog::get("i18n"), ("Duplicate key '" + key).append("' in language file '").append(lang).append("'. Overriding it."));
                    }
                    pos = 0;
                    while((pos = value.find("\\n", pos)) != std::string::npos) {
                        value.replace(pos, 2, "\n");
                    }
                    langMap[key] = value;
                }
                translations[lang] = langMap;
            } else {
                SPDLOG_LOGGER_WARN(spdlog::get("i18n"), ("File '" + file.filename().string()).append("' is not a LANG file. Ignoring it."));
            }
        }
    }
    return flag;
}

bool i18n::setLanguage(const std::string &lang) {
    if(!translations.contains(lang)) {
        SPDLOG_LOGGER_WARN(spdlog::get("i18n"), ("Language " + lang).append(" does not exist"));
        return false;
    }
    SPDLOG_LOGGER_INFO(spdlog::get("i18n"), "Setting language to " + lang);
    curr = lang;
    return true;
}

const std::string &i18n::translated(const std::string &key) {
    return translations[curr].contains(key) ? translations[curr][key] : key;
}
