#!/usr/bin/env python

import numpy

# read array from temporary file
asdf = numpy.fromfile('asdf', sep=' ')
asdf = asdf.reshape((1001, 17))

# assign meaning to each column
n = asdf[:,0]
t = asdf[:,1]
x = asdf[:,2:5]
y0 = asdf[:,5:8]
y1 = asdf[:,8:11]
rho = asdf[:,11:14]
a = asdf[:,14]
b = asdf[:,15]
r = asdf[:,16]

# pylab example
#plot(t,y0,t,y1)
#legend(('dx','dy','dz','dx2','dy2','dz2'))

