from osgeo import gdal
import csv
import rasterio
from rasterio.plot import show
import numpy as np

gdal.DontUseExceptions()

output = open("ElHierro.mat", "w")


ds = gdal.Open("EH.tif")

ar = ds.GetRasterBand(1).ReadAsArray()

flat = ar.flatten()

xsize = ds.RasterXSize
ysize = ds.RasterYSize

print(xsize, ysize)

for i in range(xsize*ysize):
    if flat[i] <= 0:
        output.write("0,")
    else:
        output.write(str(flat[i]) + ",")

output.close()
