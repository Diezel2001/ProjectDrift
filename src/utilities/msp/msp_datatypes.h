#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace MSP 
{
    struct nameData
    {
        std::string name;

        nameData(const std::vector<uint8_t>& result);
        void print();
    };

    struct rcChannelData
    {
        uint16_t roll;
        uint16_t pitch;
        uint16_t yaw;
        uint16_t throttle;
        uint16_t aux4;

        rcChannelData(const std::vector<uint16_t>& result);
        void print();
    };

    struct statusData
    {
        
    };

    struct imuData
    {
        float accel_x;
        float accel_y;
        float accel_z;
        float gyro_x;
        float gyro_y;
        float gyro_z;
        float mag_x;
        float mag_y;
        float mag_z;

        imuData(const std::vector<float>& result);
        void print();
    };

    struct analogData
    {
        uint8_t batteryVoltage;
        uint16_t mAhDrawn;
        uint16_t rssi;
        uint16_t amperage;

        analogData(const std::vector<uint8_t>& result);
        void print();
    };

    struct vtxConfigIn
    {
        uint8_t vtxType;
        uint8_t vtxBand;
        uint8_t vtxChannel;
        uint8_t vtxPower;
        uint8_t vtxStatus;
        uint16_t vtxFreq;
        uint8_t vtxDeviceReady;
        uint8_t vtxLowPowerDisarm;
        uint16_t vtxPitFreq;
        uint8_t vtxTableAvail;
        uint8_t vtxTableBands;
        uint8_t vtxTableChannels;
        uint8_t vtxPowerLevels;
        uint8_t padding1;
        uint8_t padding2;
        uint8_t padding3;
        uint8_t padding4;

        vtxConfigIn(const std::vector<uint8_t>& result);
        void print();
    };
}

