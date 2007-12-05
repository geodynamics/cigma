#!/usr/bin/env python

import numpy
from Cell import Cell

class FiatSimplex(Cell):
    """
    Python object for managing basis functions and quadrature rules
    of a simplex reference finite-element cell using FIAT.
    """
    def __init__(self, name, **kw):
        Cell.__init__(self, name)
        self.degree = kw.get('degree', 1)
        self.order = kw.get('order', -1)
        self.configure()
        return
        
    def configure(self):
        self.shape = self.getShape()
        if self.order == -1:
            self.order = self.degree
        return

    def getShape(self):
        import FIAT.shapes
        shape = None
        name = self.name.lower()
        if name == "tetrahedron":
            shape = FIAT.shapes.TETRAHEDRON
        elif name == "triangle":
            shape = FIAT.shapes.TRIANGLE
        elif name == "line":
            shape == FIAT.shapes.LINE
        return shape

    def setupGeometry(self, spacedim):
        self.geometry = None
        pass # XXX: cell geometry object

    def setupQuadrature(self):
        """
        Setup quadrature rule for reference cell.
        """
        import FIAT.quadrature
        return FIAT.quadrature.make_quadrature_by_degree(self.shape, self.order)

    def setupBasisFns(self):
        """
        Setup basis functions for reference cell
        """
        from FIAT.Lagrange import Lagrange
        return Lagrange(self.shape, self.degree).function_space()

    def setupVertices(self):
        """
        Setup evaulation functional points for reference 
        """
        from FIAT.Lagrange import Lagrange
        return Lagrange(self.shape, self.degree).Udual.pts

    def initialize(self, spaceDim):
        self.setupGeometry(spaceDim)

        quadrature = self.setupQuadrature()
        basisFns = self.setupBasisFns()
        
        # Get coordinates of vertices (dual basis)
        self.vertices = numpy.array(self.setupVertices(), dtype=numpy.float64)

        # Evaluate basis functions at quadrature points
        quadpts = quadrature.get_points()
        basis = numpy.array(basisFns.tabulate(quadpts)).transpose()
        self.basis = numpy.reshape(basis.flatten(), basis.shape)

        # Evaluate derivatives of basis functions at quadrature points
        import FIAT.shapes
        dim = FIAT.shapes.dimension(basisFns.base.shape)
        basisDeriv = numpy.array([basisFns.deriv_all(d).tabulate(quadpts)
                                  for d in range(dim)]).transpose()
        self.basisDeriv = numpy.reshape(basisDeriv.flatten(), basisDeriv.shape)

        self.quadPts = numpy.array(quadrature.get_points())
        self.quadWts = numpy.array(quadrature.get_weights())

        self.cellDim = dim
        self.numCorners = len(basisFns)
        self.numQuadPts = len(quadrature.get_weights())

        return
