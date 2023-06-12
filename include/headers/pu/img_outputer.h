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
 * @file img_outputer.h
 *
 * @brief The header file for the ImgOutputer processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#ifndef IMG_OUTPUTER_H
#define IMG_OUTPUTER_H

#include "../processing_unit_interface.h"

/**
 * @class ImgOutputer
 *
 * @brief This class represents an ImgOutputer, it gets an img matrix with
 * unsigned char values and sends it to the IO for creating two .ppm files
 */
class ImgOutputer : public ProcessingUnitInterface {
  public:
  // Default Constructor
  ImgOutputer();
  
  // Destructor
  ~ImgOutputer();
  
  // Doesn't need to allocate nothing
  void Start(void** pre_process_args) override;
  
  // Gets the img and prints it into 2 files.
  void Run(void *) override;
  
  // Doesn't need to deallocate anything
  void Delete() override;
  
  // Returns a pointer to a clone of ImgOutputer
  ProcessingUnitInterface *Clone() override;
  
  private:
  // Changes a value from Int to a Pixel value
  pixel_value IntToPixel(int);
};

#endif
