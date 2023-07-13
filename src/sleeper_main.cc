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

int SleeperMain(bool debug_flag, bool pu_debug_flag, bool profiling) {
  Sleeper sleeper_unit;
  Sleeper sleeper_unit2;
  NullUnit void_unit;
  MemoryManager *data_in = new MemoryManager(2, debug_flag);
  for (int i = 0; i < 2; i += 1) {
    Data *holder = new Data(new int(i));
    holder->PushExtraData(new Data::DataKey{"profiling", &profiling});
    data_in->LoadMemoryManager(holder);
  }

  Pipeline *pipe = new Pipeline(&void_unit, data_in, 1, debug_flag, profiling);
  pipe->AddProcessingUnit(&sleeper_unit, 1, "d", 1);
  pipe->AddProcessingUnit(&sleeper_unit2, 1, "d", 1);
  pipe->RunPipe();

  for (int i = 0; i < 4; ++i) {
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

  pipe->Profile();

  return 0;
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
