#!/usr/bin/env python

import numpy

VTK_TRIANGLE = 5
VTK_QUAD = 9
VTK_TETRA = 10
VTK_HEXAHEDRON = 12

class VtkWriter(object):
    
    def __init__(self):
        self.fp = None
        self.coords = None
        self.connect = None
        self.residuals = None
        self.cell = None

    def set_mesh(self, coords, connect, cell):
        self.coords = coords
        self.connect = connect
        self.cell = cell

    def set_data(self, residuals):
        self.residuals = residuals
        assert residuals.shape[0] == self.connect.shape[0]

    def open(self, filename):
        print("Creating", fp.name)
        self.fp = open(filename, "w")
        self.write_header()

    def close(self):
        self.fp.close()
        self.fp = None

    def write_header(self):
        fp = self.fp
        fp.write("# vtk DataFile Version 3.0\n")
        fp.write("This line is a comment\n")
        fp.write("ASCII\n")
        fp.write("DATASET UNSTRUCTURED_GRID\n")

    def write_points(self):
        nno,nsd = self.coords.shape
        fp.write("POINTS %d float\n" % nno)
        for n in xrange(nno):
            fp.write("%g %g %g\n" % tuple(coords[n]))

    def write_cells(self):
        fp = self.fp
        connect = self.connect
        nel,ndofs = connect.shape
        
        cellstr = str(ndof) + (" %d" * ndof) + "\n"
        cellstr2 = "%d\n" % cell

        fp.write("CELLS %d %d\n" % (nel, nel + nel*ndof))
        for e in xrange(nel):
            fp.write(cellstr % tuple(connect[e]))
        
        fp.write("CELL_TYPES %d\n" % nel)
        for e in xrange(nel):
            fp.write(cellstr2)
    
    def write_celldata(self, name='epsilon'):
        fp = self.fp
        celldata = self.residuals
        (nel,rank) = celldata.shape
        fp.write("CELL_DATA %d\n" % nel)
        fp.write("SCALARS %s float\n" % name)
        fp.write("LOOKUP_TABLE default\n")
        for e in xrange(nel):
            fp.write("%g\n" % errors[e])


if __name__ == '__main__':

    import numpy
    import tables

    base = 'residuals'
    meshfile = 'cube.h5'

    fp1 = tables.openFile(meshfile, 'r')
    coords = fp1.root.coordinates
    connect = fp1.root.connectivity

    fp2 = tables.openFile(base + '.h5', 'r')
    residuals = fp2.root.epsilon

    writer = VtkWriter()
    writer.open(base + '.vtk')
    writer.set_mesh(coords, connect, VTK_TETRA)
    writer.set_data(residuals)
    writer.write_header()
    writer.write_points()
    writer.write_cells()
    writer.write_celldata()
    writer.close()
