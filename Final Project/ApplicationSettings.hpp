#pragma once
#include <iostream>
#include <string>
#include <map>
#include <optional>

class Settings {
private:
    std::map<std::string, std::string> settings_map;

    Settings() {
        std::cout << "[Info] Settings Instance Created." << std::endl;
    }
    ~Settings() {}

public:
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    static Settings& get_instance() {
        static Settings instance;
        return instance;
    }

    void set(const std::string& key, const std::string& value) {
        settings_map[std::string(key)] = std::string(value);
    }

    std::optional<std::string> get(const std::string& key) const {
        auto it = settings_map.find(key);
        if (it != settings_map.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    bool has(const std::string& key) const {
        return settings_map.find(key) != settings_map.end();
    }
};