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
#include <tiff.h>
#define PRINT(format, ...) printf(format)
#define MAT_AT(mat, row, col) mat[i][j]
#define NEWLINE printf('\n');
#include "processing_units.h"

void
extraerAlturasDesdeTIFF(const char *archivoTIFF) {
  TIFF *tif = TIFFOpen(archivoTIFF, "r");
  if (tif) {
    uint32_t ancho, alto;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &ancho);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &alto);
    
    uint32_t tileWidth, tileHeight;
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tileWidth);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileHeight);
    
    uint32_t *buffer =
    (uint32_t *)_TIFFmalloc(tileWidth * tileHeight * sizeof(uint32_t));
    float **matrizAlturas = new float *[alto];
    for (uint32_t i = 0; i < alto; ++i) {
      matrizAlturas[i] = new float[ancho];
    }
    
    for (uint32_t y = 0; y < alto; y += tileHeight) {
      for (uint32_t x = 0; x < ancho; x += tileWidth) {
        TIFFReadTile(tif, buffer, x, y, 0, 0);
        for (uint32_t i = 0; i < tileHeight; ++i) {
          uint32_t fila = y + i;
          if (fila >= alto) {
            break;
          }
          for (uint32_t j = 0; j < tileWidth; ++j) {
            uint32_t col = x + j;
            if (col >= ancho) {
              break;
            }
            matrizAlturas[fila][col] = (float)buffer[i * tileWidth + j];
          }
        }
      }
    }
    
    // Hacer algo con la matriz de alturas aquí
    
    _TIFFfree(buffer);
    TIFFClose(tif);
  } else {
    std::cout << "Error al abrir el archivo TIFF." << std::endl;
  }
}

int
IslandsMain(bool debug_flag, bool pu_debug_flag) {
  
  TIFF *tif = TIFFOpen("../input_images/ElHierro.tiff", "r");
  u64 width;
  u64 height;
  u64 tile_width;
  u64 tile_height;
  u64 unit;
  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
  TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
  TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &unit);
  printf("%d", unit);
  
  u64 *buffer = (u64 *)_TIFFmalloc(tile_width * tile_height * sizeof(u64));
  float **matrix = new float *[height];
  for (u64 i = 0; i < height; ++i) {
    matrix[i] = new float[width];
  }
  
  for (u64 y = 0; y < height; y += tile_height) {
    for (u64 x = 0; x < width; x += tile_width) {
      TIFFReadTile(tif, buffer, x, y, 0, 0);
      
      for (u64 i = 0; i < tile_height; ++i) {
        u64 row = y + i;
        if (row >= height)
          break;
        for (u64 j = 0; j < tile_width; ++j) {
          u64 col = x + j;
          if (col > width)
            break;
          matrix[row][col] = (double)buffer[i * tile_width + j];
          printf("%f ", MAT_AT(matrix, row, col));
        }
        printf("\n");
      }
    }
  }
  
  std::string unidadMedida = "AAAA";
  switch (unit) {
    case RESUNIT_NONE:
    unidadMedida = "Sin unidad";
    break;
    case RESUNIT_INCH:
    unidadMedida = "Pulgadas";
    break;
    case RESUNIT_CENTIMETER:
    unidadMedida = "Centímetros";
    break;
  }
  
  _TIFFfree(buffer);
  TIFFClose(tif);
  printf("\n%s\n", unidadMedida.c_str());
  return 0;
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */