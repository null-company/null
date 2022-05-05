#include "Schedulers/LinearScheduler.hpp"

namespace null {
    void LinearScheduler::start() {
        start_tp = std::chrono::steady_clock::now();
        end_tp = start_tp;
        started = true;
    }

    void LinearScheduler::end() {
        end_tp = std::chrono::steady_clock::now();
        started = false;
    }

    bool LinearScheduler::isStarted() const {
        return started;
    }

    float LinearScheduler::getValue() {
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(start_tp - end_tp);
        if (duration > max_time) {
            return to;
        }
        float percentage = static_cast<float>(duration.count()) / static_cast<float>(max_time.count());
        return (from - to) * percentage + from;
    }

    LinearScheduler::LinearScheduler(float from, float to, const std::chrono::milliseconds& maxTime) : from(from),
                                                                                                       to(to), max_time(
                    maxTime) {}


}
