#include "config.h"
#include <fstream>
#include <stdexcept>

Config::Config(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filepath);
    }
    file >> configData;
}

std::string Config::getSerialPort() const {
    return configData["serial_port"].get<std::string>();
}

int Config::getBaudRate() const {
    return configData["baud_rate"].get<int>();
}
