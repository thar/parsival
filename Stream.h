//
// Created by Miguel Angel Julian Aguilar on 12/11/17.
//

#ifndef PARSIVAL_STREAM_H
#define PARSIVAL_STREAM_H


#include <string>
#include <iostream>

#include <memory>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>

class Stream {
public:
    Stream(bool readInput = true) : readInput(readInput) {
        if (readInput) {
            readingThread = std::thread(&Stream::readStdin, this);
        }
    }

    ~Stream() {
        if (readInput) {
            readingThread.join();
        }
    }

    template <typename T>
    void println(T s) { std::cout << s << std::endl; }

    template <typename T>
    void print(T s) { std::cout << s; }

    void readStdin() {
        std::string s;
        while(!exitProgram && std::getline(std::cin, s, '\n'))
        {
            auto lock = std::unique_lock<std::mutex>(m);
            if (s == "exit") {
                exitProgram = true;
            } else {
                new_string = std::move(s);
            }
            lock.unlock();
        }
    }

    bool available() {
        auto lock = std::unique_lock<std::mutex>(m);
        if (new_string.size() > 0) {
            inputBuffer += new_string;
            new_string = "";
        }
        lock.unlock();

        return inputBuffer.size() > 0;
    }

    char read() {
        char a = inputBuffer.front();
        inputBuffer = std::string(inputBuffer.begin() + 1, inputBuffer.end());
        return a;
    }

    bool exit() {
        return exitProgram;
    }
protected:
    bool readInput;
    std::mutex m;
    std::string new_string;
    std::thread readingThread;
    std::string inputBuffer;
    bool exitProgram = false;

private:
};


#endif //PARSIVAL_STREAM_H
