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
 * @file double_pipe.h
 *
 * @brief The header file for the DoublePipe processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#ifndef DOUBLE_PIPE_H
#define DOUBLE_PIPE_H
#include "../../headers/pipeline.h"
#include "../processing_unit_interface.h"
/**
 * @class DoublePipe
 *
 * @brief This class instantiates a Pipe inside a pipe showing the potential of LUCID at processing any data.
 */
class DoublePipe : public ProcessingUnitInterface {
    public:
    DoublePipe();
    ~DoublePipe();
    
    void Start();
    void Run(void*);
    void Delete();
    ProcessingUnitInterface* Clone();
    
    private:
    MemoryManager* sub_mem_manager;
    Pipeline* sub_pipeline;
};
#endif //DOUBLE_PIPE_H
