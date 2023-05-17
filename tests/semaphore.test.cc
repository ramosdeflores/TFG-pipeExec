/**
 * @file semaphore.test.cc
 *
 * @brief This file contains the tests for the functionalities of the semaphore
 * class
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */
#include "../include/headers/semaphore.h"
#include <gtest/gtest.h>

/**
 * @test Tests the initial count of the Semaphore object
 * Creates a Semaphore object with an initial count of 5 and tests if the count
 * is correct.
 */
TEST(SemaphoreTest, InitialCount) {
    Semaphore sem(5);
    EXPECT_EQ(sem.count(), 5);
}

/**
 * @test Tests the Wait and Signal functions of the Semaphore object.
 * Creates a Semaphore object with an initial count of 0 and starts a thread
 * that waits on the semaphore and signals it. The main thread signals the
 * semaphore to start the thread and waits for it to complete. Then it tests if
 * the count of the semaphore is 1.
 */
TEST(SemaphoreTest, WaitSignal) {
    Semaphore sem(0);
    std::thread t([&sem]() {
        sem.Wait();
        sem.Signal();
    });
    sem.Signal();
    t.join();
    EXPECT_EQ(sem.count(), 1);
}

/**
 * @test Tests the Wait and Signal functions of the Semaphore object with
 * multiple threads. Creates a Semaphore object with an initial count of 0 and
 * starts 10 threads that wait on the semaphore and signal it. The main thread
 * signals the semaphore 10 times to wake up the waiting threads, waits for the
 * threads to complete, waits on the semaphore and signals it once. Then it
 * tests if the count of the semaphore is 10.
 */
TEST(SemaphoreTest, MultipleWaitSignal) {
    Semaphore sem(0);
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&sem]() {
            sem.Wait();
            sem.Signal();
        });
    }
    for (int i = 0; i < 10; i++) {
        sem.Signal();
    }
    for (auto &t : threads) {
        t.join();
    }
    sem.Wait();
    sem.Signal();
    EXPECT_EQ(sem.count(), 10);
}

/**
 * @test Tests the behavior of the Semaphore object when a Wait is called
 * without a corresponding Signal
 * Creates a Semaphore object with an initial count of 0 and starts a thread
 * that waits on the semaphore. The main thread waits for 100 milliseconds and
 * tests if the count of the semaphore is still 0. Then it signals the semaphore
 * and waits for the thread to complete.
 */
TEST(SemaphoreTest, WaitWithoutSignal) {
    Semaphore sem(0);
    std::thread t([&sem]() { sem.Wait(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(sem.count(), 0);
    sem.Signal();
    t.join();
}

/**
 * @test Tests the behavior of the Semaphore object when a Signal is called
 * without a corresponding Wait. Creates a Semaphore object with an initial
 * count of 1 and calls the Signal function twice. Then it tests if the count of
 * the semaphore is 2.
 */
TEST(SemaphoreTest, SignalWithoutWait) {
    Semaphore sem(1);
    sem.Signal();
    EXPECT_EQ(sem.count(), 2);
}

/**
 * @test Tests the behavior of the Semaphore object when multiple Signal
 * functions are called. Creates a Semaphore object with an initial count of 0
 * and calls the Signal function twice. Then it tests if the count of the
 * semaphore is 2.
 */
TEST(SemaphoreTest, MultipleSignal) {
    Semaphore sem(0);
    sem.Signal();
    sem.Signal();
    EXPECT_EQ(sem.count(), 2);
}
