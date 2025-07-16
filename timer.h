#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer();
    void reset();
    int start();
    int stop();
    int result();
private:
    bool running;
    std::chrono::high_resolution_clock::time_point begin, end;
};

#endif
#pragma once
