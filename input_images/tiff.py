from osgeo import gdal
import csv
import rasterio
from rasterio.plot import show
import numpy as np

output = open("ElHierro.mat", "w")


ds = gdal.Open("EH.tif")

