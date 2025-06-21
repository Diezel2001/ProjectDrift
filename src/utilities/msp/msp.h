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
        msp(serial &mySerial);
        msp();

        void sendCmd(uint8_t data_length, uint8_t code, const std::vector<uint8_t>& data);
        Payload getData(uint8_t cmd);

        std::any sendMspCmd(uint8_t code, const std::any& data);
        std::any sendMspCmd(uint8_t code);

        Payload processData(uint8_t cmd, ssize_t count, char* buff);
        bool checkMspResponse(char* buff, ssize_t count);

        std::string getName();

        vtxConfigIn getVtx();
        void setVtx(uint8_t band, uint8_t channel);
        void setVtx(uint16_t freq);

        void getAttitude();
        rcChannelData getRC();

        imuData getRawIMU();
    
    private:
        serial* m_serial;

        std::unordered_map<std::string, std::function<std::any(std::vector<std::any>)>> mspHandlers = {
            { "MSP_NAME", [=](std::vector<std::any>) -> std::any {
                return getName();
            }},
            { "MSP_VTX_CONFIG", [=](std::vector<std::any>) -> std::any {
                return getVtx();
            }},
            { "MSP_SET_VTX_CONFIG", [=](std::vector<std::any> args) -> std::any {
                if (args.size() >= 2)
                {
                    setVtx(std::any_cast<uint8_t>(args[0]), std::any_cast<uint8_t>(args[1]));
                }
                else
                {
                    setVtx(std::any_cast<uint16_t>(args[0]));
                }
                return {};
            }},
            { "MSP_ATTITUDE", [=](std::vector<std::any> args) -> std::any {
                getAttitude();
                return {};
            }},
            { "MSP_RC", [=](std::vector<std::any> args) -> std::any {
                return getRC();
            }},
            { "MSP_RAW_IMU", [=](std::vector<std::any> args) -> std::any {
                return getRawIMU();
            }},
        };
    };

    

}