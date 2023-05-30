/*
 * LUCID (Unified Computing Image and Data-Processing) is a program to process
 * any type of data concurrently. Copyright (C) 2023 Lucas Hernández Abreu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
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

#include "processing_units.h"

int SleeperMain(bool debug_flag, bool pu_debug_flag) {
    Sleeper sleeper_unit;
    MemoryManager *data_in = new MemoryManager(3, debug_flag);
    for (int i = 0; i < 3; i += 1){
        int* holder = (int*)malloc(sizeof(int));
        data_in->LoadMemoryManager(holder);
    }
    
    Pipeline* pipe = new Pipeline(&sleeper_unit, data_in, 3, debug_flag);
    pipe->RunPipe();
    
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 3; ++i) {
        if (debug_flag) {
            printf("%s(main) Popping from IN %s\n", LUCID_CYAN, LUCID_NORMAL);
        }
        int *data_handler = (int *)data_in->PopFromIn();
        if (debug_flag) {
            printf("%s(main) Pushing into OUT %s\n", LUCID_CYAN, LUCID_NORMAL);
        }
        data_in->PushIntoOut(data_handler);
    }
    pipe->WaitFinish();
    auto t2 = std::chrono::high_resolution_clock::now();
    
    printf("Time Elapsed running the pipe: %fs\n",
           std::chrono::duration<double>(t2 - t1).count());
    return 0;
}