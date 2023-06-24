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

#include "libraries.h"
#include "pipe_node.h"
#include "processing_unit_interface.h"

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
  enum PipelineError {
    kBadArgumentFormat,
    kBadArgumentType,
  };

  /**
   * @brief This struct holds the information needed for the profiling of the
   * processing unit
   * @desc This struct holds all the information that the pipe will print when
   * the print_profile method function is called
   */
  struct Profiling {
    i32 node_id;   /**< The id of the node to profile */
    i32 thread_id; /**< The id of the thread that executed the processing unit
                    */
    u64 cycles_start; /**< The timestamp from the tsc in the CPU at the
                        start of the RunNode function */
    u64 cycles_end;   /**< The timestamp from the tsc in the CPU at the end
                        of the RunNode function */
    TIME_POINT
    time_start; /**< The clock time at the start of the RunNode function */
    TIME_POINT
    time_end; /**< The clock time at the end of the RunNode function */
    i64 sys_time_start; /**< The system time at the start of the RunNode
                          function */
    i64 sys_time_end; /**< The system time at the end of the RunNode function */
  };
  // Constructor for the Pipeline class
  Pipeline(ProcessingUnitInterface *, MemoryManager *, int, bool = false,
           bool = false);

  // Destructor of the Pipeline
  ~Pipeline();

  // Adds a new processing unit to the Pipeline
  void AddProcessingUnit(ProcessingUnitInterface *, int, const char * = nullptr,
                         ...);

  // Runs the pipe making all the threads wait for an input
  int RunPipe();

  // Waits until all the threads finished processing
  void WaitFinish();

  void Profile();

private:
  /**
   * @brief The types of the internal arguments
   * @desc The types of the internal arguments in the variadic function
   */
  enum ArgumentType {
    kInt,         /**< Type is d */
    kUnsigned,    /**< Type is u */
    kFloat,       /**< Type is f */
    kExponential, /**< Type is e */
    kString,      /**< Type is s */
    kChar         /**< Type is c */
  };

  std::vector<PipeNode *> execution_list_; /**< The list of nodes that need to
                                             be executed in order */
  int count_arguments(const char *); /**< Returns the number of arguments */

  ArgumentType extract_arg(const char *,
                           u64); /**< Extracts the argument type */

  std::mutex execution_mtx_;   /**< The mutex to safely run the nodes */
  std::mutex profiling_mutex_; /**< The mutex to safely recollect times */
  int node_number_;            /**< The number of nodes that are active */
  bool debug_;                 /**< The flag to show debug information */
  bool profiling_;             /**< The flag to show profiling information */
  std::vector<Profiling>
      profiling_list_; /**< The list of profiling information */
};

#endif
