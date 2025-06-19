#pragma once

#include "msp_protocol_betaflight.h"

#include "serial.h"

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

#define MAX_RECIEVE_BUFFER 50

namespace MSP {
    class msp
    {
    public:
        msp(serial &mySerial);
        msp();

        void sendCmd(uint8_t data_length, uint8_t code, const std::vector<uint8_t>& data);
        void getData(uint8_t cmd);

        void processData(uint8_t cmd, ssize_t count, char* buff);
        bool checkMspResponse(char* buff, ssize_t count);

    private:
        serial* m_serial;
    };

    struct mspData
    {
        uint8_t* payload;
        size_t* payload_len;
    };

}