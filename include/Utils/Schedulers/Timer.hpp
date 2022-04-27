#pragma once

#include <chrono>

namespace null {
    class Timer {
    private:
        std::chrono::milliseconds max_time;
        std::chrono::time_point<std::chrono::steady_clock> start_tp; // Time Point
    public:
        Timer(const std::chrono::milliseconds& maxTime);

        void start();

        bool expired();
    };
}
