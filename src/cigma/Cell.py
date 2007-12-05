#!/usr/bin/env python

class Cell(object):
    """
    Python abstract base class for managing basis functions
    and quadrature rules of a reference finite-element cell.
    """

    def __init__(self, name):

        self.name = name.lower()

        self.geometry = None
        self.basisVert = None           # basis fns at vertices
        self.basisDerivVert = None      # basis fn derivs at vertices
        self.basisQuad = None           # basis fns at quad pts
        self.basisDerivQuad = None      # basis fn derivs at quad points

        self.quadPts = None     # coordinates of quad pts
        self.quadWts = None     # weights of quad pts

        self.cellDim = None     # dimension of cell
        self.numCorners = None  # number of vertices in cell
        self.numQuadPts = None  # number of quadrature points

        return

    def configure(self):
        pass

