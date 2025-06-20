#pragma once

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstdint>

#define DEFAULT_BAUD 115200

class serial
{
public:
    serial(const std::string portName, int baudrate);
    serial();
    ~serial();

    int openSerial(const std::string portName, int baudrate);
    void closeSerial();

    int getSerialFd();
    bool safeWrite(const uint8_t* data, size_t size);
    ssize_t readSerial(void *buf, size_t count);
    void safeReset();
    
private:
    int fd;
    std::string m_portName;
    int m_baudrate;

};