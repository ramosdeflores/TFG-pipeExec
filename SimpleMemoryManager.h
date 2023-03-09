// SimpleMemoryManager.h

#ifndef _SimpleMemoryManager_h_
#define _SimpleMemoryManager_h_

#include <mutex>
#include <condition_variable>
        
class Semaphore {
public:
    Semaphore (int count_ = 0)
    : count(count_) 
    {
    }
    
    inline void notify( ) {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        //notify the waiting thread
        cv.notify_one();
    }
    inline void wait( ) {
        std::unique_lock<std::mutex> lock(mtx);
        while(count == 0) {
            //wait on the mutex until notify is called
            cv.wait(lock);
        }
        count--;
    }
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
};

class SimpleMemoryManager {

	   public:

			 SimpleMemoryManager(size_t size, unsigned int poolSize);

			 ~SimpleMemoryManager();

			 void *getFreeBuffer();			// Return an empty buffer
			 void *getFullBuffer();			// Return a loaded buffer
			 int putFullBuffer(void *buffer);	// Queue a loaded buffer
			 int putFreeBuffer(void *buffer);	// Queue an empty buffer
			 int getBufferCount();			// Get the number of buffers to allocate
			 int getBufferSize();			// Get the number of buffers to allocate
			 int getFreeCount();			// Get the number of free buffers
			 int getFullCount();			// Get the number of loaded buffers
			 void waitForFull();			// Wait for data to be available
			 void waitForFree();			// Wait for a empty buffer to became available
			 void waitForDone();
			 void waitForEmpty();
			 void loadMemoryManager(void *buffer);


	   private:

			 size_t buffSize;
			 int pool_size;
			 int freeCount;
			 int fullCount;
			 int freeHead, freeTail;
			 int fullHead, fullTail;
			 void **freeQueue, **fullQueue;

			 std::mutex fullMutex_;
			 std::mutex freeMutex_;
			 std::mutex emptyMutex_;
			 Semaphore   *fullSema_;
			 Semaphore   *freeSema_;

};
#endif
