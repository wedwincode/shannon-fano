#include "../include/ScopedTimer.h"

ScopedTimer::ScopedTimer(const std::string &label): label(label), start(std::chrono::high_resolution_clock::now()) {
}

ScopedTimer::~ScopedTimer() {
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "[" << label << "] Time: " << duration << " ms\n";
}
