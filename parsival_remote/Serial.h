//
// Created by Miguel Angel Julian Aguilar on 26/11/17.
//

#ifndef PARSIVAL_SERIAL_H
#define PARSIVAL_SERIAL_H

#include <string>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>

class Serial {
public:
    Serial(const char* serial_port) {
        std::cout << "opening" << std::endl;
        fd = open(serial_port, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
        std::cout << "opened" << std::endl;
        if (fd < 0) {
            std::cerr << "error " << errno << " opening " << serial_port << ": " << strerror(errno) << std::endl;
            error = true;
            return;
        }

        set_interface_attribs(fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
        set_blocking(fd, 0);

    }

    void writeString(const std::string& s) {
        write(fd, s.c_str(), s.size());
    }

    std::string readString(int maxNumberOfCharsToRead = 100) {
        char buf[maxNumberOfCharsToRead];
        ssize_t n = read(fd, buf, maxNumberOfCharsToRead);
        if (n) {
            return std::string(buf, n);
        } else {
            return "";
        }

    }

    bool readByte(uint8_t byte) {
        return 1 == read(fd, &byte, 1);
    }

    void writeByte(uint8_t b) {
        write(fd, &b, 1);
    }

    bool error = false;

protected:

    int set_interface_attribs (int fd, int speed, int parity) {
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0) {
            error = true;
            std::cerr << "error " << errno << " from tcgetattr" << std::endl;
            return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0) {
            std::cerr << "error " << errno << " from tcsetattr" << std::endl;
            error = true;
            return -1;
        } else {
            return 0;
        }
    }

    void set_blocking (int fd, int should_block) {
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0) {
            std::cerr << "error " << errno << " from tcgetattr" << std::endl;
            error = true;
            return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0) {
            std::cerr << "error " << errno << " setting term attributes" << std::endl;
            error = true;
        }
    }
private:
    int fd = 0;
};


#endif //PARSIVAL_SERIAL_H
