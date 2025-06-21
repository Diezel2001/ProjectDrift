#include "config.h"
#include "serial.h"
#include "msp.h"

#include <iostream>


int main()
{
    auto path = CONFIG_FILE_PATH;
    Config config(path);

    serial fc_uart(config.getSerialPort(), config.getBaudRate());

    MSP::msp msp(fc_uart);


    msp.sendMspCmd(MSP_NAME);
    std::cout << "\n";
    msp.sendMspCmd(MSP_ATTITUDE);
    std::cout << "\n";
    msp.sendMspCmd(MSP_VTX_CONFIG);


    return 0;
}