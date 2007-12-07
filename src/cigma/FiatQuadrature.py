#!/usr/bin/env python
import FIAT.shapes
from FIAT.quadrature import make_quadrature_by_degree
from numpy import array, zeros
from FiatShapes import *


def make_rule(shape):
    def qr(order):
        q = make_quadrature_by_degree(shape, order)
        x = array(q.get_points())
        w = array(q.get_weights())
        return (x,w)
    return qr

line_qr = make_rule(FIAT.shapes.LINE)
tri_qr = make_rule(FIAT.shapes.TRIANGLE)
tet_qr = make_rule(FIAT.shapes.TETRAHEDRON)

def quad_qr(order):
    qpts,qwts = line_qr(order)
    nq = len(qpts)
    x = zeros((nq*nq, 2))
    w = zeros((nq*nq,))
    n = 0
    # Bottom
    for r in range(0,nq-1):
        x[n,0] = qpts[r]
        x[n,1] = qpts[0]
        w[n] = qwts[r]*qwts[0]
        n += 1
    # Right
    for q in range(0,nq-1):
        x[n,0] = qpts[nq-1]
        x[n,1] = qpts[q]
        w[n] = qwts[nq-1]*qwts[q]
        n += 1
    # Top
    for r in range(nq-1, 0, -1):
        x[n,0] = qpts[r]
        x[n,1] = qpts[nq-1]
        w[n] = qwts[r]*qwts[nq-1]
        n += 1
    # Left
    for q in range(nq-1, 0, -1):
        x[n,0] = qpts[0]
        x[n,1] = qpts[q]
        w[n] = qwts[0]*qwts[q]
        n += 1
    # Interior
    for q in range(1, nq-1):
        for r in range(1, nq-1):
            x[n,0] = qpts[r]
            x[n,1] = qpts[q]
            w[n] = qwts[r]*qwts[q]
            n += 1
    assert (n == nq*nq*nq)
    return (x,w)


def hex_qr(order):
    qpts,qwts = line_qr(order)
    nq = len(qpts)
    x = zeros((nq*nq*nq, 3))
    w = zeros((nq*nq*nq,))
    n = 0
    for s in range(nq):
        # Bottom
        for r in range(0,nq-1):
            x[n,0] = qpts[r]
            x[n,1] = qpts[0]
            x[n,2] = qpts[s]
            w[n] = qwts[r]*qwts[0]*qwts[s]
            n += 1
        # Right
        for q in range(0,nq-1):
            x[n,0] = qpts[nq-1]
            x[n,1] = qpts[q]
            x[n,2] = qpts[s]
            w[n] = qwts[nq-1]*qwts[q]*qwts[s]
            n += 1
        # Top
        for r in range(nq-1, 0, -1):
            x[n,0] = qpts[r]
            x[n,1] = qpts[nq-1]
            x[n,2] = qpts[s]
            w[n] = qwts[r]*qwts[nq-1]*qwts[s]
            n += 1
        # Left
        for q in range(nq-1, 0, -1):
            x[n,0] = qpts[0]
            x[n,1] = qpts[q]
            x[n,2] = qpts[s]
            w[n] = qwts[0]*qwts[q]*qwts[s]
            n += 1
        # Interior
        for q in range(1, nq-1):
            for r in range(1, nq-1):
                x[n,0] = qpts[r]
                x[n,1] = qpts[q]
                x[n,2] = qpts[s]
                w[n] = qwts[r]*qwts[q]*qwts[s]
                n += 1
    assert (n == nq*nq*nq)
    return (x,w)


def quadrature(shape, order):
    qr = {
        LINE: line_qr,
        TRI: tri_qr,
        TETRA: tet_qr,
        QUAD: quad_qr,
        HEX: hex_qr,
    }.get(shape)
    return qr(order)

