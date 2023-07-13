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
 * @file NumberPrinter.cc
 *
 * @brief The definition file for the NumberPrinter processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#include "../../headers/pu/number_printer.h"

#include <cstdio>
#include <string>

/**
 * @brief Starts the number printer.
 */
void NumberPrinter::Start(void **pre_process_args) {}

/**
 * @brief Prints the number contained in a given pointer.
 *
 * @param data A void pointer to the data to print. The pointer has to be a Data
 * object pointer.
 */
void NumberPrinter::Run(void *data) {
  Data *conv_data = (Data *)data;
  printf("Data contained: %d\n", *((int *)conv_data->data()));
}

/**
 * @brief Does nothing.
 */
void NumberPrinter::Delete() {}

/**
 * @brief Creates a new instance of the number printer.
 *
 * @return A pointer to a new NumberPrinter object.
 */
ProcessingUnitInterface *NumberPrinter::Clone() { return new NumberPrinter; }

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
