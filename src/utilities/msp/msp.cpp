#include "msp.h"
#include "msp_protocol_betaflight.h"
#include "msp_codes.h"

#include "serial.h"

#include <string>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <thread>

#define DEFAULT_BAUD 115200

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
    msp::msp(serial &mySerial)
    {
        m_serial = &mySerial;
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

        switch (cmd)
        {
            case MSP_NAME:                      // Get board name
            {
                Payload result = extractPerByte(count, buff);
                return result;
            }
            case MSP_VTX_CONFIG:                // Get VTX config
            {
                Payload result = extractPerByte(count, buff);
                return result;
            }
            case MSP_ATTITUDE:                  // Get attitude
            {
                std::vector<uint16_t> result = extractPerTwoBytes(count, buff);

                for (size_t i = 0; i < result.size(); ++i) {
                    // Cast to signed 16-bit
                    int16_t signedValue = static_cast<int16_t>(result[i]);
                    float value;

                    // Example: divide first two by 10.0 like angx, angy
                    if (i == 0 || i == 1)
                        value = static_cast<float>(signedValue) / 10.0f;
                    else
                        value = static_cast<float>(signedValue); // no division for heading or others

                    std::cout << "Value[" << i << "] = " << value << std::endl;
                }
                break;
            }
            default:
                
                return Payload();
        }
        return Payload();

    }

    void msp::getName()
    {
        Payload result = getData(MSP_NAME);
        std::cout << "Drone Name (craft name): ";
        for (size_t i = 0; i < result.size(); ++i)
        {
            std::cout << result[i];
        }
        std::cout << "\n";
    }

    vtxConfigIn msp::getVtx()
    {
        Payload result = getData(MSP_VTX_CONFIG);
        vtxConfigIn config(result);

        // printVTXConfigIn(config);
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

            std::cout << "Checking match: "
            << "config.vtxBand (" << +config.vtxBand << ") == band (" << +band << "), "
            << "config.vtxChannel (" << +config.vtxChannel << ") == channel (" << +channel << ") => "
            << std::boolalpha << match << std::endl;

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

    

}