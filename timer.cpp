#include "timer.h"

Timer::Timer() { reset(); }

void Timer::reset() {
    running = false;
    begin = end = std::chrono::high_resolution_clock::now();
}

int Timer::start() {
    running = true;
    begin = std::chrono::high_resolution_clock::now();
    return 0;
}

int Timer::stop() {
    if (running) {
        end = std::chrono::high_resolution_clock::now();
        running = false;
    }
    return 0;
}

int Timer::result() {
    return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}