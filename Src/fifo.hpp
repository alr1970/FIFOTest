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
template<class T> class Fifo
{
public:
    /// Default constructor
    Fifo()
        : qCapacity(1)
    {
    };

    /// Create a Fifo with specified capacity
    Fifo(unsigned int capacity)
        : qCapacity(capacity)
    {
    };

    ~Fifo()
    {
        // Get a lock before allowing the queue to be deleted
        std::lock_guard<std::mutex> pushLock(queueMutex);
        fifoQ.clear();
    }

    // Delete copy construstor and operator because copying the underlying queue
    // and its contents doen't make sense
    Fifo(const Fifo&) = delete;
    Fifo& operator=(const Fifo&) = delete;

    // Move operator and constructor
    Fifo(Fifo&& other)
    {
        // Lock both queues
        std::lock_guard<std::mutex> otherPushLock(other.queueMutex);
        std::lock_guard<std::mutex> pushLock(queueMutex);

        fifoQ = std::move(other.fifoQ);
    }

    Fifo& operator=(Fifo&& other)
    {
        // Lock both queues
        std::lock_guard<std::mutex> otherPushLock(other.queueMutex);
        std::lock_guard<std::mutex> pushLock(queueMutex);

        fifoQ = std::move(other.fifoQ);
    }

    /// Push an item onto the stack unless it is full.
    bool push(const T& i)
    {
        std::lock_guard<std::mutex> pushLock(queueMutex);
        if(fifoQ.size() >= qCapacity) {
            return false;
        }
        fifoQ.push_back(i);
        return true;
    };

    /// Pop an item off the stack, waiting for one to be available
    T pop()
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        while(fifoQ.size() == 0) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            lock.lock();
        }
        T retVal = fifoQ.front();
        fifoQ.pop_front();
        return retVal;
    };

    /// Pop an item off the stack, returning if none is available
    std::pair<bool, T> pop_try()
    {
        std::pair<bool, T> retVal;
        std::lock_guard<std::mutex> pushLock(queueMutex);
        if(fifoQ.size() > 0) {
            retVal.first = true;
            retVal.second = fifoQ.front();
            fifoQ.pop_front();
        }
        else {
            retVal.first = false;
        }
        return retVal;
    };

private:

    std::deque<T> fifoQ; /*<! encapsulated queue */
    unsigned int qCapacity; /*<! maximum number of items that can be queued */

    std::mutex queueMutex; /*<! Mutex to protect the queue */
};

#endif
