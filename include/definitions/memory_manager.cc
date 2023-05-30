/*
 * LUCID (Unified Computing Image and Data-Processing) is a program to process
 * any type of data concurrently.
 * Copyright (C) 2023 Lucas Hernández Abreu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Author:  Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

/**
 * @file memory_manager.cc
 *
 * @brief The source file for the MemoryManager class
 * @details Here all the methods of the class are defined along with the
 * security and error handling for the class
 *
 * @author Lucas Hernández Abreu
 *
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#include "../headers/memory_manager.h"
#include <cstdio>
#include <malloc.h>

/**
 * @brief Constructor for MemoryManager class
 *
 * @param mx_size The maximum number of memory buffers the queue can hold
 * @param debug The flag for the debug information
 * @throw MemoryManagerError::kBadSizing If the maximum size is less than 1
 */
MemoryManager::MemoryManager(int mx_size, bool debug)
: max_size_(mx_size), debug_(debug) {
    if (mx_size < 1) {
        throw MemoryManagerError::kBadSizing;
        return;
    }
    in_queue_ = (void **)malloc(max_size_ * sizeof(void *));
    out_queue_ = (void **)malloc(max_size_ * sizeof(void *));
    
    for (int it = 0; it < max_size_; ++it) {
        in_queue_[it] = nullptr;
    }
    rear_in_iterator_ = -1;
    rear_out_iterator_ = -1;
    
    front_in_iterator_ = 0;
    front_out_iterator_ = 0;
    in_semaphore_ =
        new Semaphore(0, Semaphore::LUCIDSemaphoreType::kIn, debug_);
    out_semaphore_ =
        new Semaphore(0, Semaphore::LUCIDSemaphoreType::kOut, debug_);
    out_queue_count_ = 0;
    in_queue_count_ = 0;
}

/**
 * @brief Destructor for MemoryManager.
 * @details Frees the buffers inside both queues and then frees the queues
 */
MemoryManager::~MemoryManager() {
    for (int it = 0; it < max_size_; ++it) {
        free(in_queue_[it]);
        free(out_queue_[it]);
    }
    free(in_queue_);
    free(out_queue_);
}

/**
 * @brief Pushes a memory buffer into the input queue.
 *
 * @param data Pointer to the memory buffer.
 *
 * @return True if the input queue is not full, false otherwise.
 */
bool
MemoryManager::PushIntoIn(void *data) {
    push_in_mtx_.lock();
    
    if (debug_) {
        printf("    %s(MemoryManager)%s Pushing into IN queue\n", LUCID_YELLOW,
               LUCID_NORMAL);
    }
    
    rear_in_iterator_ += 1;
    in_queue_[rear_in_iterator_] = data;
    
    if (rear_in_iterator_ == max_size_ - 1) {
        rear_in_iterator_ = -1;
    }
    
    in_queue_count_ += 1;
    push_in_mtx_.unlock();
    in_semaphore_->Signal();
    
    return !(in_queue_count_ == max_size_);
}

/**
 * @brief Pushes a memory buffer into the output queue.
 *
 * @param data Pointer to the memory buffer.
 *
 * @return True if the output queue is not full, false otherwise.
 */
bool
MemoryManager::PushIntoOut(void *data) {
    push_out_mtx_.lock();
    if (debug_) {
        printf("    %s(MemoryManager)%s Pushing into OUT queue\n", LUCID_YELLOW,
               LUCID_NORMAL);
    }
    rear_out_iterator_ += 1;
    out_queue_[rear_out_iterator_] = data;
    
    if (rear_out_iterator_ == max_size_ - 1) {
        rear_out_iterator_ = -1;
    }
    out_queue_count_ += 1;
    push_out_mtx_.unlock();
    out_semaphore_->Signal();
    
    return !(out_queue_count_ == max_size_);
}

/**
 * @brief Pops a memory buffer from the input queue.
 *
 * @throw Throws MemoryManagerError::kNullPtr If the content to return is null
 * (it can't be processed)
 *
 * @return Pointer to the memory buffer.
 */
void *
MemoryManager::PopFromIn() {
    pop_in_mtx_.lock();
    in_semaphore_->Wait();
    
    void *memory_buffer = nullptr;
    
    if (debug_) {
        printf("    %s(MemoryManager)%s Popping from IN Queue\n", LUCID_YELLOW,
               LUCID_NORMAL);
    }
    
    in_queue_count_ -= 1;
    memory_buffer = in_queue_[front_in_iterator_];
    in_queue_[front_in_iterator_] = nullptr;
    
    front_in_iterator_ = (front_in_iterator_ + 1) % max_size_;
    pop_in_mtx_.unlock();
    
    if (memory_buffer == nullptr) {
        throw MemoryManagerError::kNullPtr;
        return nullptr;
    }
    
    return memory_buffer;
}

/**
 * @brief Pops a memory buffer from the output queue.
 *
 * @throw MemoryManagerError::kNullPtr If the content to return is null (it
 * can't be processed)
 *
 * @return Pointer to the memory buffer.
 */
void *
MemoryManager::PopFromOut() {
    pop_out_mtx_.lock();
    out_semaphore_->Wait();
    
    void *memory_buffer = nullptr;
    
    if (debug_) {
        printf("    %s(MemoryManager)%s Popping from OUT Queue\n", LUCID_YELLOW,
               LUCID_NORMAL);
    }
    
    out_queue_count_ -= 1;
    memory_buffer = out_queue_[front_out_iterator_];
    out_queue_[front_out_iterator_] = nullptr;
    
    front_out_iterator_ = (front_out_iterator_ + 1) % max_size_;
    pop_out_mtx_.unlock();
    
    if (memory_buffer == nullptr) {
        throw MemoryManagerError::kNullPtr;
        return nullptr;
    }
    return memory_buffer;
}

/**
 * @brief Returns the maximum size of the memory buffer queues.
 *
 * @return Maximum size of the memory buffer queues.
 */
int
MemoryManager::max_size() {
    return max_size_;
}

/**
 * @brief Loads a memory buffer into the queues.
 *
 * @param data Pointer to the memory buffer.
 */
void
MemoryManager::LoadMemoryManager(void *data) {
    PushIntoOut(data);
    PushIntoIn(PopFromOut());
}

/**
 * @brief Returns the number of memory buffers in the input queue.
 *
 * @return Number of memory buffers in the input queue.
 */
int
MemoryManager::in_queue_count() {
    return in_queue_count_;
}

/**
 * @brief Returns the number of memory buffers in the output queue.
 *
 * @return Number of memory buffers in the output queue.
 */
int
MemoryManager::out_queue_count() {
    return out_queue_count_;
}

/**
 * @brief Tries to get the ownership of the cpu resources to do any action
 */
void
MemoryManager::wait_finish() {
    in_semaphore_->Wait();
}
