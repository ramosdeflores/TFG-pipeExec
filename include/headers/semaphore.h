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
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Author:  Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

/**
 * @file semaphore.h
 *
 * @brief This file contains the declaration of the Semaphore class, which
 * provides a synchronization mechanism for concurrent access to a shared
 * resource.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "libraries.h"

/**
 * @class Semaphore
 * @brief A class that implements a semaphore
 *
 * This class implements a semaphore, which is a synchronization mechanism used
 * to protect shared resources or to synchronize threads. It provides two basic
 * operations, Wait and Signal.
 */
class Semaphore {
    
    public:
    /**
     * @enum LUCIDSemaphoreType
     * @brief Enumerated type for possible semaphore types
     *
     * There are three types of Semaphore:
     * - kNone (No type)
     * - kIn (IN queue Semaphore)
     * - kOut (OUT queue Semaphore)
     */
    enum LUCIDSemaphoreType {
        kNone, /**< Standard semaphore type if not assigned */
        kIn,   /**< Semaphore for the IN queue inside the MemoryManager */
        kOut,  /**< Semaphore for the OUT queue inside the MemoryManager */
    };
    
    // Constructs a new Semaphore object
    Semaphore(int = 0, LUCIDSemaphoreType = LUCIDSemaphoreType::kNone,
              bool = false);
    
    // Destroys the Semaphore object
    ~Semaphore();
    
    // This function blocks the calling thread until the semaphore count is
    // greater than zero.
    void Wait();
    
    // This function increments the semaphore count and wakes up one waiting
    // thread, if there is any.
    void Signal();
    
    // Returns the current count of the semaphore
    int count();
    
    private:
    std::atomic<int> count_;     /**< The count of the semaphore */
    std::mutex mtx_;             /**< The mutex used to protect the semaphore */
    std::condition_variable cv_; /**< The condition variable used for waiting */
    std::string type_; /**< Name of the type of the Semaphore after processing
                          the LUCIDSemaphoreType in the constuctor */
    bool debug_; /**< Debug flag for showing the information on each Semaphore
                    operation */
};

#endif
