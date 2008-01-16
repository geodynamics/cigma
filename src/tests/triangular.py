#!/usr/bin/env python

import numpy
import tables

from math import sin,cos,exp

def f(x,y,z):
    return 4*exp(-(x-0.5)**2 - (y-0.5)**2 - (z-0.5)**2) + cos(x*y*z)/2

def msh_to_h5(inputfile, outputfile):
    """
    For details on the msh format, visit
    http://www.geuz.org/gmsh/doc/texinfo/gmsh_10.html
    """

    fp = open(inputfile, 'r')
    h5 = tables.openFile(outputfile, 'w')

    #################################################

    # expecting $NOD
    nod_line = fp.readline().strip()
    assert nod_line == '$NOD'

    # number-of-nodes
    nno = int(fp.readline())
    nsd = 3
    coords = numpy.zeros((nno,nsd))

    # node-number x-coord y-coord z-coord
    for i in xrange(nno):
        line = fp.readline()
        n,x,y,z = line.split()
        coords[int(n)-1] = map(float,(x,y,z))

    # expecting $ENDNOD
    endnod_line = fp.readline().strip()
    assert endnod_line == '$ENDNOD'

    # save mesh coords to hdf5 file
    coords_h5 = h5.createArray('/', 'coordinates', coords)


    #################################################

    # expecting $ELM
    elm_line = fp.readline().strip()
    assert elm_line == '$ELM'

    # number-of-elements
    nel = int(fp.readline())

    # elm-number elm-type reg-phys reg-elem number-of-nodes node-number-list
    def parse_elm_columns(a,b,c,d,e,*f):
        return map(int,(a,b,c,d,e)) + [map(int,f)]

    edges = []
    triangles = []
    for i in xrange(nel):
        line = fp.readline()
        cols = line.split()
        elmId, elmType, regPhys, regElem, numNodes, nodeList = parse_elm_columns(*cols)
        assert numNodes == len(nodeList)
        #print elmId, nodeList
        if numNodes == 3:
            assert elmType == 2
            triangles.append(nodeList)
        elif numNodes == 2:
            assert elmType == 1
            edges.append(nodeList)

    connect  = numpy.array(triangles, int)
    connect -= 1    # account for offset

    # expecting $ENDELM
    endelm_line = fp.readline().strip()
    assert endelm_line == '$ENDELM'

    # save mesh connectivity to hdf5 file
    connect_h5 = h5.createArray('/', 'connectivity', connect)


    #################################################

    dataset = numpy.zeros((nno,1))
    for i in xrange(nno):
        x,y,z = coords[i]
        dataset[i] = f(x,y,z)
    dataset_h5 = h5.createArray('/', 'dataset', dataset)

    #################################################

    # clean up
    fp.close()
    h5.close()


def main():
    for i in (1,2,3):
        prefix = 'triangular%d' % i
        print 'Processing', prefix
        msh_to_h5(prefix + '.msh', prefix + '.h5')

if __name__ == '__main__':
    main()

