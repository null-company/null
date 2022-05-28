#pragma once

#include <queue>
#include <stdexcept>
#include <functional>

namespace null {
    template<typename T>
    class EveryAcceptedMessageProcessor {
    public:
        EveryAcceptedMessageProcessor() = default;

        void attachTo(std::queue<T>* queue) {
            storage = queue;
        }

        /**
         * Invokes processor iff the queue has any elements, uses this processor for every message
         * in order in which they came
         * @param processor
         */
        void processMessageIfAny(std::function<void(T&)> processor) {
            if (storage == nullptr) {
                throw std::logic_error("storage not set");
            }
            while (!storage->empty()) {
                auto message = storage->front();
                storage->pop();
                processor(message);
            }
        }

    private:
        std::queue<T>* storage{};
    };
}
