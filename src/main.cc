/*
 * LUCID (Unified Computing Image and Data-Processing) is a program to process
 * any type of data concurrently. Copyright (C) 2023 Lucas Hernández Abreu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Author:  Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#include "double_pipe_main.cc"
#include "islands_main.cc"
#include "processing_units.h"
#include "simple_edge_detection_main.cc"
#include "sleeper_main.cc"

// FORWARD DECLARATIONS
std::string Help();
int RunMain();
int protected_main();

/**
 * @brief Helper showed when used the --help flag or had a bad argument input
 */
std::string
Help() {
  std::string usage_str =
      "\tLUCID (Unified Computing Image and Data-Processing)\n"
      "\tCopyright (C) 2023  Lucas Hernández Abreu\n"
      "\tThis program comes with ABSOLUTELY NO WARRANTY\n"
      "\nExecute inside bin with:\n"
      "    ./lucid run <arg>\n"
      "Valid args:\n"
      "\tDebug -> [debug]|[-d]|[--debug]: Shows the debug "
      "information:\n"
      "\t    (operations in MemoryManager, semaphore statuses...)\n"
      "\tDebug Processing Units -> [pudebug][--pd][--pu-debug]: shows the "
      "debug for the processing units\n"
      "\t    (operations inside the processing units)\n"
      "\tHelp -> [help]|[-h]|[--help]: Shows this message\n";
  return usage_str;
}

/**
 * @brief This function execs the main and permits the throwing of exceptions so
 * the main function can catch them
 */
int
protected_main(int argc, char **argv) {
  std::string arguments;
  std::string arguments2;
  bool debug_flag = false;
  bool pu_debug_flag = false;
  bool profiling_flag = false;
  if (argc != 1) {
    arguments = std::string(argv[1]);
    if (argc > 2) {
      arguments2 = argv[2];
      if (arguments == "run" && (arguments2 == "debug" || arguments2 == "-d" ||
                                 arguments2 == "--debug")) {
        debug_flag = true;
        pu_debug_flag = true;
      } else if (arguments == "run" &&
                 (arguments2 == "pudebug" || arguments2 == "-pd" ||
                  arguments2 == "--pu-debug" || arguments2 == "--pd")) {
        pu_debug_flag = true;
      } else if (arguments == "run" &&
                 (arguments2 == "profiling" || arguments2 == "-pr" ||
                  arguments2 == "--profiling" || arguments2 == "--pr")) {
        profiling_flag = true;
      }
    } else if (arguments != "run") {
      printf("%s", Help().c_str());
      return 0;
    }
  } else {
    printf("%s", Help().c_str());
    return 0;
  }

  // Put here the function we're going to use
  return DoublePipeMain(debug_flag, pu_debug_flag, profiling_flag);
}

int
main(int argc, char **argv) {
  try {
    protected_main(argc, argv);
  } catch (MemoryManager::MemoryManagerError e) {
    switch (e) {
      case MemoryManager::MemoryManagerError::kBadSizing: {
        fprintf(stderr, "%s(MemoryManager) Bad size for buffer queues%s\n",
                LUCID_RED, LUCID_NORMAL);
        return 1;
      }
      case MemoryManager::MemoryManagerError::kNullPtr: {
        fprintf(stderr, "%s(MemoryManager) Tried to return a null pointer\n%s",
                LUCID_RED, LUCID_NORMAL);
        return 1;
      }
    }
  }
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
