#include "Schedulers/Timer.hpp"

namespace null {
    void Timer::start() {
        start_tp = std::chrono::steady_clock::now();
    }

    bool Timer::expired() {
        auto now_tp = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now_tp - start_tp) > max_time;
    }


    Timer::Timer(const std::chrono::milliseconds& maxTime) : max_time(maxTime) {}
}
