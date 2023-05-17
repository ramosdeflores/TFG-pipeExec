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
 * @file NumberPrinter.h
 *
 * @brief The header file for the NumberPrinter processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#ifndef _NUMBERPRINTER_H
#define _NUMBERPRINTER_H

#include "../processing_unit_interface.h"

/**
 * @class NumberPrinter
 *
 * @brief This class outputs the number inside the data to the standard output
 */
class NumberPrinter : public ProcessingUnitInterface {
  public:
    // Doesn't allocate anything
    void Start();

    // Prints the number to the standard output
    void Run(void *);

    // Doesn't deallocate anything
    void Delete();

    // Returns a pointer to a clone of NumberPrinter
    ProcessingUnitInterface *Clone();
};

#endif
