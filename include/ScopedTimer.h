#ifndef SCOPEDTIMER_H
#define SCOPEDTIMER_H
#include <chrono>
#include <iostream>
#include <string>


class ScopedTimer {
public:
    ScopedTimer(const std::string &label);

    ~ScopedTimer();

private:
    std::string label;
    std::chrono::high_resolution_clock::time_point start;
};



#endif //SCOPEDTIMER_H
