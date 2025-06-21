#pragma once

#include "msp_protocol_betaflight.h"
#include "msp_vtx.h"

#include "serial.h"

#include <string>
#include <vector>
#include <cstdint>
#include <function>

#define MAX_RECIEVE_BUFFER 50

namespace MSP {
    using Payload = std::vector<uint8_t>;

    class msp
    {
    public:
        msp(serial &mySerial);
        msp();

        void sendCmd(uint8_t data_length, uint8_t code, const std::vector<uint8_t>& data);
        Payload getData(uint8_t cmd);

        Payload processData(uint8_t cmd, ssize_t count, char* buff);
        bool checkMspResponse(char* buff, ssize_t count);

        void getName();

        vtxConfigIn getVtx();
        void setVtx(uint8_t band, uint8_t channel);
        void setVtx(uint16_t freq);

        void getAttitude();



    private:
        serial* m_serial;
    };

    std::unordered_map<std::string, std::function<std::any()>> mspHandlers = {
    { "MSP_NAME",    []() -> std::any { return getName(); } },
    { "string", []() -> std::any { return getString(); } },
    { "double", []() -> std::any { return getDouble(); } }
};

}