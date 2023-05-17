add_library(include SHARED
    headers/semaphore.h
    headers/memory_manager.h
    headers/data.h
    headers/processing_unit_interface.h
    headers/pipeline.h
    headers/pipe_node.h
    headers/pu/adder.h
    headers/pu/number_printer.h
    headers/pu/img_truncator.h
    headers/pu/edge_detector.h
    headers/pu/img_outputer.h
    definitions/semaphore.cc
    definitions/memory_manager.cc
    definitions/data.cc
    definitions/pipeline.cc
    definitions/pipe_node.cc
    definitions/pu/adder.cc
    definitions/pu/number_printer.cc
    definitions/pu/img_truncator.cc
    definitions/pu/edge_detector.cc
    definitions/pu/img_outputer.cc
    )

target_include_directories(include PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/headers
    ${CMAKE_CURRENT_LIST_DIR}/definitions)
