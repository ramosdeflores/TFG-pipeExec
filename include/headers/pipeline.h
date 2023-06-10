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
 * @file pipeline.h
 *
 * @brief Declaration of the Pipeline class methods
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */
#ifndef PIPELINE_H
#define PIPELINE_H

#include "pipe_node.h"
#include "processing_unit_interface.h"

// TODO Create a table that describes the types of data inside the 
// AddProcessingUnit function

/**
 * @class Pipeline
 * @brief Class representing the pipeline for the processing of any type of
 * data.
 *
 * This class masks the processing of the data allowing the (main) thread to
 * only see one queue of data while inside the Pipeline there are multiple
 * queues and threads consuming the data and processing it concurrently to speed
 * up the proccess.
 */
class Pipeline {
  public:
  // Constructor for the Pipeline class
  Pipeline(ProcessingUnitInterface *, MemoryManager *, int,
           bool debug = false);
  
  // Destructor of the Pipeline
  ~Pipeline();
  
  // Adds a new processing unit to the Pipeline
  void AddProcessingUnit(ProcessingUnitInterface *, int, const char*, ...);
  
  // Runs the pipe making all the threads wait for an input
  int RunPipe();
  
  // Waits until all the threads finished processing
  void WaitFinish();
  
  private:
  std::vector<PipeNode *> execution_list_; /**< The list of nodes that need to
                                              be executed in order */
  int count_string_chars(const char*);
  std::mutex execution_mtx_; /**< The mutex to safely run the nodes */
  int node_number_;          /**< The number of nodes that are active */
  bool debug_;               /**< The flag to show debug information */
};

#endif
