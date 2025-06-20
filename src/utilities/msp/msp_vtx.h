#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

namespace MSP 
{
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

    struct vtxConfigOut
    {
        uint8_t vtxBand;
        uint8_t vtxChannel;
        uint8_t vtxPower;
        uint8_t vtxStatus;
        uint16_t vtxFreq;
        uint8_t vtxDeviceReady;
        uint8_t vtxLowPowerDisarm;
        uint16_t vtxPitFreq;
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

