#include "processing_units.h"

int SimpleEdgeDetectionMain(bool debug_flag, bool pu_debug_flag,
                            bool profiling) {
  int height = 1188;
  int width = 1014;
  int max_num = 0;
  int number_of_images = 1;
  int number_of_threads = 1;

  ImgTruncator img_data_discrimination;
  EdgeDetector edge_detection;
  ImgOutputer img_output;
  Indexer indexer;

  MemoryManager *data_in = new MemoryManager(number_of_images, debug_flag);
  for (int it = 0; it < number_of_images; ++it) {
    std::ifstream file("../input_images/ElHierro.mat");
    std::string mat_input;
    std::vector<int> input_img_values;
    while (getline(file, mat_input, ',')) {
      input_img_values.push_back(std::stoi(mat_input));
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
    some_data->PushExtraData(new Data::DataKey{"debug", &pu_debug_flag});
    some_data->PushExtraData(new Data::DataKey{"id", new int(it)});
    some_data->PushExtraData(new Data::DataKey{"max_rand", new int(max_num)});
    data_in->LoadMemoryManager(some_data);
  }

  Pipeline *pipe = new Pipeline(&indexer, data_in, 1, debug_flag, profiling);
  pipe->AddProcessingUnit(&img_data_discrimination, number_of_threads);
  pipe->AddProcessingUnit(&edge_detection, number_of_threads);
  pipe->AddProcessingUnit(&img_output, number_of_threads);
  pipe->RunPipe();
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

  pipe->Profile();

  return 0;
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
