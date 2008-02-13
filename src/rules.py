#!/usr/bin/env python
"""
Write out a number of quadrature rules into an HDF5 file
"""

import tables
from cigmalib.FiatQuadrature import *


def transform_rule(qr):
    def new_qr(order):
        x,w = qr(order)
        x += 1
        x *= 0.5
        return (x,w)
    return new_qr

fp = tables.openFile("rules.h5", "w")

elements = ['line', 'tri', 'quad', 'tet', 'hex']

orders_by_element = {
    'line': range(0,31),
    'tri' : range(0,31),
    'quad': range(2,31),
    'tet' : range(0,31),
    'hex' : range(2,31),
}

rules = {
    'line': line_qr,
    'tri' : transform_rule(tri_qr),
    'quad': quad_qr,
    'tet' : transform_rule(tet_qr),
    'hex' : hex_qr,
}

for elt in elements:
    rule = rules[elt]
    orders = orders_by_element[elt]
    eltGroup = fp.createGroup('/', elt)
    for order in orders:
        x,w = rule(order)
        o = str(order)
        g = fp.createGroup(eltGroup, o)
        fp.createArray(g, 'points', x)
        fp.createArray(g, 'weights', w)
        print "Created /%s/%s" % (elt,o)

fp.close()

