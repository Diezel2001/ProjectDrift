#include "msp.h"
#include "msp_protocol_betaflight.h"
#include "msp_codes.h"
#include "msp_vtx.h"

#include "serial.h"

#include <string>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <cstring>

#define DEFAULT_BAUD 115200

namespace
{
    // Extracts each byte individually as uint8_t
    std::vector<uint8_t> extractPerByte(ssize_t count, char* buff)
    {
        size_t payload_len = count - 6;  // exclude header(3), size(1), cmd(1), checksum(1)
        std::vector<uint8_t> payload(buff + 5, buff + 5 + payload_len);

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
        std::vector<uint8_t> packet;

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

    void msp::getData(uint8_t cmd)
    {
        char buffer[MAX_RECIEVE_BUFFER];

        sendCmd(0, cmd, {});
        ssize_t bytesRead_count = m_serial->readSerial(buffer, sizeof(buffer) - 1);

        processData(cmd, bytesRead_count, buffer);

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

    void msp::processData(uint8_t cmd, ssize_t count, char* buff)
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
                std::vector<uint8_t> result = extractPerByte(count, buff);

                std::cout << "Drone Name (craft name): ";
                for (size_t i = 0; i < result.size(); ++i)
                {
                    std::cout << result[i];
                }
                std::cout << "\n";
                break;
            }

            // === Cleanflight Original Features (Getters Only) ===
            case MSP_BATTERY_CONFIG:            // Get battery configuration
            case MSP_MODE_RANGES:               // Get mode ranges
            case MSP_FEATURE_CONFIG:            // Get feature configuration
            case MSP_BOARD_ALIGNMENT_CONFIG:    // Get board alignment
            case MSP_CURRENT_METER_CONFIG:      // Get current meter configuration
            case MSP_MIXER_CONFIG:              // Get mixer configuration
            case MSP_RX_CONFIG:                 // Get RX configuration
            case MSP_LED_COLORS:                // Get LED colors
            case MSP_LED_STRIP_CONFIG:          // Get LED strip configuration
            case MSP_RSSI_CONFIG:               // Get RSSI configuration
            case MSP_ADJUSTMENT_RANGES:         // Get adjustment ranges
            case MSP_PID_CONTROLLER:            // Get PID controller
            case MSP_ARMING_CONFIG:             // Get arming configuration
                break;

            // === Baseflight Commands (Getters Only) ===
            case MSP_RX_MAP:                    // Get RX map
            case MSP_DATAFLASH_SUMMARY:         // Get dataflash summary
            case MSP_DATAFLASH_READ:            // Read dataflash
            case MSP_FAILSAFE_CONFIG:           // Get failsafe config
            case MSP_RXFAIL_CONFIG:             // Get RX failsafe
            case MSP_SDCARD_SUMMARY:            // Get SD card summary
            case MSP_BLACKBOX_CONFIG:           // Get blackbox config
            case MSP_TRANSPONDER_CONFIG:        // Get transponder config
            case MSP_OSD_CONFIG:                // Get OSD config
            case MSP_OSD_CHAR_READ:             // Read OSD chars
            case MSP_VTX_CONFIG:                // Get VTX config
            {
                std::vector<uint8_t> result = extractPerByte(count, buff);
                vtxconfig config{result[0], result[1], result[2], result[3], result[4],
                                static_cast<uint16_t>((static_cast<uint16_t>(result[6]) << 8) | result[5]),
                                result[7], result[8],
                                static_cast<uint16_t>((static_cast<uint16_t>(result[10]) << 8) | result[9]),
                                result[11], result[12], result[13], result[14],
                                result[15], result[16], result[17], result[18]};

                printVTXConfig(config);
                std::cout << "\n";
                break;
            }

            // === Betaflight Additional Getters (90–99) ===
            case MSP_ADVANCED_CONFIG:           // Get advanced config
            case MSP_FILTER_CONFIG:             // Get filter config
            case MSP_PID_ADVANCED:              // Get advanced PID
            case MSP_SENSOR_CONFIG:             // Get sensor config
                break;

            // === Multiwii Original (101–139) ===
            case MSP_STATUS:                    // Get system status
            case MSP_RAW_IMU:                   // Get raw IMU
            case MSP_SERVO:                     // Get servo data
            case MSP_MOTOR:                     // Get motor data
            case MSP_RC:                        // Get RC values
            case MSP_RAW_GPS:                   // Get raw GPS
            case MSP_COMP_GPS:                  // Get GPS comparison
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
            case MSP_ALTITUDE:                  // Get altitude
            case MSP_ANALOG:                    // Get analog readings
            case MSP_RC_TUNING:                 // Get RC tuning
            case MSP_PID:                       // Get PIDs
            case MSP_BOXNAMES:                  // Get mode names
            case MSP_PIDNAMES:                  // Get PID names
            case MSP_WP:                        // Get waypoint
            case MSP_BOXIDS:                    // Get box IDs
            case MSP_SERVO_CONFIGURATIONS:      // Get servo configs
            case MSP_NAV_STATUS:                // Get nav status
            case MSP_NAV_CONFIG:                // Get nav config
            case MSP_MOTOR_3D_CONFIG:           // Get 3D motor config
            case MSP_RC_DEADBAND:               // Get deadband
            case MSP_SENSOR_ALIGNMENT:          // Get sensor alignment
            case MSP_LED_STRIP_MODECOLOR:       // Get LED strip mode-color
            case MSP_VOLTAGE_METERS:            // Get voltage meter data
            case MSP_CURRENT_METERS:            // Get current meter data
            case MSP_BATTERY_STATE:             // Get battery state
            case MSP_MOTOR_CONFIG:              // Get motor config
            case MSP_GPS_CONFIG:                // Get GPS config
            case MSP_COMPASS_CONFIG:            // Get compass config
            case MSP_ESC_SENSOR_DATA:           // Get ESC sensor data
            case MSP_GPS_RESCUE:                // Get GPS rescue params
            case MSP_GPS_RESCUE_PIDS:           // Get GPS rescue PID
            case MSP_VTXTABLE_BAND:             // Get VTX table band
            case MSP_VTXTABLE_POWERLEVEL:       // Get VTX power levels
            case MSP_MOTOR_TELEMETRY:           // Get motor telemetry
                break;

            // === Additional Non-MultiWii Commands (Getters Only) ===
            case MSP_STATUS_EX:
            case MSP_UID:
            case MSP_GPSSVINFO:
            case MSP_GPSSTATISTICS:

            default:
                // Unknown or unsupported command
                break;
        }

    }

}