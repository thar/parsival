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

class Serial {
public:
    Serial(const char* serial_port) {
        std::cout << "opening" << std::endl;
        fd = open(serial_port, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
        std::cout << "opened" << std::endl;
        if (fd < 0) {
            std::cout << "error " << errno << " opening " << serial_port << ": " << strerror(errno) << std::endl;
            error = true;
            return;
        }

        std::cout << "set speed" << std::endl;
        set_interface_attribs(fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
        std::cout << "set blocking" << std::endl;
        set_blocking(fd, 0);                // set no blocking
        std::cout << "end" << std::endl;

    }

    void writeString(const std::string& s) {
        write(fd, s.c_str(), s.size());

        usleep((7 + 25) * 10000);             // sleep enough to transmit the 7 plus
        // receive 25:  approx 100 uS per char transmit
        char buf[100];
        int n = read(fd, buf, sizeof buf);  // read up to 100 characters if ready to read
        std::cout << "leido: " << buf << std::endl;
    }

    void writeString(uint8_t b) {
        write(fd, &b, 1);

        usleep((7 + 25) * 10000);             // sleep enough to transmit the 7 plus
        // receive 25:  approx 100 uS per char transmit
        char buf[100];
        int n = read(fd, buf, sizeof buf);  // read up to 100 characters if ready to read
        std::cout << "leido: " << std::to_string(buf[0]) << std::endl;
    }

    bool error = false;

protected:

    int
    set_interface_attribs (int fd, int speed, int parity)
    {
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
            error = true;
            std::cout << "error " << errno << " from tcgetattr" << std::endl;
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

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
            std::cout << "error " << errno << " from tcsetattr" << std::endl;
            error = true;
            return -1;
        }
        return 0;
    }

    void
    set_blocking (int fd, int should_block)
    {
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
            std::cout << "error " << errno << " from tcgetattr" << std::endl;
            error = true;
            return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0) {
            std::cout << "error " << errno << " setting term attributes" << std::endl;
            error = true;
        }
    }
private:
    int fd = 0;
};


#endif //PARSIVAL_SERIAL_H
