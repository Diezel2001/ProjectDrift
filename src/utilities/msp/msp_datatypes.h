#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

namespace MSP 
{
    struct rcChannelData
    {
        uint16_t roll;
        uint16_t pitch;
        uint16_t yaw;
        uint16_t throttle;
        uint16_t aux4;

        inline rcChannelData(const std::vector<uint16_t>& result)
        {
            roll = result[0];
            pitch = result[1];
            yaw = result[2];
            throttle = result[3];
            aux4 = result[4];
        }
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

        inline imuData(const std::vector<float>& result)
        {
            accel_x = result[0];
            accel_y = result[1];
            accel_z = result[2];
            gyro_x = result[3];
            gyro_y = result[4];
            gyro_z = result[5];
            mag_x = result[6];
            mag_y = result[7];
            mag_z = result[8];
        }
    };

    struct analogData
    {
        uint8_t batteryVoltage;
        uint16_t mAhDrawn;
        uint16_t rssi;
        uint16_t amperage;

        inline analogData(const std::vector<uint8_t>& result)
        {
            batteryVoltage = result[0];
            mAhDrawn = (result[2] << 8) | result[1];
            rssi = (result[4] << 8) | result[3];
            amperage = (result[6] << 8) | result[5];
        }
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

        explicit vtxConfigIn(const std::vector<uint8_t>& result)
        {
            vtxType = result[0];
            vtxBand = result[1];
            vtxChannel = result[2];
            vtxPower = result[3];
            vtxStatus = result[4];
            vtxFreq = (result[6] << 8) | result[5];
            vtxDeviceReady = result[7];
            vtxLowPowerDisarm = result[8];
            vtxPitFreq = (result[10] << 8) | result[9];
            vtxTableAvail = result[11];
            vtxTableBands = result[12];
            vtxTableChannels = result[13];
            vtxPowerLevels = result[14];
            padding1 = result[15];
            padding2 = result[16];
            padding3 = result[17];
            padding4 = result[18];
        }
    };
    
    inline void printVTXConfigIn(const vtxConfigIn& cfg) {
        std::cout << "VTX Config:" << std::endl;
        std::cout << "  Type: " << static_cast<int>(cfg.vtxType) << std::endl;
        std::cout << "  Band: " << static_cast<int>(cfg.vtxBand) << std::endl;
        std::cout << "  Channel: " << static_cast<int>(cfg.vtxChannel) << std::endl;
        std::cout << "  Power: " << static_cast<int>(cfg.vtxPower) << std::endl;
        std::cout << "  Status: " << static_cast<int>(cfg.vtxStatus) << std::endl;
        std::cout << "  Frequency: " << cfg.vtxFreq << std::endl;
        std::cout << "  Device Ready: " << static_cast<int>(cfg.vtxDeviceReady) << std::endl;
        std::cout << "  Low Power Disarm: " << static_cast<int>(cfg.vtxLowPowerDisarm) << std::endl;
        std::cout << "  Pit Mode Frequency: " << cfg.vtxPitFreq << std::endl;
        std::cout << "  VTX Table Available: " << static_cast<int>(cfg.vtxTableAvail) << std::endl;
        std::cout << "  VTX Table Bands: " << static_cast<int>(cfg.vtxTableBands) << std::endl;
        std::cout << "  VTX Table Channels: " << static_cast<int>(cfg.vtxTableChannels) << std::endl;
        std::cout << "  Power Levels: " << static_cast<int>(cfg.vtxPowerLevels) << std::endl;
        std::cout << "  Padding: "
                << static_cast<int>(cfg.padding1) << ", "
                << static_cast<int>(cfg.padding2) << ", "
                << static_cast<int>(cfg.padding3) << ", "
                << static_cast<int>(cfg.padding4) << std::endl;
    }


}

