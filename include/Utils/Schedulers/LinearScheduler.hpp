#pragma once

#include <chrono>

namespace null {
    class LinearScheduler {
    private:
        bool started = false;
        float from, to;
        std::chrono::milliseconds max_time;
        std::chrono::time_point<std::chrono::steady_clock> start_tp, end_tp; // Time Point

    public:
        LinearScheduler(float from,
                        float to,
                        const std::chrono::milliseconds& maxTime = std::chrono::milliseconds{2000});

        void start();

        void end();

        float getValue();

        [[nodiscard]]
        bool isStarted() const;
    };
}
