#pragma once

#include <string>
#include <nlohmann/json.hpp>

class Config{
public:
    explicit Config(const std::string& filepath);

    std::string getSerialPort() const;
    int getBaudRate() const;

private:
    nlohmann::json configData;
};
