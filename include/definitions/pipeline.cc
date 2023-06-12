#include "../headers/pipeline.h"
#include <cstdio>
#include <string>

//                              |  ...  |
//                              |-------|
//                              | data  |
//                              |-------|
//                              |   |   |
//                              |   v   |
// |-----|-----|-----|      +----------------+
// | ... | PU  |  PU |  ->  |    PipeNode    |
// |-----|-----|-----|      +----------------+

/**
 * @brief Constructor for the Pipeline class
 *
 * @details Sets the first Node: assigns the processing unit to the one
 * specified as "first_function" then gets the MemoryManager from the (main)
 * thread and puts it as input and output of the Node. Sets the Node as the last
 * node to know that the data has to be pushed into the IN queue for the
 * circular processing to be working.
 *
 */
Pipeline::Pipeline(ProcessingUnitInterface *first_function,
                   MemoryManager *data_in, int threads_per_node_, bool debug)
    : debug_(debug) {
  node_number_ = 0;
  PipeNode *first_node;

  // first_node = (PipeNode *)malloc(sizeof(PipeNode));
  first_node = new PipeNode;
  first_node->node_id(node_number_);
  first_node->last_node(true);
  first_node->out_data_queue(data_in);
  first_node->in_data_queue(data_in);
  first_node->processing_unit(first_function);
  first_node->number_of_instances(threads_per_node_);

  execution_list_.push_back(first_node);
  node_number_ += 1;
}

/**
 * @brief Default destructor for the pipeline.
 */
Pipeline::~Pipeline() {}

/**
 * @brief Adds a processing unit to the execution list.
 * @details Adds a processing unit to the execution list. Creates a new Memory
 * Manager in the previous node for it to function as an output of the previous
 * and an input for the new one. Sets the previous node flag of last_node to
 * false and gets the new node to be the tail. Then connects the output of the
 * last node to the input of the first node to complete the circular processing
 * queue.
 *
 * @param processing_unit A pointer to an object of ProcessingUnitInterface
 * @param instances The number of threads that have to be instanced to run this
 * node.
 * @param fmg A pointer to the format string of the next arguments
 */
void
Pipeline::AddProcessingUnit(ProcessingUnitInterface *processing_unit,
                            int instances, const char *fmt, ...) {

  PipeNode *new_node = new PipeNode;

  // Variadic function handling
  if (fmt != nullptr) {
    int n_args = count_arguments(fmt);
    void **push_arguments = (void **)malloc(n_args * sizeof(void *));
    va_list variadic_args;
    va_start(variadic_args, fmt);
    for (int i = 0; i < n_args; ++i) {
      ArgumentType arg = extract_arg(fmt, i);
      switch (arg) {
        case kInt: {
          i64 argument = va_arg(variadic_args, i64);
          push_arguments[i] = new i64(argument);
        } break;

        case kUnsigned: {
          u64 argument = va_arg(variadic_args, u64);
          push_arguments[i] = new u64(argument);
        } break;

        case kFloat: {
          double argument = va_arg(variadic_args, double);
          push_arguments[i] = new double(argument);
        } break;

        case kExponential: {
          double argument = va_arg(variadic_args, double);
          push_arguments[i] = new double(argument);
        } break;

        case kString: {
          char *argument = va_arg(variadic_args, char *);
          push_arguments[i] = new char*(argument);
        } break;

        case kChar: {
          char argument = va_arg(variadic_args, char);
          push_arguments[i] = new char(argument);
        } break;

        default: {
          throw PipelineError::kBadArgumentType;
        } break;
      }
    }
    va_end(variadic_args);
    new_node->extra_args(push_arguments);
  }

  int prev_idx = node_number_ - 1;
  execution_list_[prev_idx]->last_node(false);
  execution_list_[prev_idx]->out_data_queue(new MemoryManager(
      execution_list_[0]->in_data_queue()->max_size(), debug_));

  new_node->out_data_queue(execution_list_[0]->in_data_queue());
  new_node->in_data_queue(execution_list_[prev_idx]->out_data_queue());
  new_node->node_id(node_number_);
  new_node->last_node(true);
  new_node->processing_unit(processing_unit);
  new_node->number_of_instances(instances);
  execution_list_.push_back(new_node);
  node_number_ += 1;
}

/**
 * @brief The function that all threads execute to run their processing unit.
 * @details This function gets the data from the input MemoryManager, executes
 * the processing unit with the data and sends it through the output queue of
 * the output MemoryManager. If it is the last node it sends the data trhough
 * the input queue of the output MemoryManager.
 *
 * @param node The node to be executed.
 * @param id The thread id. Especially used for debug information.
 * @param mtx The mutex to keep safe the initialization of the node while its
 * cloning instances of the processing unit.
 * @param debug The debug flag
 *
 */
void
RunNode(PipeNode *node, int id, std::mutex &mtx, bool debug = false) {
  try {
    void *data;
    ProcessingUnitInterface *processing_unit;
    processing_unit = node->processing_unit();
    if (id != 0) {
      processing_unit = node->processing_unit()->Clone();
    }
    if (processing_unit == nullptr) {
      fprintf(stderr,
              "%s(NODE %d)(THREAD %d):%s Cannot clone the processing "
              "unit: GOING TO SET AS THE DEFAULT GIVEN BY THE NODE\n",
              LUCID_RED, node->node_id(), id, LUCID_NORMAL);
      processing_unit = node->processing_unit();
    }
    mtx.unlock();
    processing_unit->Start(node->extra_args());
    do {
      if (debug) {
        printf("%s(NODE %d)(THREAD %d):%s Popping from IN->OUT\n", LUCID_CYAN,
               node->node_id(), id, LUCID_NORMAL);
      }
      data = node->in_data_queue()->PopFromOut();
      processing_unit->Run(data);
      if (node->last_node()) {
        if (debug) {
          printf("%s(NODE %d)(THREAD %d):%s Pushing into OUT->IN\n", LUCID_CYAN,
                 node->node_id(), id, LUCID_NORMAL);
        }
        node->out_data_queue()->PushIntoIn(data);
      } else {
        if (debug) {
          printf("%s(NODE %d)(THREAD %d):%s Pushing into OUT->OUT\n",
                 LUCID_CYAN, node->node_id(), id, LUCID_NORMAL);
        }
        node->out_data_queue()->PushIntoOut(data);
      }
    } while (true);
    processing_unit->Delete();
  } catch (...) {
  }
}

/**
 * @brief Sets the pipeline to run.
 * @details For each node inside the execution list it creates "n" instances of
 * threads per node and executes all of them.
 *
 * @return The number of nodes executed
 */
int
Pipeline::RunPipe() {
  int nodes_executed = 0;
  for (int it = 0; it < node_number_; ++it) {
    int number_of_subthreads = execution_list_[it]->number_of_instances();
    for (int thread_it = 0; thread_it < number_of_subthreads; ++thread_it) {
      try {
        execution_mtx_.lock();
        execution_list_[it]->PushThread(
            new std::thread(RunNode, execution_list_[it], thread_it,
                            std::ref(execution_mtx_), debug_));
      } catch (...) {
      }
    }
    nodes_executed += 1;
  }
  return nodes_executed;
}

/**
 * @brief Waits until all the threads have finished putting its data inside the
 * (main)'s in_queue MemoryManager.
 */
void
Pipeline::WaitFinish() {
  while (execution_list_[0]->in_data_queue()->in_queue_count() !=
         execution_list_[0]->in_data_queue()->max_size()) {
    execution_list_[0]->in_data_queue()->wait_finish();
  };
}

/**
 * @brief A private function that couns the chars into const char*
 */
int
Pipeline::count_arguments(const char *str) {
  int counter = 0;
  while (str[counter] != '\0') {
    counter += 1;
  }
  return counter;
}

/**
 * @brief This method extracts each argument for it to be compared to a series
 * of specified formats to allow the extraction of the arguments of the
 * AddProcessingUnit variadic function
 *
 * @param fmt The C string containing the format
 * @param arg_pos The position of the argument in the C string: (0,1,2...);
 */
Pipeline::ArgumentType
Pipeline::extract_arg(const char *fmt, u64 arg_pos) {
  ArgumentType result;
  char c = fmt[arg_pos];
  switch (c) {
    case 'd':
      result = ArgumentType::kInt;
      break;
    case 'u':
      result = ArgumentType::kUnsigned;
      break;
    case 'f':
      result = ArgumentType::kFloat;
      break;
    case 'e':
      result = ArgumentType::kExponential;
      break;
    case 's':
      result = ArgumentType::kString;
      break;
    case 'c':
      result = ArgumentType::kChar;
      break;
    default:
      throw PipelineError::kBadArgumentFormat;
  }
  return result;
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
