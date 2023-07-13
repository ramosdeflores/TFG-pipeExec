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

/**
 * @file indexer.cc
 *
 * @brief The header file for the Indexer processing unit.
 *
 * @author Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#include "../../headers/pu/indexer.h"

/**
 * @brief Default constructor: does nothing
 */
Indexer::Indexer() {}

/**
 * @desc Default destructor: does nothing
 */
Indexer::~Indexer() {}

/**
 * @desc This function initializes the Indexer unit
 */
void Indexer::Start(void **pre_process_args) {
  table_size_ = 0;
  lookup_table_ = (TableIndexer *)malloc(sizeof(TableIndexer));
}

/**
 * @desc This function gets data and indexes it if it is broken into parts to
 * reconstruct it correctly later.
 * @param data The data to be indexed.
 */
void Indexer::Run(void *data) {
  Data *handler = (Data *)data;
  int32_t *id = (int32_t *)handler->GetExtraData("id");
  if (id == nullptr) {
    fprintf(stderr, "%s(Indexer)%s The Data has no identificator", LUCID_BLUE,
            LUCID_NORMAL);
    return;
  }
  TableIndexer *index = FindTableEntry(*id);
  if (index) {
    // The data is already beign indexed
    index->count++;
  } else {
    // The data is not indexed
    // Allocates the memory for the new block
    index = (TableIndexer *)malloc(sizeof(TableIndexer));
    // Reallocates the array
    lookup_table_ = (TableIndexer *)realloc(lookup_table_, table_size_ + 1);
    // Assigns the new data to the index
    index->id = *id;
    index->count = 0;
    // Sets the index in the last position of the array
    lookup_table_[table_size_] = *index;
    // Adds one to the array size
    table_size_++;
  }
  handler->PushExtraData(new Data::DataKey{"index", new int(index->count)});
}

/**
 * @desc Frees the memory of the Indexer
 */
void Indexer::Delete() { free(lookup_table_); }

/**
 * @desc This function clones the Indexer
 * @return The cloned Indexer
 */
ProcessingUnitInterface *Indexer::Clone() { return nullptr; }

Indexer::TableIndexer *Indexer::FindTableEntry(int id) {
  for (int32_t it = 0; it < table_size_; ++it) {
    if (lookup_table_[it].id == id) {
      return &lookup_table_[it];
    }
  }
  return nullptr;
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
