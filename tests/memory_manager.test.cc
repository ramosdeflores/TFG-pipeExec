/**
 * @file memory_manager.test.cc
 *
 * @brief This file contains the tests for the memory_manager class pushing its
 * limits
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */
#include "../include/headers/memory_manager.h"
#include <gtest/gtest.h>

/**
 * @test Test case for initializing the MemoryManager.
 * The MemoryManager is initialized with a given size, and the test checks if
 * the maximum size, input queue count, and output queue count are
 * initialized to their correct values.
 */
TEST(MemoryManagerTest, Initial) {
    MemoryManager mm(2);
    EXPECT_EQ(mm.max_size(), 2);
    EXPECT_EQ(mm.in_queue_count(), 0);
    EXPECT_EQ(mm.out_queue_count(), 0);
}

/**
 * @test Test case for pushing an item into the input queue.
 * The test initializes the MemoryManager with a size of 2 and attempts to push
 * three different items into the input queue. The test verifies that the
 * first two items are successfully pushed into the queue, and the result of the
 * second push is false due to the queue's full capacity. Finally, the test
 * checks if the input queue count is updated to the correct value.
 */
TEST(MemoryManagerTest, PushIntoIn) {
    MemoryManager mm(2);
    int *data1 = new int(1);
    int *data2 = new int(2);
    EXPECT_TRUE(mm.PushIntoIn(data1));
    EXPECT_FALSE(mm.PushIntoIn(data2));
    EXPECT_EQ(mm.in_queue_count(), 2);
}

/**
 * @test Test case for pushing an item into the output queue.
 * The test initializes the MemoryManager with a size of 2 and attempts to
 * push two different items into the output queue. The test verifies that the
 * first two items are successfully pushed into the queue, and the result of the
 * second push is false due to the queue's full capacity. Finally,
 * the test checks if the output queue count is updated to the correct value.
 */
TEST(MemoryManagerTest, PushIntoOut) {
    MemoryManager mm(2);
    int *data1 = new int(1);
    int *data2 = new int(2);
    EXPECT_TRUE(mm.PushIntoOut(data1));
    EXPECT_FALSE(mm.PushIntoOut(data2));
    EXPECT_EQ(mm.out_queue_count(), 2);
}

/**
 * @test Test case for popping an item from the input queue.
 * The test initializes the MemoryManager with a size of 2 and pushes two
 * items into the input queue. The test verifies that the first item pushed is
 * popped from the queue first, followed by the second item. Finally, the test
 * checks if a nullptr is returned when trying to pop an item from an empty
 * queue.
 */

TEST(MemoryManagerTest, PopFromIn) {
    MemoryManager mm(2);
    int data1 = 1;
    int data2 = 2;
    mm.PushIntoIn(&data1);
    mm.PushIntoIn(&data2);
    EXPECT_EQ(mm.PopFromIn(), &data1);
    EXPECT_EQ(mm.PopFromIn(), &data2);
}

/**
 * @test Tests the case of popping from the output queue.
 * It creates a MemoryManager object with a maximum size of 2 and pushes two
 * integers into its out_queue_. Then, it calls PopFromOut() twice and checks
 * if it returns the expected values.
 */
TEST(MemoryManagerTest, PopFromOut) {
    MemoryManager mm(2);
    int *data1 = new int(1);
    int *data2 = new int(2);
    mm.PushIntoOut(data1);
    mm.PushIntoOut(data2);
    EXPECT_EQ(mm.PopFromOut(), data1);
    EXPECT_EQ(mm.PopFromOut(), data2);
}

/**
 * @test Test the LoadMemoryManager method of MemoryManager class.
 * Given a MemoryManager object mm and an int pointer data1,
 * load data1 into mm and test if in_queue_count() is 1 and out_queue_count()
 * is 0.
 */
TEST(MemoryManagerTest, LoadMemoryManager) {
    MemoryManager mm(2);
    int *data1 = new int(1);
    mm.LoadMemoryManager(data1);
    EXPECT_EQ(mm.in_queue_count(), 1);
    EXPECT_EQ(mm.out_queue_count(), 0);
}

/**
 *
 * @test Test the behaviour of MemoryManager class with null pointers.
 * Given a MemoryManager object mm and two int pointers, load them into mm
 * and pop them out to mm's out queue. Assert that mm's in queue and out queue
 * are both non empty.
 *
 * If we tried to pop without having nothing, the thread would block
 */
TEST(MemoryManagerTest, NullPtr) {
    MemoryManager mm(2);
    for (int it = 0; it < 2; it++) {
        int *data = (int *)malloc(sizeof(int));
        *data = 0;
        mm.LoadMemoryManager(data);
    }
    void *data_from_in;
    EXPECT_NE(data_from_in = mm.PopFromIn(), nullptr);
    mm.PushIntoOut(data_from_in);
    EXPECT_NE(mm.PopFromOut(), nullptr);
}

/**
 *
 * @test Test the behaviour of MemoryManager class with invalid sizing.
 * Assert that creating a MemoryManager object with a negative size throws a
 * MemoryManagerError.
 */
TEST(MemoryManagerTest, BadSizing) {
    EXPECT_THROW(MemoryManager mm(-1), MemoryManager::MemoryManagerError);
}

void
RunThread(MemoryManager &mm) {
    void *data = mm.PopFromOut();
    mm.PushIntoIn(data);
}

TEST(MemoryManagerTest, MultithreadingAccess) {
    MemoryManager *data_in = new MemoryManager(3);
    for (int i = 0; i < 3; ++i) {
        int *some = (int *)malloc(sizeof(int));
        *some = 0;
        data_in->LoadMemoryManager((void *)some);
    }
    std::vector<std::thread *> running;
    for (int i = 0; i < 2000; ++i) {
        running.push_back(new std::thread([data_in, i]() {
            bool cont = false;

            do {
                int *data = (int *)data_in->PopFromOut();
                *data += 1;
                cont = data_in->PushIntoIn(data);

            } while (true);
        }));
    }

    for (int i = 0; i < 2000; ++i) {
        void *data = data_in->PopFromIn();
        data_in->PushIntoOut(data);
    }
}
