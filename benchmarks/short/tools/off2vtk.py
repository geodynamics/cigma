#!/usr/bin/env python

import sys
import numpy
import tables

from vtkwriter import write_tet4_vtk

off_file = "residuals.off"
vtk_file = "residuals.vtk"

argc = len(sys.argv)
if argc > 1:
    off_file = sys.argv[1]
if argc > 2:
    vtk_file = sys.argv[2]

fp = open(off_file, 'r')
print "Reading", fp.name

nno, nel = map(int, fp.readline().split())
coordinates = numpy.zeros((nno,3), numpy.float32)
connectivity = numpy.zeros((nel, 4), numpy.int32)
residuals = numpy.zeros((nel,), numpy.float32)

for n in xrange(nno):
    line = fp.readline()
    coordinates[n] = map(float, line.split())

for e in xrange(nel):
    line = fp.readline()
    cols = line.split()
    connectivity[e] = map(int, cols[0:4])
    residuals[e] = float(cols[4])

write_tet4_vtk(vtk_file, coordinates, connectivity, residuals)

