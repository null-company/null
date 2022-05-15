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
         * Invokes processor iff the queue has any elements, and sometimes clears the queue
         * @param processor
         */
        void processMessageIfAny(std::function<void(T&)> processor) {
            constexpr uint32_t queueSizeThreshold = 1000;
            if (storage == nullptr) {
                throw std::logic_error("storage not set");
            }
            if (!storage->empty()) {
                int32_t lastIndex = storage->size() - 1;
                if (lastIndex > lastProcessedIndex) {
                    auto lastStateMessage = storage->back();
                    processor(lastStateMessage);
                    lastProcessedIndex = lastIndex;
                }
                if (storage->size() >= queueSizeThreshold) {
                    std::queue<T> emptyQueue;
                    std::swap(*storage, emptyQueue);
                    lastProcessedIndex = -1;
                }
            }
        }

    private:
        std::queue<T>* storage{};
        int32_t lastProcessedIndex{};
    };
}
