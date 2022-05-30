#pragma once

#include <queue>
#include <stdexcept>
#include <functional>

namespace null {
    /**
     * Use this utility class for receiving commands or generally any kind of
     * messages which you cannot ignore
     * @tparam T message type
     */
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

        /**
         * Ignores all messages, clears the queue
         */
        void clear() {
            std::queue<T> emptyQueue;
            std::swap(*storage, emptyQueue);
        }

    private:
        std::queue<T>* storage{};
    };
}
