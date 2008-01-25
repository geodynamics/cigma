#!/usr/bin/env python


import os
from time import time


elements = ['tet4', 'hex8']
resolutions = ['1000m', '0500m']

cases = []
for e in elements:
    for r in resolutions:
        cases.append('%s_%s' % (e,r))


pathstr = './strikeslip_%s_t0.vtk:displacements_t0'
outputstr = './residuals%d%d-%s-%s.vtk'

comparisons = []
for i,a in enumerate(cases):
    for j,b in enumerate(cases):
        input1 = pathstr % a
        input2 = pathstr % b
        output = outputstr % (i,j,a,b)
        comparison = dict(first=input1, second=input2, output=output, case=(i,j))
        comparisons.append(comparison)


commandstr = ' '.join([
    '../cigma', 'compare',
    '--output=%(output)s',
    '--first=%(first)s',
    '--second=%(second)s',
    '--verbose',
    '--output-frequency=1000',
])

t0 = time()
for comparison in comparisons:
    i,j = comparison['case']
    print "Case %d%d" % (i,j)
    print "Comparing %s with %s" % (cases[i], cases[j])
    command = commandstr % comparison
    print command
    os.system(command)
    print "-" * 78
t1 = time()

dt = (t1 - t0)/60.0

print "Total: %lf mins" % dt

