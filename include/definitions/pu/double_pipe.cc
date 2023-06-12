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
 * @file double_pipe.cc
 *
 * @brief The header file for the DoublePipe processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */
// TODO(lucashdez): COMMENT THIS FILE DOUBLEPIPE.CC
#include "../../headers/pu/double_pipe.h"

DoublePipe::DoublePipe(){}
DoublePipe::~DoublePipe(){}

/**
* @brief This method instantiates the pipeline and the MemoryManager
* for the pipeline
* 
*/
void
DoublePipe::Start(void** pre_process_args) {
  // TODO(lucashdez): Instantiate pipeline
  // NOTE(lucashdez): Probably create a null processing unit
}

/**
* 
* 
*/
void
DoublePipe::Run(void* data) {
  // TODO(lucashdez): Insert the processing units and run the pipe with the data
  
}

/**
* 
* 
*/
void
DoublePipe::Delete() {
  
}

/**
* 
* 
*/
ProcessingUnitInterface* DoublePipe::Clone() {
  return new DoublePipe;
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
