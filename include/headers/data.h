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
 * @file data.h
 *
 * @brief The header file for the Data class
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#ifndef DATA_H
#define DATA_H

#include "memory_manager.h"

/**
 * @class Data
 *
 * @brief A Data class that provides access to various forms of manipulation and
 * allocation for the processing units
 *
 * @details This class provides functionallity to store multiple data via keys
 * and a pointer to the original data that was stored initially with the
 * creation of the class.
 */
class Data {
public:
  /**
   * @brief This structure works like a std::pair but its member names are
   * meaningful and are used to store data paired to a particular key.
   */
  struct DataKey {
    std::string key; /**< The key used to find the data associated */
    void *data;      /**< The extra data stored*/
  };

  // The data constructor
  Data(void *, bool = false);

  // The data destructor
  ~Data();

  // Pushes the struct {key, data} to the extra data allocator
  void PushExtraData(DataKey *);

  // Gets the pointer to the extra data so you can manipulate it
  void *GetExtraData(std::string);

  // Gets the initial data stored in the class
  void *data();

private:
  void *data_;
  unsigned int index_;
  std::vector<DataKey *> extra_data_;
  bool debug_;
};

#endif
