#include "msp.h"
#include "msp_protocol_betaflight.h"
#include "msp_codes.h"
#include "msp_datatypes.h"

#include "serial.h"

#include <string>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <thread>

#define DEFAULT_BAUD 115200

// #define HAS_GPS
// #define HAS_COMPASS

namespace
{
    // Extracts each byte individually as uint8_t
    MSP::Payload extractPerByte(ssize_t count, char* buff)
    {
        size_t payload_len = count - 6;  // exclude header(3), size(1), cmd(1), checksum(1)
        MSP::Payload payload(buff + 5, buff + 5 + payload_len);

        // for (size_t i = 0; i < payload.size(); ++i) {
        //     std::cout << "Byte[" << i << "] = 0x"
        //             << std::hex << static_cast<int>(payload[i])
        //             << " - " << std::dec << static_cast<int>(payload[i]) << std::endl;
        // }

        return payload;
    }

    // Extracts every 2 bytes as a uint16_t (little endian)
    std::vector<uint16_t> extractPerTwoBytes(ssize_t count, char* buff)
    {
        size_t payload_len = count - 6;
        std::vector<uint16_t> result;

        for (size_t i = 0; i + 1 < payload_len; i += 2) {
            uint8_t low = static_cast<uint8_t>(buff[5 + i]);
            uint8_t high = static_cast<uint8_t>(buff[5 + i + 1]);
            uint16_t value = static_cast<uint16_t>(high << 8 | low); // little endian
            result.push_back(value);

            // std::cout << "Pair[" << i / 2 << "] = 0x"
            //         << std::hex << value << " - "
            //         << std::dec << value << std::endl;
        }

        return result;
    }
}

namespace MSP {
    msp::msp(const std::string portName, int baudrate)
    {
        m_serial = new serial(portName, baudrate);
    }
    msp::~msp()
    {
        delete(m_serial);
    }

    void msp::sendCmd(uint8_t data_length, uint8_t code, const std::vector<uint8_t>& data) {
        Payload packet;

        packet.push_back('$');
        packet.push_back('M');
        packet.push_back('<');
        packet.push_back(data_length);
        packet.push_back(code);
        packet.insert(packet.end(), data.begin(), data.end());

        uint8_t checksum = data_length ^ code;
        for (uint8_t byte : data) checksum ^= byte;

        packet.push_back(checksum);

        if(m_serial->safeWrite(packet.data(), packet.size()))
        {
            std::cout << "Successfully Sent Command: " << MSP::getCommandName(code) << std::endl;
        }
        else
        {
            std::cout << "Unuccessfully Sent Command: " << MSP::getCommandName(code) << std::endl;
        }
    }

    Payload msp::getData(uint8_t cmd)
    {
        char buffer[MAX_RECIEVE_BUFFER];

        sendCmd(0, cmd, {});
        ssize_t bytesRead_count = m_serial->readSerial(buffer, sizeof(buffer) - 1);
        return processData(cmd, bytesRead_count, buffer);
    }

    bool msp::checkMspResponse(char* buff, ssize_t count)
    {
        if (count < 6)
        {
            std::cerr << "Frame too short\n";
            return false;
        }

        if (count < 6 || std::memcmp(buff, "$M>", 3) != 0)
        {
            std::cerr << "Invalid header: " << buff[0] << buff[1] << buff[2] << "\n";
            return false;
        }

        uint8_t payloadSize = static_cast<uint8_t>(buff[3]);
        uint8_t cmd = static_cast<uint8_t>(buff[4]);
        uint8_t receivedChecksum = static_cast<uint8_t>(buff[count - 1]);

        // Expected total count = header(3) + size(1) + cmd(1) + payload + checksum(1)
        if (count != (6 + payloadSize - 0))
        {
            std::cerr << "Payload size mismatch: expected " << static_cast<int>(payloadSize)
                    << " got " << (count - 6) << "\n";
            return false;
        }

        // Compute checksum: XOR of size, cmd, and payload
        uint8_t computedChecksum = 0;
        for (int i = 3; i < count - 1; ++i)
        {
            computedChecksum ^= static_cast<uint8_t>(buff[i]);
        }

        if (computedChecksum != receivedChecksum)
        {
            std::cerr << "Checksum mismatch: expected 0x" << std::hex << (int)receivedChecksum
                    << " computed 0x" << (int)computedChecksum << "\n";
            return false;
        }

        return true;
    }

    Payload msp::processData(uint8_t cmd, ssize_t count, char* buff)
    {
        
        if (checkMspResponse(buff, count))
        {
            std::cout << "MSP response frame is valid.\n";
        } 
        else
        {
            std::cerr << "Invalid MSP frame.\n";
        }

        Payload result = extractPerByte(count, buff);
        std::cout << "Recieved payload is of size. " << result.size() <<"\n";
        return result;
    }

    nameData msp::getName()
    {
        Payload result = getData(MSP_NAME);
        return nameData(result);
    }

    void msp::setName(const nameData new_name)
    {
        nameData current_name = msp::getName();

        bool match = (current_name.name == new_name.name);

        while(! match)
        {
            sendCmd(new_name.name.length(), MSP_SET_NAME, std::vector<uint8_t>(new_name.name.begin(), new_name.name.end()));
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            sendCmd(0, MSP_EEPROM_WRITE, {}); // Save to flash
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            m_serial->safeReset();

            current_name = msp::getName();
            match = (current_name.name == new_name.name);

            if(! match)
                std::cout << "Sent new name does not match current name: resending name" << std::endl;
        }
    }

    vtxConfigIn msp::getVtx()
    {
        Payload result = getData(MSP_VTX_CONFIG);
        vtxConfigIn config(result);
        std::cout << "\n";
        return config;
    }

    void msp::setVtx(uint8_t band, uint8_t channel) {
        uint16_t freqEncoded = ((band - 1) * 8) + (channel - 1);

        vtxConfigIn config = getVtx();

        bool match = (config.vtxBand == band) && (config.vtxChannel == channel);
        Payload payload = 
        {
            static_cast<uint8_t>(freqEncoded & 0xFF),        // low byte
            static_cast<uint8_t>((freqEncoded >> 8) & 0xFF), // high byte
            config.vtxPower,
        };

        while (! match)
        {
            sendCmd(payload.size(), MSP_SET_VTX_CONFIG, payload);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            sendCmd(0, MSP_EEPROM_WRITE, {}); // Save to flash
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            m_serial->safeReset();

            config = getVtx();
            match = (config.vtxBand == band) && (config.vtxChannel == channel);

            // std::cout << "Checking match: "
            // << "config.vtxBand (" << +config.vtxBand << ") == band (" << +band << "), "
            // << "config.vtxChannel (" << +config.vtxChannel << ") == channel (" << +channel << ") => "
            // << std::boolalpha << match << std::endl;

            if(! match)
                std::cout << "Sent vtx config does not match current vtx config: resending config" << std::endl;
        }
    }
    
    void msp::setVtx(uint16_t freq)
    {
        vtxConfigIn config = getVtx();

        bool match = (config.vtxFreq == freq);
        Payload payload = 
        {
            static_cast<uint8_t>(freq & 0xFF),        // low byte
            static_cast<uint8_t>((freq >> 8) & 0xFF), // high byte
            config.vtxPower,
        };

        while (! match)
        {
            sendCmd(payload.size(), MSP_SET_VTX_CONFIG, payload);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            sendCmd(0, MSP_EEPROM_WRITE, {}); // Save to flash
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            m_serial->safeReset();

            config = getVtx();
            match = (config.vtxFreq == freq);

            std::cout << "Checking match: "
            << "config.vtxFreq (" << +config.vtxFreq << ") == freq (" << +freq << "), "
            << std::boolalpha << match << std::endl;

            if(! match)
                std::cout << "Sent vtx config does not match current vtx config: resending config" << std::endl;
        }
    }

    void msp::getAttitude()
    {
        Payload result = getData(MSP_ATTITUDE);
        for (size_t i = 0; i + 1 < result.size(); i += 2) {
            // Combine two bytes into a signed 16-bit int (little-endian)
            int16_t signedValue = static_cast<int16_t>(
                static_cast<uint16_t>(result[i]) |
                (static_cast<uint16_t>(result[i + 1]) << 8)
            );

            float value;
            size_t index = i / 2;  // Convert byte index to value index

            // First two values (angx, angy) are scaled
            if (index == 0 || index == 1)
                value = static_cast<float>(signedValue) / 10.0f;
            else
                value = static_cast<float>(signedValue); // e.g. heading

            std::cout << "Value[" << index << "] = " << value << std::endl;
        }
    }

    rcChannelData msp::getRC()
    {
        std::vector<uint16_t> values;
        Payload result = getData(MSP_RC);
        for (size_t i = 0; i + 1 < result.size(); i += 2) {
            // Combine two bytes into an unsigned 16-bit int (little-endian)
            uint16_t value = static_cast<uint16_t>(result[i]) |
                                (static_cast<uint16_t>(result[i + 1]) << 8);

            size_t index = i / 2;  // Convert byte index to value index

            std::cout << "Value[" << index << "] = " << value << std::endl;

            values.push_back(value);
        }
        rcChannelData rcValues(values);
        return rcValues;
    }

    imuData msp::getRawIMU()
    {
        std::vector<float> values;
        Payload result = getData(MSP_RAW_IMU);
        for (size_t i = 0; i + 1 < result.size(); i += 2) {
            // Combine two bytes into a signed 16-bit int (little-endian)
            int16_t signedValue = static_cast<int16_t>(
                static_cast<uint16_t>(result[i]) |
                (static_cast<uint16_t>(result[i + 1]) << 8)
            );

            float value;
            size_t index = i / 2;  // Convert byte index to value index

#ifndef HAS_COMPASS
            if(index == 7 || index == 8 || index == 9)
            {
                continue;
            }
#endif
            value = static_cast<float>(signedValue); // e.g. heading
            values.push_back(value);
        }
        imuData imuValues(values);
        return imuValues;
    }

    analogData msp::getAnalogData()
    {
        Payload result = getData(MSP_ANALOG);
        return analogData(result);
    }

}