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
 * @file edge_detector.h
 *
 * @brief The header file for the EdgeDetector processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#ifndef EDGE_DETECTOR_H
#define EDGE_DETECTOR_H

#include "../processing_unit_interface.h"

/**
 * @class EdgeDetector
 *
 * @brief This processing unit detects the borders inside the img and sends them
 * as a new image through the pipe
 */
class EdgeDetector : public ProcessingUnitInterface {
  public:
  // Default constructor of the class
  EdgeDetector();
  
  // Destructor of the class
  ~EdgeDetector();
  
  // Method to allocate the sobel filter
  void Start(void* = nullptr) override;
  
  // Method to pass the filters and process the img
  void Run(void *) override;
  
  // Method to liberate the memory allocated
  void Delete() override;
  
  // Method to clone the class
  ProcessingUnitInterface *Clone() override;
  
  // Method to convolute via a kernel and an img
  int **Convolution(int **, int **, int, int);
  
  // Method to calculate the magnitude of the processed img
  void Magnitude(pixel_value **, int **, int **, int, int, int);
  
  private:
  pixel_value IntToPixel(int);
  int Bound(int);
  int Hypotenuse(int, int);
  int **sobelx_;
  int **sobely_;
  bool debug_;
};

#endif
