#pragma once

#include <queue>
#include <stdexcept>
#include <functional>

namespace null {
    template<typename T>
    class LastAcceptedMessageProcessor {
    public:
        LastAcceptedMessageProcessor() = default;

        void attachTo(std::queue<T>* queue) {
            storage = queue;
        }

        /**
         * Invokes processor iff the queue has any elements, and leaves the queue empty
         * @param processor
         */
        void processMessageIfAny(std::function<void(T&)> processor) {
            if (storage == nullptr) {
                throw std::logic_error("storage not set");
            }
            if (!storage->empty()) {
                auto lastStateMessage = storage->back();
                std::queue<T> emptyQueue;
                std::swap(*storage, emptyQueue);
                processor(lastStateMessage);
            }
        }

    private:
        std::queue<T>* storage{};
    };
}
