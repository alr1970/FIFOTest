#ifndef FIFO_H
#define FIFO_H
#include <queue>
#include <mutex>
#include <thread>
#include <utility>

/** \class Fifo
 *  \brief Thread-safe FIFO queue
 *
 *  Wrapper for std::queue that provides thread-safety and limits the threads
 *  capacity.
 */
template<class T> class TSFifo
{
public:
    /// Default constructor
    TSFifo()
        : qCapacity(1)
    {
    };

    /// Create a Fifo with specified capacity
    TSFifo(unsigned int capacity)
        : qCapacity(capacity)
    {
    };

    ~TSFifo()
    {
        // Get a lock before allowing the queue to be deleted
        std::lock_guard<std::mutex> pushLock(queueMutex);
        fifoQ.clear();
        itemWaiting = false;
    }

    // Delete copy constructor and operator because copying the underlying queue
    // and its contents doesn't make sense
    TSFifo(const TSFifo&) = delete;
    TSFifo& operator=(const TSFifo&) = delete;

    // Move operator and constructor
    TSFifo(TSFifo&& other)
    {
        // Lock both queues
        std::lock_guard<std::mutex> otherPushLock(other.queueMutex);
        std::lock_guard<std::mutex> pushLock(queueMutex);

        fifoQ = std::move(other.fifoQ);
        itemWaiting = other.itemWaiting;
        other.itemWaiting = false;
    }

    TSFifo& operator=(TSFifo&& other)
    {
        // Lock both queues
        std::lock_guard<std::mutex> otherPushLock(other.queueMutex);
        std::lock_guard<std::mutex> pushLock(queueMutex);

        fifoQ = std::move(other.fifoQ);
        itemWaiting = other.itemWaiting;
        other.itemWaiting = false;
    }

    /// Push an item onto the stack unless it is full.
    bool push(const T& i)
    {
        std::lock_guard<std::mutex> pushLock(queueMutex);
        if(fifoQ.size() >= qCapacity) {
            return false;
        }
        fifoQ.push_back(i);
        itemWaiting = true;
        return true;
    };

    /// Pop an item off the stack, waiting for one to be available
    T pop()
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        while(!itemWaiting) {
            lock.unlock();
            // Allow other threads to get in and push items
            lock.lock();
        }
        T retVal = fifoQ.front();
        fifoQ.pop_front();
        if(fifoQ.size() == 0) {
            itemWaiting = false;
        }
        return retVal;
    };

    /// Pop an item off the stack, returning if none is available
    std::pair<bool, T> pop_try()
    {
        std::pair<bool, T> retVal;
        std::lock_guard<std::mutex> pushLock(queueMutex);
        if(itemWaiting) {
            retVal.first = true;
            retVal.second = fifoQ.front();
            fifoQ.pop_front();
            if(fifoQ.size() == 0) {
                itemWaiting = false;
            }
        }
        else {
            retVal.first = false;
        }
        return retVal;
    };

private:

    std::deque<T> fifoQ; /*<! encapsulated queue */
    unsigned int qCapacity; /*<! maximum number of items that can be queued */
    bool itemWaiting = false;

    std::mutex queueMutex; /*<! Mutex to protect the queue */
};

#endif
