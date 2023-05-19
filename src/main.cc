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

#include "../include/headers/pipeline.h"
#include "../include/headers/pu/adder.h"
#include "../include/headers/pu/edge_detector.h"
#include "../include/headers/pu/img_outputer.h"
#include "../include/headers/pu/img_truncator.h"
#include "../include/headers/pu/indexer.h"
#include "../include/headers/pu/number_printer.h"
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
        "    ./lucid <arg>\n"
        "Valid args:\n"
        "\tDebug -> [debug]|[-d]|[--debug]: Shows the debug "
        "information:\n"
        "\t    (operations in MemoryManager, semaphore statuses...)\n"
        "\tHelp -> [help]|[-h]|[--help]: Shows this message\n";
    return usage_str;
}

int
protected_main(int argc, char **argv) {
    std::string arguments = "";
    bool debug_flag = false;
    bool pu_debug_flag = false;
    if (argc > 1) {
        arguments = std::string(argv[1]);
        if (arguments == "debug" || arguments == "-d" ||
            arguments == "--debug") {
            debug_flag = true;
            pu_debug_flag = true;
        } else if (arguments == "pudebug" || arguments == "-pd" ||
                   arguments == "--pu-debug" || arguments == "--pd") {
            pu_debug_flag = true;
        } else if (arguments == "help" || arguments == "-h" ||
                   arguments == "--help") {
            printf("%s", Help().c_str());
            return 0;
        } else {
            printf("%s", Help().c_str());
            return 0;
        }
    }

    int height = 512;
    int width = 512;
    int max_num = 0;
    int number_of_images = 1;
    int number_of_threads = 12;

    ImgTruncator img_data_discrimination;
    EdgeDetector edge_detection;
    ImgOutputer img_output;
    Indexer indexer;

    MemoryManager *data_in = new MemoryManager(number_of_images, debug_flag);
    for (int it = 0; it < number_of_images; ++it) {
        std::ifstream file("../../input_images/heightmap" + std::to_string(it) +
                           ".mat");
        std::string mat_input;
        std::vector<int> input_img_values;
        while (getline(file, mat_input, ',')) {
            input_img_values.emplace_back(std::stoi(mat_input));
        }

        int **img = (int **)malloc(height * sizeof(int *));
        for (int i = 0; i < height; ++i) {
            img[i] = (int *)malloc(width * sizeof(int));
        }

        for (int h_it = 0; h_it < height; h_it++) {
            for (int w_it = 0; w_it < width; ++w_it) {
                int value = input_img_values[w_it + (h_it * width)];
                img[h_it][w_it] = value;
            }
        }

        Data *some_data = new Data(img);
        some_data->PushExtraData(new Data::DataKey{"width", new int(height)});
        some_data->PushExtraData(new Data::DataKey{"height", new int(width)});
        some_data->PushExtraData(new Data::DataKey({"debug", &pu_debug_flag}));
        some_data->PushExtraData(new Data::DataKey{"id", new int(it)});
        some_data->PushExtraData(
            new Data::DataKey{"max_rand", new int(max_num)});
        data_in->LoadMemoryManager(some_data);
    }

    Pipeline *pipe = new Pipeline(&indexer, data_in, 1, debug_flag);
    pipe->AddProcessingUnit(&img_data_discrimination, number_of_threads);
    pipe->AddProcessingUnit(&edge_detection, number_of_threads);
    pipe->AddProcessingUnit(&img_output, number_of_threads);
    pipe->RunPipe();

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < number_of_images; ++i) {
        if (debug_flag) {
            printf("%s(main) Popping from IN %s\n", LUCID_CYAN, LUCID_NORMAL);
        }
        int *data = (int *)data_in->PopFromIn();
        if (debug_flag) {
            printf("%s(main) Pushing into OUT %s\n", LUCID_CYAN, LUCID_NORMAL);
        }
        data_in->PushIntoOut(data);
    }
    pipe->WaitFinish();
    auto t2 = std::chrono::high_resolution_clock::now();

    printf("Time Elapsed running the pipe: %f",
           std::chrono::duration<double>(t2 - t1).count());

    return 0;
}

int
main(int argc, char **argv) {
    try {
        protected_main(argc, argv);

    } catch (MemoryManager::MemoryManagerError e) {
        switch (e) {
            case MemoryManager::MemoryManagerError::kBadSizing: {
                fprintf(stderr,
                        "%s(MemoryManager) Bad size for buffer queues%s\n",
                        LUCID_RED, LUCID_NORMAL);
                return 1;
            }
            case MemoryManager::MemoryManagerError::kNullPtr: {
                fprintf(stderr,
                        "%s(MemoryManager) Tried to return a null pointer\n%s",
                        LUCID_RED, LUCID_NORMAL);
                return 1;
            }
        }
    }
}
