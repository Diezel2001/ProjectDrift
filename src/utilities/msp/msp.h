#pragma once

#include "msp_protocol_betaflight.h"
#include "msp_datatypes.h"

#include "serial.h"

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <any>

#define MAX_RECIEVE_BUFFER 50

namespace MSP {
    using Payload = std::vector<uint8_t>;

    class msp
    {
    public:
        msp(const std::string portName, int baudrate);
        ~msp();

        void sendCmd(uint8_t data_length, uint8_t code, const std::vector<uint8_t>& data);
        Payload getData(uint8_t cmd);

        Payload processData(uint8_t cmd, ssize_t count, char* buff);
        bool checkMspResponse(char* buff, ssize_t count);

        nameData getName();
        void setName(const nameData new_name);

        vtxConfigIn getVtx();
        void setVtx(uint8_t band, uint8_t channel);
        void setVtx(uint16_t freq);

        void getAttitude();
        rcChannelData getRC();

        imuData getRawIMU();
        analogData getAnalogData();
    
    private:
        serial* m_serial;

    };
}