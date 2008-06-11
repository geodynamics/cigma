#!/usr/bin/env python

import numpy

X08 = numpy.fromfile('X08.dat', sep='\n')
X16 = numpy.fromfile('X16.dat', sep='\n')
X32 = numpy.fromfile('X32.dat', sep='\n')
X64 = numpy.fromfile('X64.dat', sep='\n')


def nodemap(n):
    def nid(i,j,k):
        return n*n*i + n*j + k
    return nid

def cellmap(n):
    def cid(i,j,k):
        return (n-1)*(n-1)*i + (n-1)*j + k
    return cid

def cubemesh(x):
    n = len(x)
    N = nodemap(n)
    C = cellmap(n)
    coords = numpy.zeros((n*n*n,3),float)
    connect = numpy.zeros(((n-1)*(n-1)*(n-1),8),int)
    for i in xrange(n):
        for j in xrange(n):
            for k in xrange(n):
                coords[N(i,j,k)] = (x[i],x[j],x[k])

    for i in xrange(n-1):
        for j in xrange(n-1):
            for k in xrange(n-1):
                c = C(i,j,k)
                
                connect[c,0] = N(i + 0, j + 0, k + 0)
                connect[c,1] = N(i + 1, j + 0, k + 0)
                connect[c,2] = N(i + 1, j + 1, k + 0)
                connect[c,3] = N(i + 0, j + 1, k + 0)

                connect[c,4] = N(i + 0, j + 0, k + 1)
                connect[c,5] = N(i + 1, j + 0, k + 1)
                connect[c,6] = N(i + 1, j + 1, k + 1)
                connect[c,7] = N(i + 0, j + 1, k + 1)

    return (coords, connect)


def save(fname, y, fmt='%.8e'):
    m,n = y.shape
    fp = open(fname, 'w')
    fp.write("%d %d\n" % (m,n))
    for i in xrange(m):
        for j in xrange(n):
            fp.write(fmt % y[i,j])
            fp.write(' ')
        fp.write('\n')
    fp.close()

for x in (X08, X16, X32, X64):
    e = x.shape[0] - 1
    M = cubemesh(x)
    save('coords%02d.dat' % e, M[0])
    save('connect%02d.dat' % e, M[1], fmt='%d')

