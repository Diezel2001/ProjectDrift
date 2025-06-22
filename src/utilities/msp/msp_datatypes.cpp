#include "msp_datatypes.h"

#include <iostream>
#include <cstdint>
#include <vector>
#include <iomanip>

namespace MSP 
{
    nameData::nameData(const std::vector<uint8_t>& result)
    {
        name = std::string(result.begin(), result.end());
    }

    void nameData::print()
    {
        std::cout << "Drone Name (Craft Name)   : " << name << std::endl;
    }

    rcChannelData::rcChannelData(const std::vector<uint16_t>& result)
    {
        roll = result[0];
        pitch = result[1];
        yaw = result[2];
        throttle = result[3];
        aux4 = result[4];
    }

    void rcChannelData::print()
    {
        std::cout << std::fixed << std::setprecision(0);
        std::cout << "RC Channels:\n";
        std::cout << "  Roll     : " << roll     << "\n";
        std::cout << "  Pitch    : " << pitch    << "\n";
        std::cout << "  Yaw      : " << yaw      << "\n";
        std::cout << "  Throttle : " << throttle << "\n";
        std::cout << "  Aux4     : " << aux4     << "\n";
    }

    imuData::imuData(const std::vector<float>& result)
    {
        accel_x = result[0];
        accel_y = result[1];
        accel_z = result[2];
        gyro_x = result[3];
        gyro_y = result[4];
        gyro_z = result[5];

        if (result.size() != 6)
        {
            mag_x = result[6];
            mag_y = result[7];
            mag_z = result[8];
        }
    }

    void imuData::print()
    {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "IMU Data:" << std::endl;
        std::cout << "  Accel [g]  -> X: " << accel_x << "  Y: " << accel_y << "  Z: " << accel_z << std::endl;
        std::cout << "  Gyro  [°/s]-> X: " << gyro_x  << "  Y: " << gyro_y  << "  Z: " << gyro_z  << std::endl;
        std::cout << "  Mag   [uT] -> X: " << mag_x   << "  Y: " << mag_y   << "  Z: " << mag_z   << std::endl;
    }

    analogData::analogData(const std::vector<uint8_t>& result)
    {
        batteryVoltage = result[0];
        mAhDrawn = (result[2] << 8) | result[1];
        rssi = (result[4] << 8) | result[3];
        amperage = (result[6] << 8) | result[5];
    }

    void analogData::print()
    {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Analog Data:" << std::endl;
        std::cout << "  Battery Voltage: " << static_cast<float>(batteryVoltage) / 10.0 << " V" << std::endl;
        std::cout << "  mAh Drawn:       " << mAhDrawn << " mAh" << std::endl;
        std::cout << "  RSSI:            " << rssi << " / 1023" << std::endl;
        std::cout << "  Amperage:        " << static_cast<float>(amperage) / 100.0 << " A" << std::endl;
    }

    vtxConfigIn::vtxConfigIn(const std::vector<uint8_t>& result)
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
    
    void vtxConfigIn::print() {
        std::cout << "VTX Config:" << std::endl;
        std::cout << "  Type                : " << static_cast<int>(vtxType) << std::endl;
        std::cout << "  Band                : " << static_cast<int>(vtxBand) << std::endl;
        std::cout << "  Channel             : " << static_cast<int>(vtxChannel) << std::endl;
        std::cout << "  Power               : " << static_cast<int>(vtxPower) << std::endl;
        std::cout << "  Status              : " << static_cast<int>(vtxStatus) << std::endl;
        std::cout << "  Frequency           : " << vtxFreq << std::endl;
        std::cout << "  Device Ready        : " << static_cast<int>(vtxDeviceReady) << std::endl;
        std::cout << "  Low Power Disarm    : " << static_cast<int>(vtxLowPowerDisarm) << std::endl;
        std::cout << "  Pit Mode Frequency  : " << vtxPitFreq << std::endl;
        std::cout << "  VTX Table Available : " << static_cast<int>(vtxTableAvail) << std::endl;
        std::cout << "  VTX Table Bands     : " << static_cast<int>(vtxTableBands) << std::endl;
        std::cout << "  VTX Table Channels  : " << static_cast<int>(vtxTableChannels) << std::endl;
        std::cout << "  Power Levels        : " << static_cast<int>(vtxPowerLevels) << std::endl;
    }


}

