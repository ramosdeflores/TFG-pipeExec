/*
 * LUCID (Unified Computing Image and Data-Processing) is a program to process
 * any type of data concurrently.
 * Copyright (C) 2023 Lucas Hernández Abreu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
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
 * @file ImgOutputer.cc
 *
 * @brief The definition file for the ImgOutputer processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#include "../../headers/pu/img_outputer.h"

/**
 * @brief Default constructor
 */
ImgOutputer::ImgOutputer() {}

/**
 * @brief Destructor
 */
ImgOutputer::~ImgOutputer() {}

/**
 * @brief Doesn't allocate anything
 */
void
ImgOutputer::Start(void** pre_process_args) {}

/**
 * @brief Gets the image and creates two .ppm files: origina.ppm and output.ppm.
 *
 * @param data The pointer to the data to manipulate
 */
void
ImgOutputer::Run(void *data) {
  Data *handler = (Data *)data;
  int width = *(int *)handler->GetExtraData("width");
  int height = *(int *)handler->GetExtraData("height");
  int max_rand = *(int *)handler->GetExtraData("max_rand");
  int **img = (int **)handler->data();
  int id = *(int *)handler->GetExtraData("id");
  bool debug = *(bool *)handler->GetExtraData("debug");
  pixel_value **to_pgm =
  (pixel_value **)handler->GetExtraData("processed" + std::to_string(id));
  int **truncated = (int **)handler->GetExtraData("truncated");
  
  std::ofstream out_truncated("truncated" + std::to_string(id) + ".pgm",
                              std::ios::binary | std::ios::out |
                              std::ios::trunc);
  std::ofstream out_proc("output" + std::to_string(id) + ".pbm",
                         std::ios::binary | std::ios::out | std::ios::trunc);
  std::ofstream out_orig("original" + std::to_string(id) + ".pgm",
                         std::ios::binary | std::ios::out | std::ios::trunc);
  out_proc << "P3\n" << width << " " << height << "\n255\n";
  out_orig << "P2\n" << width << " " << height << "\n255\n";
  out_truncated << "P2\n" << width << " " << height << "\n255\n";
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      pixel_value to_pgm_v = to_pgm[i][j];
      
      pixel_value out_orig_v = IntToPixel((img[i][j] * 255) / max_rand);
      pixel_value out_truncated_v =
        IntToPixel(truncated[i][j] * 255 / max_rand);
      if (to_pgm_v != 0) {
        out_proc << 0 << " " << (255 - out_orig_v) << " " << 0 << " ";
      } else {
        out_proc << out_orig_v << " " << out_orig_v << " " << out_orig_v
          << " ";
      }
      out_truncated << out_truncated_v << " ";
      out_orig << out_orig_v << " ";
    }
  }
}

/**
 * @brief Doesn't deallocate anything
 */
void
ImgOutputer::Delete() {}

/**
 * @brief This method clones the processing unit
 *
 * @return A pointer to a new instance of ImgOutputer
 */
ProcessingUnitInterface *
ImgOutputer::Clone() {
  return new ImgOutputer;
}

/**
 * @brief Transforms the Int number into the pixel_value value (unsigned char)
 *
 * @param i_val The Int value
 *
 * @return The value in pixels
 */
pixel_value
ImgOutputer::IntToPixel(int i_val) {
  if (i_val < 0) {
    return 0;
  } else if (i_val > 255) {
    return 255;
  } else {
    return i_val & 255;
  }
}



/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
