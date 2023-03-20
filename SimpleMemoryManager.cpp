#include "SimpleMemoryManager.h"
#include <iostream>

SimpleMemoryManager::SimpleMemoryManager(const size_t size, unsigned int poolSize) {
	int index, err_index;

	pool_size = poolSize;

	fullQueue = (void **)malloc(pool_size * sizeof(void *));
	freeQueue = (void **)malloc(pool_size * sizeof(void *));

	for ( index = 0; index < pool_size; ++index) {
		fullQueue[index] = NULL;
		// If size is 0 then the freeQueue be populated with NULLs and
		// will be the responsibility of the calling program to populate the queue
		// with storage.
		if ( size == 0 )
			freeQueue[index] = NULL;
		else {
			// If size is non 0 then "pool_size" buffers of size "size" will be allocated 
			if ((freeQueue[index] = (void *)malloc(size)) == NULL) {
				std::cout << "SimpleMemoryManager::SimpleMemoryManager() - ERROR allocating memory" << std::endl;
				// If allocation fails then free any previously allocated buffers
				for ( err_index = 0; err_index < index; ++err_index)
					free(freeQueue[index]);
				pool_size = -1;
				// Need to send error up
			}
		}
	}

	buffSize = size;
	freeHead = pool_size - 1;
	fullHead = 0;
	fullTail = 0;
	freeTail = 0;
	freeCount = pool_size;
	fullCount = 0;

	freeSema_ = new Semaphore(pool_size - 1);
	fullSema_ = new Semaphore(0);
}

SimpleMemoryManager::~SimpleMemoryManager() {
	int index;

	// If the buffers should be preserved after the object is deleted, then
	// the queues should be refilled with NULLs.
	if ( pool_size != -1 ){
		for ( index = freeHead; index < freeTail; ++index)
			if ( freeQueue[index] != NULL )
				free(freeQueue[index]);

		for ( index = fullHead; index < fullTail; ++index)
			if ( fullQueue[index] != NULL )
				free(fullQueue[index]);

		free(fullQueue);
		free(freeQueue);
	}

}


void *SimpleMemoryManager::getFreeBuffer() {
	void *buffer = NULL;

	freeMutex_.lock();
	--freeCount;
	buffer = freeQueue[freeTail];
	freeQueue[freeTail] = NULL;
	freeTail = ( freeTail + 1 ) % pool_size;
	freeMutex_.unlock();

	return buffer;

}

void *SimpleMemoryManager::getFullBuffer() {
	void *buffer = NULL;


	fullMutex_.lock();
	--fullCount;
	buffer = fullQueue[fullTail];
	fullQueue[fullTail] = NULL;
	fullTail = ( fullTail + 1 ) % pool_size;
	fullMutex_.unlock();

	return buffer;
}

int SimpleMemoryManager::putFreeBuffer(void *buffer) {

	freeMutex_.lock();

	//	if ( ! freeCount )
	//		emptyMutex_.acquire();

	++freeCount;
	freeQueue[freeHead] = buffer;
	freeHead = (freeHead + 1) % pool_size;

	freeMutex_.unlock();

	freeSema_->notify();

	return pool_size - freeCount;
}

int SimpleMemoryManager::putFullBuffer(void *buffer) {


	fullMutex_.lock();

	++fullCount;
	fullQueue[fullHead] = buffer;
	fullHead = (fullHead + 1) % pool_size;

	fullMutex_.unlock();

	fullSema_->notify();

	return pool_size - fullCount;
}

int SimpleMemoryManager::getBufferCount() {

	return pool_size;
}

int SimpleMemoryManager::getFreeCount() {

	return freeCount;
}

int SimpleMemoryManager::getFullCount() {

	return fullCount;
}

int SimpleMemoryManager::getBufferSize() {

	return buffSize;
}

void SimpleMemoryManager::waitForFull() {

	fullSema_->wait();
}

void SimpleMemoryManager::waitForFree() {

	freeSema_->wait();
}

// Wait for the free queue to be the same as the buffer count, in which case
// there is no buffers are being processed
void SimpleMemoryManager::waitForDone()
{
	while ( getFreeCount() != getBufferCount() ) {
//		std::cout << "getFreeCount() = " << getFreeCount() << " getBufferCount() = " << getBufferCount() << std::endl;
		waitForFree();
	}
}

// Block until full queue is empty
void SimpleMemoryManager::waitForEmpty() {

	emptyMutex_.lock();
	emptyMutex_.unlock();
}

// Load the memory manager with user provided buffers

void SimpleMemoryManager::loadMemoryManager(void *buffer) {
	waitForFree();
	getFreeBuffer();
	putFullBuffer(buffer);
	waitForFull();
	putFreeBuffer(getFullBuffer());
}
