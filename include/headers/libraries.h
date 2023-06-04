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

#ifndef LIBRARIES_H
#define LIBRARIES_H

#define LUCID_NORMAL "\x1B[0m"
#define LUCID_RED "\x1B[31m"
#define LUCID_GREEN "\x1B[32m"
#define LUCID_YELLOW "\x1B[33m"
#define LUCID_BLUE "\x1B[34m"
#define LUCID_MAGENTA "\x1B[35m"
#define LUCID_CYAN "\x1B[36m"
#define LUCID_WHITE "\x1B[37m"

#include "tiff.h"
#include "tiffconf.h"
#include "tiffio.h"
#include "tiffvers.h"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <malloc.h>
#include <math.h>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
// Value of colors in a PGM/PBM img
typedef unsigned int pixel_value;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


#endif
