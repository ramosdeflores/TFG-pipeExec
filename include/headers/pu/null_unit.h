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
 * @file null_unit.h
 *
 * @brief The header file for the NullUnit processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#ifndef NULL_UNIT_H
#define NULL_UNIT_H

#include "../processing_unit_interface.h"

class NullUnit : public ProcessingUnitInterface {
 public:
  // Default constructor
  NullUnit();

  // Destructor
  ~NullUnit();

  // Does nothing 
  void Run(void *) override;

  // Doesn't allocate nothing
  void Start(void** = nullptr) override;

  // Doesn't delete nothing
  void Delete() override;

  ProcessingUnitInterface* Clone() override;
};

#endif
