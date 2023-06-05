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
 * @file data.cc
 *
 * @brief The header file for the Data class
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#include "../headers/data.h"

/**
 * @brief Constructor of the Data class
 * @details Given the initial data allocated it gets it into a wrapper where you
 * can put extra data. This extra-data can be outputed from any of the
 * processing units and given a key to be recognizable for each contiguous
 * processing unit.
 *
 * @param data The initial data to assign to the class. It will be used ass the
 * carrier for all the next data in the class.
 * @param debug The debug flag for showing the information inside the Data class
 */
Data::Data(void *data, bool debug) : data_(data), debug_(debug) {}

/**
 * @brief The data destructor.
 * @details Don't know what to do with it yet
 */
Data::~Data() {}

/**
 * @brief Pushes data to the extra data vector to be accessed later on if needed
 *
 * @param extra_data The struct {key, data} to push to the vector
 */
void
Data::PushExtraData(DataKey *extra_data) {
  extra_data_.push_back(extra_data);
}

/**
 * @brief Looks for the extra data by the key given
 * @details Given a key, it iterates through a vector looking for an specific
 * pair {key, data}. If its found it returns the pointer to the data. Else it
 * returns a nullptr
 *
 * @param key The key to lookup in the vector
 */
void *
Data::GetExtraData(std::string key) {
  int size = extra_data_.size();
  for (int it = 0; it < size; ++it) {
    if (extra_data_[it]->key == key) {
      return extra_data_[it]->data;
    }
  }
  // No data found with that key
  return nullptr;
}

/**
 * @brief Returns the initial stored data inside the class
 * @return The pointer to the data
 */
void *
Data::data() {
  return data_;
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */