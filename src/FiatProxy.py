#!/usr/bin/env
#
# Wrapper functions called from FiatProxy C++ object
#

from cigmalib.api import quadrature as Q
from cigmalib.api import tabulate as T

def test(msg, n):
    for i in xrange(n):
        print msg
    return 42

def quadrature(shape, order):
    return Q(shape, order)


def tabulate(shape, degree, x):
    return T(shape, degree, x)

