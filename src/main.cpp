#include "config.h"
#include "serial.h"
#include "msp.h"

#include <iostream>


int main()
{
    auto path = CONFIG_FILE_PATH;
    Config config(path);

    MSP::msp msp(config.getSerialPort(), config.getBaudRate());


    MSP::imuData data = msp.getRawIMU();

    data.print
();

    return 0;
}