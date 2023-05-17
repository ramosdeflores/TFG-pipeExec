/*
 * LUCID (Unified Computing Image and Data-Processing) is a program to process
 * any type of data concurrently. Copyright (C) 2023 Lucas Hernández Abreu
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
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
 * @file edge_detector.cc
 *
 * @brief The definition file for the EdgeDetector processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */
#include "../../headers/pu/edge_detector.h"
#include <cmath>
#include <cstdio>

/**
 * @brief Default constructor for EdgeDetector class
 */
EdgeDetector::EdgeDetector() {}

/**
 * @brief Destructor of the EdgeDetector class
 */
EdgeDetector::~EdgeDetector() {}

/**
 * @brief This method allocates memory for the sobel filter and assigns the
 * values.
 */
void
EdgeDetector::Start() {
    sobelx_ = (int **)malloc(3 * sizeof(int *));
    sobely_ = (int **)malloc(3 * sizeof(int *));
    for (int it = 0; it < 3; ++it) {
        sobelx_[it] = (int *)malloc(sizeof(int));
        sobely_[it] = (int *)malloc(sizeof(int));
    }
    sobelx_[0][0] = -1;
    sobelx_[0][1] = 0;
    sobelx_[0][2] = 1;
    sobelx_[1][0] = -2;
    sobelx_[1][1] = 0;
    sobelx_[1][2] = 2;
    sobelx_[2][0] = -1;
    sobelx_[2][1] = 0;
    sobelx_[2][2] = 1;

    sobely_[0][0] = 1;
    sobely_[0][1] = 2;
    sobely_[0][2] = 1;
    sobely_[1][0] = 0;
    sobely_[1][1] = 0;
    sobely_[1][2] = 0;
    sobely_[2][0] = -1;
    sobely_[2][1] = -2;
    sobely_[2][2] = -1;
}

/**
 * @brief Gets the Data and applies the needed methods to detect borders
 * @details Extracts all the information from the Data object. Once done with
 * it, applies a convolution in X and Y to the image. Then calculates the
 * Magnitude of the results and pushes it to the ExtraData inside the data
 * pointer.
 *
 * @param data The data extracted from the in_queue
 */
void
EdgeDetector::Run(void *data) {
    Data *handler = (Data *)data;
    int **img = (int **)handler->GetExtraData("truncated");

    bool debug = *(bool *)handler->GetExtraData("debug");
    debug_ = debug;
    int width = *(int *)handler->GetExtraData("width");
    int height = *(int *)handler->GetExtraData("height");
    int max_rand = *(int *)handler->GetExtraData("max_rand");
    int id = *(int *)handler->GetExtraData("id");

    if (debug_) {
        printf("%s(EdgeDetector)%s -> \n\tWidth:%d\n\tHeight:%d\n", LUCID_BLUE,
               LUCID_NORMAL, width, height);
    }

    int **img_sobelx = Convolution(sobelx_, img, width, height);

    int **img_sobely = Convolution(sobely_, img, width, height);

    if (debug_) {

        printf("%s(EdgeDetector)%s -> \n\tCONVOLUTION:\n\t\tsobelx pointer: "
               "%p memalloc: %zu\n\t\tsobely pointer: %p memalloc: %zu\n",
               LUCID_BLUE, LUCID_NORMAL, img_sobelx,
               malloc_usable_size(img_sobelx), img_sobely,
               malloc_usable_size(img_sobely));
    }

    pixel_value **result =
        (pixel_value **)malloc(height * sizeof(pixel_value *));
    for (int it = 0; it < height; ++it) {
        result[it] = (pixel_value *)malloc(width * sizeof(pixel_value));
    }

    Magnitude(result, img_sobelx, img_sobely, width, height, max_rand);

    if (debug_) {
        printf("%s(EdgeDetector)%s -> Going TO PUSH\n", LUCID_BLUE,
               LUCID_NORMAL);
    }
    handler->PushExtraData(
        new Data::DataKey({"processed" + std::to_string(id), result}));
}

/**
 * @brief Frees the memory of the sobel filters
 */
void
EdgeDetector::Delete() {
    for (int it = 0; it < 3; ++it) {
        free(sobelx_[it]);
        free(sobely_[it]);
    }
    free(sobelx_);
    free(sobely_);
}

/**
 * @brief Clones the processing unit
 * @return A new pointer to the EdgeDetector class clone
 */
ProcessingUnitInterface *
EdgeDetector::Clone() {
    return new EdgeDetector;
}

/**
 * @brief Applies the convolution of the specified kernel to the image taking
 * into consideration its width and height
 *
 * @param kernel A pointer to the matrix with the kernel values (it has to be
 * 3x3)
 * @param img A pointer to the matrix with the values of the img
 * @param width The width of the image
 * @param height The height of the image
 *
 * @return The result of the convolution between the kernel and the img
 */
int **
EdgeDetector::Convolution(int **kernel, int **img, int width, int height) {

    // Creating the result img
    int **result = (int **)malloc(height * sizeof(int *));
    for (int it = 0; it < height; ++it) {
        result[it] = (int *)malloc(width * sizeof(int));
    }
    int kernel_width = 3;
    int kernel_height = 3;
    int offset_x = kernel_width / 2;
    int offset_y = kernel_height / 2;

    if (debug_) {
        printf(
            "%s(EdgeDetector)%s -> \n\tCONVOLUTION:\n\t\tResult pointer: %p\n",
            LUCID_BLUE, LUCID_NORMAL, result);
    }

    int sum = 0;
    int *line;
    int *lookup_line;

    for (int y = 0; y < height; ++y) {
        line = result[y];
        for (int x = 0; x < width; ++x) {
            sum = 0;
            for (int kh_it = 0; kh_it < kernel_height; ++kh_it) {
                if (y + kh_it < offset_y || y + kh_it >= height) {
                    continue;
                }
                lookup_line = img[y + kh_it - offset_y];
                for (int kw_it = 0; kw_it < kernel_width; ++kw_it) {
                    if (x + kw_it < offset_x || x + kw_it >= width) {
                        continue;
                    }
                    sum += kernel[kh_it][kw_it] *
                           lookup_line[x + kw_it - offset_x];
                }
            }
            line[x] = Bound(sum);
        }
    }
    return result;
}

/**
 * @brief Calculates the magnitude between the kernels and puts the data inside
 * the result param in pixel_value(s)
 *
 * @param result The pointer to the matrix where we'll write
 * @param gx The result of the convolution by x
 * @param gy The result of the convolution by y
 * @param width The width of the final img
 * @param height The height of the final img
 * @param max_rand The number that we used in the mod operator to limit the
 * rand() result
 */
void
EdgeDetector::Magnitude(pixel_value **result, int **gx, int **gy, int width,
                        int height, int max_rand) {

    pixel_value *line;
    int *gx_line;
    int *gy_line;

    for (int h_it = 0; h_it < height; ++h_it) {
        line = result[h_it];
        gx_line = gx[h_it];
        gy_line = gy[h_it];

        for (int w_it = 0; w_it < width; ++w_it) {
            line[w_it] = IntToPixel((Hypotenuse(gx_line[w_it], gy_line[w_it])) *
                                    255 / (max_rand * 100 / 100));
        }
    }
}

/**
 * @brief Converts from Int to pixel_value(unsigned char)
 *
 * @param i_val The int value
 *
 * @return The pixel_value
 */

pixel_value
EdgeDetector::IntToPixel(int i_val) {
    if (i_val < 0) {
        return 0;
    } else if (i_val > 255) {
        return 255;
    } else {
        return i_val & 255;
    }
}

/**
 * @brief Gets the int number to the limits of the unsigned char colors
 *
 * @param i_val The int param
 *
 * @return The int value below the limit
 */
int
EdgeDetector::Bound(int i_val) {
    if (i_val < 0) {
        return 0;
    } else if (i_val > 255) {
        return 255;
    } else {
        return i_val & 255;
    }
}

/**
 * @brief Calculates the hypotenuse of two integers
 *
 * @return The result of the calculation as integer
 */
int
EdgeDetector::Hypotenuse(int a, int b) {
    return std::sqrt<int>(a * a + b * b);
}
