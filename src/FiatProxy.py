#!/usr/bin/env
#
# Wrapper functions called from FiatProxy C++ object
#

from cigma.api import quadrature as Q
from cigma.api import tabulation as T

def quadrature(shape, order):
    return Q(shape, order)


def tabulation(shape, degree, x):
    return T(shape, degree, x)

