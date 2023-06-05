/*
 * LUCID (Unified Computing Image and Data-Processing) is a program to process
 * any type of data concurrently.
 * Copyright (C) 2023 Lucas Hernández Abreu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. This program is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Author:  Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

/** @file img_truncator.cc
 *
 * @brief The definition file for the ImgTruncator processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */
#include "../../headers/pu/img_truncator.h"

/**
 * @brief Constructor of the class
 */
ImgTruncator::ImgTruncator() {}

/**
 * @brief Destructor of the class
 */
ImgTruncator::~ImgTruncator() {}

/**
 * @brief Function used for allocation, but allocation is not needed
 */
void
ImgTruncator::Start(void* pre_process_args) {}

/**
 * @brief The method that modified the data inside the pointer
 * @details Gets the width and height of the map with the Data methods. Sets all
 * the values of the matrix as multiples of 100
 *
 * @param data The pointer to the data to be truncated
 */
void
ImgTruncator::Run(void *data) {
  Data *handler = (Data *)data;
  int **img = (int **)(handler->data());
  int width = *(int *)handler->GetExtraData("width");
  int height = *(int *)handler->GetExtraData("height");
  int *max_value = (int *)handler->GetExtraData("max_rand");
  
  int **result = (int **)malloc(height * sizeof(int *));
  for (int it = 0; it < height; ++it) {
    result[it] = (int *)malloc(width * sizeof(int));
  }
  
  for (int y_it = 0; y_it < height; ++y_it) {
    for (int x_it = 0; x_it < width; ++x_it) {
      int value = (int)(img[y_it][x_it] / 3000) * 3000;
      if (value > *max_value) {
        *max_value = value;
      }
      result[y_it][x_it] = value;
    }
  }
  
  handler->PushExtraData(new Data::DataKey{"truncated", result});
}

/**
 * @brief Needed if allocation happened
 */
void
ImgTruncator::Delete() {}

/**
 * @brief Creates a clone of itself
 *
 * @return a pointer to a new ImgTruncator object
 */
ProcessingUnitInterface *
ImgTruncator::Clone() {
  return new ImgTruncator;
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */