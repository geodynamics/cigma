import numpy

def write_tet4_vtk(filename, coords, connect, errors):
    """Write out cell-based local errors on given tetrahedral mesh"""

    (nno, nsd) = coords.shape
    (nel, ndof) = connect.shape
    assert errors.shape[0] == nel

    fp = open(filename, "w")
    print "Creating", fp.name

    fp.write("# vtk DataFile Version 3.0\n")
    fp.write("This line is a comment\n")
    fp.write("ASCII\n")

    fp.write("DATASET UNSTRUCTURED_GRID\n")

    fp.write("POINTS %d float\n" % nno)
    for n in xrange(nno):
        fp.write("%g %g %g\n" % tuple(coords[n]))

    cellstr = str(ndof) + (" %d" * ndof) + "\n"
    fp.write("CELLS %d %d\n" % (nel, nel + nel*ndof))
    for e in xrange(nel):
        fp.write(cellstr % tuple(connect[e]))

    fp.write("CELL_TYPES %d\n" % nel)
    for e in xrange(nel):
        fp.write("10\n")
    
    fp.write("CELL_DATA %d\n" % nel)
    fp.write("SCALARS LocalResiduals float\n")
    fp.write("LOOKUP_TABLE default\n")
    for e in xrange(nel):
        fp.write("%g\n" % errors[e])

    fp.close()
