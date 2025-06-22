#include "serial.h"

#include <cerrno>
#include <cstring>
#include <cstdint>
#include <chrono>
#include <thread>

serial::serial(const std::string portName, int baudrate)
{
    fd = openSerial(portName, baudrate);
    if (fd < 0) 
    {
        std::cerr << "Failed to open serial port\n";
    }
    m_portName = portName;
    m_baudrate = baudrate;
}

serial::serial()
{
    serial("", DEFAULT_BAUD);
}

serial::~serial()
{
    closeSerial();
}

int serial::openSerial(const std::string portName, int baudrate) 
{
    std::cout << portName.c_str() << std::endl;
    int fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        std::cerr << "Error in opening serial port" << portName << "': " << strerror(errno) << std::endl;
        return -1;
    }

    termios tty{};
    if (tcgetattr(fd, &tty) != 0)
    {
        std::cerr << "Error from tcgetattr" << std::endl;
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, baudrate);
    cfsetispeed(&tty, baudrate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 1;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // Disable software flow ctrl
    tty.c_cflag |= (CLOCAL | CREAD);                // Enable receiver
    tty.c_cflag &= ~(PARENB | PARODD);              // No parity
    tty.c_cflag &= ~CSTOPB;                         // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;                        // No hardware flow control

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        std::cerr << "Error from tcsetattr" << std::endl;
        close(fd);
        return -1;
    }

    return fd;
}

void serial::closeSerial() {
    close(fd);
}

int serial::getSerialFd()
{
    return fd;
}

bool serial::safeWrite(const uint8_t* data, size_t size) 
{
    if (fd < 0) 
    {
        std::cerr << "Invalid safeWrite failed\n";
        throw std::runtime_error("Invalid file descriptor, opening of serial port may have failed");
        return false;
    }
    
    ssize_t written = write(fd, data, size);
    if (written == -1) 
    {
        perror("write failed");
        return false;
    } 
    else if (static_cast<size_t>(written) != size) 
    {
        std::cerr << "Partial write\n";
        return false;
    }
    return true;
}

ssize_t serial::readSerial(void *buf, size_t count)
{
    if (fd < 0) 
    {
        std::cerr << "readSerial failed\n";
        throw std::runtime_error("Invalid file descriptor, opening of serial port may have failed");
    }
    return read(fd, buf, count);
}

void serial::safeReset()
{
    closeSerial();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    openSerial(m_portName, m_baudrate);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}