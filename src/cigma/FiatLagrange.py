#!/usr/bin/env python

import numpy
from Cell import Cell

class FiatLagrange(Cell):
    """
    Python object for managing basis functions and quadrature rules of a
    Lagrange reference finite-element cell using FIAT.
    """

    def __init__(self, name, **kw):
        Cell.__init__(self, name)
        self.cellDim = kw.get('dimension', 3)   # dimension of finite element cell
        self.degree = kw.get('degree', 1)       # degree of finite-element cell
        self.order = kw.get('order', -1)        # order of finite-element cell
        self.configure()
        return
    
    def configure(self):
        Cell.configure(self)
        if self.order == -1:
            self.order = 2*(self.degree) + 1
        return

    def setupGeometry(self, spaceDim):
        self.geometry = None
        pass # XXX: cell geometry object

    def setupQuadrature(self):
        """
        Setup quadrature rule for reference cell.
        """
        from FIAT.quadrature import make_quadrature_by_degree
        import FIAT.shapes
        return make_quadrature_by_degree(FIAT.shapes.LINE, self.order)

    def setupElement(self):
        """
        Setup the finite element for reference cell
        """
        from FIAT.Lagrange import Lagrange
        import FIAT.shapes
        return Lagrange(FIAT.shapes.LINE, self.degree)

    def setupVertices(self, element):
        """
        Setup evaluation functional points for reference cell.
        """
        return element.Udual.pts

    def initialize(self, spaceDim):
        """
        Initialize reference finite-element cell from a tensor
        product of 1-D Lagrange elements.
        """
        self.setupGeometry(spaceDim)

        assert self.cellDim > 0
        
        quadrature = self.setupQuadrature()
        element = self.setupElement()
        dim = self.cellDim
    
        # Get coordinates of vertices (dual basis)
        vertices = numpy.array(self.setupVertices(element))

        # Evaluate basis functions at quadrature points
        quadpts = numpy.array(quadrature.get_points())
        quadwts = numpy.array(quadrature.get_weights())
        numQuadPts = len(quadpts)
        basis = numpy.array(element.function_space().tabulate(quadrature.get_points())).transpose()
        numBasisFns = len(element.function_space())

        # Evaluate derivatives of basis functions at quadrature points
        basisDeriv = numpy.array([element.function_space().deriv_all(d).tabulate(quadrature.get_points())
                                  for d in range(1)]).transpose()
        self.numQuadPts = numQuadPts ** dim
        self.numCorners = numBasisFns ** dim

        if dim == 1:
            #######################################################################
            self.vertices = numpy.array(vertices)
            self.quadPts = quadpts
            self.quadWts = quadwts
            self.basis = numpy.reshape(basis.flatten(), basis.shape)
            self.basisDeriv = numpy.reshape(basisDeriv.flatten(), basisDeriv.shape)
        elif dim == 2:
            #######################################################################
            v = self.vertices = numpy.zeros((self.numCorners, dim))
            n = 0
            # Bottom
            for r in range(0, numBasisFns-1):
                v[n,0] = vertices[r]
                v[n,1] = vertices[0]
                n += 1
            # Right
            for q in range(0, numBasisFns-1):
                v[n,0] = vertices[numBasisFns-1]
                v[n,1] = vertices[q]
                n += 1
            # Top
            for r in range(numBasisFns-1, 0, -1):
                v[n,0] = vertices[r]
                v[n,1] = vertices[numBasisFns-1]
                n += 1
            # Left
            for q in range(numBasisFns-1, 0, -1):
                v[n,0] = vertices[0]
                v[n,1] = vertices[q]
            # Interior
            for q in range(1, numBasisFns-1):
                for r in range(1, numBasisFns-1):
                    v[n,0] = vertices[r]
                    v[n,1] = vertices[q]
                    n += 1

            if n != self.numCorners:
                raise RuntimeError("Invalid 2D function tabulation")

            qp = self.quadPts = numpy.zeros((self.numQuadPts, dim))
            qw = self.quadWts = numpy.zeros((self.numQuadPts,))
            bs = self.basis = numpy.zeros((self.numQuadPts, self.numCorners))
            dbs = self.basisDeriv = numpy.zeros((self.numQuadPts, self.numCorners, dim))

            n = 0
            # Bottom ############
            for r in range(0, numQuadPts-1):
                self.quadPts[n][0] = quadpts[r]
                self.quadPts[n][1] = quadpts[0]
                self.quadWts[n]    = quadwts[r]*quadwts[0]
                m = 0
                # Bottom ####
                for g in range(0, numBasisFns-1):
                    self.basis[n][m] = basis[r][g]*basis[0][0]
                    self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[0][0]
                    self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[0][0][0]
                    m += 1
                # Right ####
                for f in range(0, numBasisFns-1):
                    self.basis[n][m] = basis[r][numBasisFns-1]*basis[0][f]
                    self.basisDeriv[n][m][0] = basisDeriv[r][numBasisFns-1][0]*basis[0][f]
                    self.basisDeriv[n][m][1] = basis[r][numBasisFns-1]*basisDeriv[0][f][0]
                    m += 1
                # Top ####
                for g in range(numBasisFns-1, 0, -1):
                    self.basis[n][m] = basis[r][g]*basis[0][numBasisFns-1]
                    self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[0][numBasisFns-1]
                    self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[0][numBasisFns-1][0]
                    m += 1
                # Left ####
                for f in range(numBasisFns-1, 0, -1):
                    self.basis[n][m] = basis[r][0]*basis[0][f]
                    self.basisDeriv[n][m][0] = basisDeriv[r][0][0]*basis[0][f]
                    self.basisDeriv[n][m][1] = basis[r][0]*basisDeriv[0][f][0]
                    m += 1
                # Interior ####
                for f in range(1, numBasisFns-1):
                    for g in range(1, numBasisFns-1):
                        self.basis[n][m] = basis[r][g]*basis[0][f]
                        self.basisDeriv[0][r][f][g][0] = basisDeriv[r][g][0]*basis[0][f]
                        self.basisDeriv[0][r][f][g][1] = basis[r][g]*basisDeriv[0][f][0]
                        m += 1
                if not m == self.numCorners:
                    raise RuntimeError('Invalid 2D function tabulation')
                n += 1
            # Right ############
            for q in range(0, numQuadPts-1):
                self.quadPts[n][0] = quadpts[numQuadPts-1]
                self.quadPts[n][1] = quadpts[q]
                self.quadWts[n]    = quadwts[numQuadPts-1]*quadwts[q]
                m = 0
                # Bottom ####
                for g in range(0, numBasisFns-1):
                    self.basis[n][m] = basis[numQuadPts-1][g]*basis[q][0]
                    self.basisDeriv[n][m][0] = basisDeriv[numQuadPts-1][g][0]*basis[q][0]
                    self.basisDeriv[n][m][1] = basis[numQuadPts-1][g]*basisDeriv[q][0][0]
                    m += 1
                # Right ####
                for f in range(0, numBasisFns-1):
                    self.basis[n][m] = basis[numQuadPts-1][numBasisFns-1]*basis[q][f]
                    self.basisDeriv[n][m][0] = basisDeriv[numQuadPts-1][numBasisFns-1][0]*basis[q][f]
                    self.basisDeriv[n][m][1] = basis[numQuadPts-1][numBasisFns-1]*basisDeriv[q][f][0]
                    m += 1
                # Top ####
                for g in range(numBasisFns-1, 0, -1):
                    self.basis[n][m] = basis[numQuadPts-1][g]*basis[q][numBasisFns-1]
                    self.basisDeriv[n][m][0] = basisDeriv[numQuadPts-1][g][0]*basis[q][numBasisFns-1]
                    self.basisDeriv[n][m][1] = basis[numQuadPts-1][g]*basisDeriv[q][numBasisFns-1][0]
                    m += 1
                # Left ####
                for f in range(numBasisFns-1, 0, -1):
                    self.basis[n][m] = basis[numQuadPts-1][0]*basis[q][f]
                    self.basisDeriv[n][m][0] = basisDeriv[numQuadPts-1][0][0]*basis[q][f]
                    self.basisDeriv[n][m][1] = basis[numQuadPts-1][0]*basisDeriv[q][f][0]
                    m += 1
                # Interior ####
                for f in range(1, numBasisFns-1):
                  for g in range(1, numBasisFns-1):
                    self.basis[n][m] = basis[numQuadPts-1][g]*basis[0][f]
                    self.basisDeriv[q][numQuadPts-1][f][g][0] = basisDeriv[numQuadPts-1][g][0]*basis[q][f]
                    self.basisDeriv[q][numQuadPts-1][f][g][1] = basis[numQuadPts-1][g]*basisDeriv[q][f][0]
                    m += 1
                if not m == self.numCorners:
                    raise RuntimeError('Invalid 2D function tabulation')
                n += 1
            # Top ############
            for r in range(numQuadPts-1, 0, -1):
                self.quadPts[n][0] = quadpts[r]
                self.quadPts[n][1] = quadpts[numQuadPts-1]
                self.quadWts[n]    = quadwts[r]*quadwts[numQuadPts-1]
                m = 0
                # Bottom ####
                for g in range(0, numBasisFns-1):
                    self.basis[n][m] = basis[r][g]*basis[numQuadPts-1][0]
                    self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[numQuadPts-1][0]
                    self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[numQuadPts-1][0][0]
                    m += 1
                # Right ####
                for f in range(0, numBasisFns-1):
                    self.basis[n][m] = basis[r][numBasisFns-1]*basis[numQuadPts-1][f]
                    self.basisDeriv[n][m][0] = basisDeriv[r][numBasisFns-1][0]*basis[numQuadPts-1][f]
                    self.basisDeriv[n][m][1] = basis[r][numBasisFns-1]*basisDeriv[numQuadPts-1][f][0]
                    m += 1
                # Top ####
                for g in range(numBasisFns-1, 0, -1):
                    self.basis[n][m] = basis[r][g]*basis[numQuadPts-1][numBasisFns-1]
                    self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[numQuadPts-1][numBasisFns-1]
                    self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[numQuadPts-1][numBasisFns-1][0]
                    m += 1
                # Left ####
                for f in range(numBasisFns-1, 0, -1):
                    self.basis[n][m] = basis[r][0]*basis[numQuadPts-1][f]
                    self.basisDeriv[n][m][0] = basisDeriv[r][0][0]*basis[numQuadPts-1][f]
                    self.basisDeriv[n][m][1] = basis[r][0]*basisDeriv[numQuadPts-1][f][0]
                    m += 1
                # Interior ####
                for f in range(1, numBasisFns-1):
                    for g in range(1, numBasisFns-1):
                        self.basis[n][m] = basis[r][g]*basis[numQuadPts-1][f]
                        self.basisDeriv[numQuadPts-1][r][f][g][0] = basisDeriv[r][g][0]*basis[numQuadPts-1][f]
                        self.basisDeriv[numQuadPts-1][r][f][g][1] = basis[r][g]*basisDeriv[numQuadPts-1][f][0]
                        m += 1
                if not m == self.numCorners:
                    raise RuntimeError('Invalid 2D function tabulation')
                n += 1
            # Left ############
            for q in range(numQuadPts-1, 0, -1):
                self.quadPts[n][0] = quadpts[0]
                self.quadPts[n][1] = quadpts[q]
                self.quadWts[n]    = quadwts[0]*quadwts[q]
                m = 0
                # Bottom ##
                for g in range(0, numBasisFns-1):
                    self.basis[n][m] = basis[0][g]*basis[q][0]
                    self.basisDeriv[n][m][0] = basisDeriv[0][g][0]*basis[q][0]
                    self.basisDeriv[n][m][1] = basis[0][g]*basisDeriv[q][0][0]
                    m += 1
                # Right
                for f in range(0, numBasisFns-1):
                    self.basis[n][m] = basis[0][numBasisFns-1]*basis[q][f]
                    self.basisDeriv[n][m][0] = basisDeriv[0][numBasisFns-1][0]*basis[q][f]
                    self.basisDeriv[n][m][1] = basis[0][numBasisFns-1]*basisDeriv[q][f][0]
                    m += 1
                # Top
                for g in range(numBasisFns-1, 0, -1):
                    self.basis[n][m] = basis[0][g]*basis[q][numBasisFns-1]
                    self.basisDeriv[n][m][0] = basisDeriv[0][g][0]*basis[q][numBasisFns-1]
                    self.basisDeriv[n][m][1] = basis[0][g]*basisDeriv[q][numBasisFns-1][0]
                    m += 1
                # Left
                for f in range(numBasisFns-1, 0, -1):
                    self.basis[n][m] = basis[0][0]*basis[q][f]
                    self.basisDeriv[n][m][0] = basisDeriv[0][0][0]*basis[q][f]
                    self.basisDeriv[n][m][1] = basis[0][0]*basisDeriv[q][f][0]
                    m += 1
                # Interior
                for f in range(1, numBasisFns-1):
                    for g in range(1, numBasisFns-1):
                        self.basis[n][m] = basis[0][g]*basis[0][f]
                        self.basisDeriv[q][0][f][g][0] = basisDeriv[0][g][0]*basis[q][f]
                        self.basisDeriv[q][0][f][g][1] = basis[0][g]*basisDeriv[q][f][0]
                        m += 1
                if not m == self.numCorners:
                    raise RuntimeError('Invalid 2D function tabulation')
                n += 1
            # Interior ############
            for q in range(1, numQuadPts-1):
                for r in range(1, numQuadPts-1):
                    self.quadPts[n][0] = quadpts[r]
                    self.quadPts[n][1] = quadpts[q]
                    self.quadWts[n]    = quadwts[r]*quadwts[q]
                    m = 0
                    # Bottom
                    for g in range(0, numBasisFns-1):
                        self.basis[n][m] = basis[r][g]*basis[q][0]
                        self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[q][0]
                        self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[q][0][0]
                        m += 1
                    # Right
                    for f in range(0, numBasisFns-1):
                        self.basis[n][m] = basis[r][numBasisFns-1]*basis[q][f]
                        self.basisDeriv[n][m][0] = basisDeriv[r][numBasisFns-1][0]*basis[q][f]
                        self.basisDeriv[n][m][1] = basis[r][numBasisFns-1]*basisDeriv[q][f][0]
                        m += 1
                    # Top
                    for g in range(numBasisFns-1, 0, -1):
                        self.basis[n][m] = basis[r][g]*basis[q][numBasisFns-1]
                        self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[q][numBasisFns-1]
                        self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[q][numBasisFns-1][0]
                        m += 1
                    # Left
                    for f in range(numBasisFns-1, 0, -1):
                        self.basis[n][m] = basis[r][0]*basis[q][f]
                        self.basisDeriv[n][m][0] = basisDeriv[r][0][0]*basis[q][f]
                        self.basisDeriv[n][m][1] = basis[r][0]*basisDeriv[q][f][0]
                        m += 1
                    # Interior
                    for f in range(1, numBasisFns-1):
                        for g in range(1, numBasisFns-1):
                            self.basis[n][m] = basis[r][g]*basis[q][f]
                            self.basisDeriv[q][r][f][g][0] = basisDeriv[r][g][0]*basis[q][f]
                            self.basisDeriv[q][r][f][g][1] = basis[r][g]*basisDeriv[q][f][0]
                            m += 1
                    if not m == self.numCorners:
                        raise RuntimeError('Invalid 2D function tabulation')
                    n += 1
            if not n == self.numQuadPts:
                raise RuntimeError('Invalid 2D quadrature')

            self.vertices = numpy.reshape(self.vertices, (self.numCorners, dim))
            self.quadPts = numpy.reshape(self.quadPts, (self.numQuadPts, dim))
            self.quadWts = numpy.reshape(self.quadWts, (self.numQuadPts))
            self.basis = numpy.reshape(self.basis, (self.numQuadPts, self.numCorners))
            self.basisDeriv = numpy.reshape(self.basisDeriv, (self.numQuadPts, self.numCorners, dim))

        elif dim == 3:
            #######################################################################

            self.vertices = numpy.zeros((self.numCorners, dim))

            n = 0
            # Depth
            for s in range(numBasisFns):
                # Bottom
                for r in range(0, numBasisFns-1):
                    self.vertices[n][0] = vertices[r]
                    self.vertices[n][1] = vertices[0]
                    self.vertices[n][2] = vertices[s]
                    n += 1
                # Right
                for q in range(0, numBasisFns-1):
                    self.vertices[n][0] = vertices[numBasisFns-1]
                    self.vertices[n][1] = vertices[q]
                    self.vertices[n][2] = vertices[s]
                    n += 1
                # Top
                for r in range(numBasisFns-1, 0, -1):
                    self.vertices[n][0] = vertices[r]
                    self.vertices[n][1] = vertices[numBasisFns-1]
                    self.vertices[n][2] = vertices[s]
                    n += 1
                # Left
                for q in range(numBasisFns-1, 0, -1):
                    self.vertices[n][0] = vertices[0]
                    self.vertices[n][1] = vertices[q]
                    self.vertices[n][2] = vertices[s]
                    n += 1
                # Interior
                for q in range(1, numBasisFns-1):
                    for r in range(1, numBasisFns-1):
                        self.vertices[n][0] = vertices[r]
                        self.vertices[n][1] = vertices[q]
                        self.vertices[n][2] = vertices[s]
                        n += 1

            if not n == self.numCorners:
                raise RuntimeError('Invalid 3D function tabulation: '+str(n)+' should be '+str(self.numCorners))

            self.quadPts = numpy.zeros((numQuadPts*numQuadPts*numQuadPts, dim))
            self.quadWts = numpy.zeros((numQuadPts*numQuadPts*numQuadPts,))
            self.basis   = numpy.zeros((numQuadPts*numQuadPts*numQuadPts,
                                        numBasisFns*numBasisFns*numBasisFns))
            self.basisDeriv = numpy.zeros((numQuadPts*numQuadPts*numQuadPts,
                                           numBasisFns*numBasisFns*numBasisFns,
                                           dim))
            n = 0
            # Depth
            for s in range(numQuadPts):
                # Bottom
                for r in range(0, numQuadPts-1):
                    self.quadPts[n][0] = quadpts[r]
                    self.quadPts[n][1] = quadpts[0]
                    self.quadPts[n][2] = quadpts[s]
                    self.quadWts[n]    = quadwts[r]*quadwts[0]*quadwts[s]
                    m = 0
                    for h in range(numBasisFns):
                        # Bottom
                        for g in range(0, numBasisFns-1):
                            self.basis[n][m] = basis[r][g]*basis[0][0]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[0][0]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[0][0][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[r][g]*basis[0][0]*basisDeriv[s][h][0]
                            m += 1
                        # Right
                        for f in range(0, numBasisFns-1):
                            self.basis[n][m] = basis[r][numBasisFns-1]*basis[0][f]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[r][numBasisFns-1][0]*basis[0][f]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[r][numBasisFns-1]*basisDeriv[0][f][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[r][numBasisFns-1]*basis[0][f]*basisDeriv[s][h][0]
                            m += 1
                        # Top
                        for g in range(numBasisFns-1, 0, -1):
                            self.basis[n][m] = basis[r][g]*basis[0][numBasisFns-1]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[0][numBasisFns-1]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[0][numBasisFns-1][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[r][g]*basis[0][numBasisFns-1]*basisDeriv[s][h][0]
                            m += 1
                        # Left
                        for f in range(numBasisFns-1, 0, -1):
                            self.basis[n][m] = basis[r][0]*basis[0][f]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[r][0][0]*basis[0][f]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[r][0]*basisDeriv[0][f][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[r][0]*basis[0][f]*basisDeriv[s][h][0]
                            m += 1
                        # Interior
                        for f in range(1, numBasisFns-1):
                            for g in range(1, numBasisFns-1):
                                self.basis[n][m] = basis[r][g]*basis[0][f]*basis[s][h]
                                self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[0][f]*basis[s][h]
                                self.basisDeriv[m][m][1] = basis[r][g]*basisDeriv[0][f][0]*basis[s][h]
                                self.basisDeriv[n][m][2] = basis[r][g]*basis[0][f]*basisDeriv[s][h][0]
                                m += 1
                    if not m == self.numCorners:
                        raise RuntimeError('Invalid 3D function tabulation')
                    n += 1
                # Right
                for q in range(0, numQuadPts-1):
                    self.quadPts[n][0] = quadpts[numQuadPts-1]
                    self.quadPts[n][1] = quadpts[q]
                    self.quadPts[n][2] = quadpts[s]
                    self.quadWts[n]    = quadwts[numQuadPts-1]*quadwts[q]*quadwts[s]
                    m = 0
                    for h in range(numBasisFns):
                        # Bottom
                        for g in range(0, numBasisFns-1):
                            self.basis[n][m] = basis[numQuadPts-1][g]*basis[q][0]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[numQuadPts-1][g][0]*basis[q][0]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[numQuadPts-1][g]*basisDeriv[q][0][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[numQuadPts-1][g]*basis[q][0]*basisDeriv[s][h][0]
                            m += 1
                        # Right
                        for f in range(0, numBasisFns-1):
                            self.basis[n][m] = basis[numQuadPts-1][numBasisFns-1]*basis[q][f]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[numQuadPts-1][numBasisFns-1][0]*basis[q][f]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[numQuadPts-1][numBasisFns-1]*basisDeriv[q][f][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[numQuadPts-1][numBasisFns-1]*basis[q][f]*basisDeriv[s][h][0]
                            m += 1
                        # Top
                        for g in range(numBasisFns-1, 0, -1):
                            self.basis[n][m] = basis[numQuadPts-1][g]*basis[q][numBasisFns-1]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[numQuadPts-1][g][0]*basis[q][numBasisFns-1]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[numQuadPts-1][g]*basisDeriv[q][numBasisFns-1][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[numQuadPts-1][g]*basis[q][numBasisFns-1]*basisDeriv[s][h][0]
                            m += 1
                        # Left
                        for f in range(numBasisFns-1, 0, -1):
                            self.basis[n][m] = basis[numQuadPts-1][0]*basis[q][f]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[numQuadPts-1][0][0]*basis[q][f]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[numQuadPts-1][0]*basisDeriv[q][f][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[numQuadPts-1][0]*basis[q][f]*basisDeriv[s][h][0]
                            m += 1
                        # Interior
                        for f in range(1, numBasisFns-1):
                            for g in range(1, numBasisFns-1):
                                self.basis[n][m] = basis[numQuadPts-1][g]*basis[q][f]*basis[s][h]
                                self.basisDeriv[n][m][0] = basisDeriv[numQuadPts-1][g][0]*basis[q][f]*basis[s][h]
                                self.basisDeriv[m][m][1] = basis[numQuadPts-1][g]*basisDeriv[q][f][0]*basis[s][h]
                                self.basisDeriv[n][m][2] = basis[numQuadPts-1][g]*basis[q][f]*basisDeriv[s][h][0]
                                m += 1
                    if not m == self.numCorners:
                        raise RuntimeError('Invalid 3D function tabulation')
                    n += 1
                # Top
                for r in range(numQuadPts-1, 0, -1):
                    self.quadPts[n][0] = quadpts[r]
                    self.quadPts[n][1] = quadpts[numQuadPts-1]
                    self.quadPts[n][2] = quadpts[s]
                    self.quadWts[n]    = quadwts[r]*quadwts[numQuadPts-1]*quadwts[s]
                    m = 0
                    for h in range(numBasisFns):
                        # Bottom
                        for g in range(0, numBasisFns-1):
                            self.basis[n][m] = basis[r][g]*basis[numQuadPts-1][0]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[numQuadPts-1][0]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[numQuadPts-1][0][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[r][g]*basis[numQuadPts-1][0]*basisDeriv[s][h][0]
                            m += 1
                        # Right
                        for f in range(0, numBasisFns-1):
                            self.basis[n][m] = basis[r][numBasisFns-1]*basis[numQuadPts-1][f]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[r][numBasisFns-1][0]*basis[numQuadPts-1][f]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[r][numBasisFns-1]*basisDeriv[numQuadPts-1][f][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[r][numBasisFns-1]*basis[numQuadPts-1][f]*basisDeriv[s][h][0]
                            m += 1
                        # Top
                        for g in range(numBasisFns-1, 0, -1):
                            self.basis[n][m] = basis[r][g]*basis[numQuadPts-1][numBasisFns-1]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[numQuadPts-1][numBasisFns-1]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[numQuadPts-1][numBasisFns-1][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[r][g]*basis[numQuadPts-1][numBasisFns-1]*basisDeriv[s][h][0]
                            m += 1
                        # Left
                        for f in range(numBasisFns-1, 0, -1):
                            self.basis[n][m] = basis[r][0]*basis[numQuadPts-1][f]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[r][0][0]*basis[numQuadPts-1][f]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[r][0]*basisDeriv[numQuadPts-1][f][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[r][0]*basis[numQuadPts-1][f]*basisDeriv[s][h][0]
                            m += 1
                        # Interior
                        for f in range(1, numBasisFns-1):
                            for g in range(1, numBasisFns-1):
                                self.basis[n][m] = basis[r][g]*basis[numQuadPts-1][f]*basis[s][h]
                                self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[numQuadPts-1][f]*basis[s][h]
                                self.basisDeriv[m][m][1] = basis[r][g]*basisDeriv[numQuadPts-1][f][0]*basis[s][h]
                                self.basisDeriv[n][m][2] = basis[r][g]*basis[numQuadPts-1][f]*basisDeriv[s][h][0]
                                m += 1
                    if not m == self.numCorners:
                        raise RuntimeError('Invalid 3D function tabulation')
                    n += 1
                # Left
                for q in range(numQuadPts-1, 0, -1):
                    self.quadPts[n][0] = quadpts[0]
                    self.quadPts[n][1] = quadpts[q]
                    self.quadPts[n][2] = quadpts[s]
                    self.quadWts[n]    = quadwts[0]*quadwts[q]*quadwts[s]
                    m = 0
                    for h in range(numBasisFns):
                        # Bottom
                        for g in range(0, numBasisFns-1):
                            self.basis[n][m] = basis[0][g]*basis[q][0]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[0][g][0]*basis[q][0]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[0][g]*basisDeriv[q][0][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[0][g]*basis[q][0]*basisDeriv[s][h][0]
                            m += 1
                        # Right
                        for f in range(0, numBasisFns-1):
                            self.basis[n][m] = basis[0][numBasisFns-1]*basis[q][f]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[0][numBasisFns-1][0]*basis[q][f]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[0][numBasisFns-1]*basisDeriv[q][f][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[0][numBasisFns-1]*basis[q][f]*basisDeriv[s][h][0]
                            m += 1
                        # Top
                        for g in range(numBasisFns-1, 0, -1):
                            self.basis[n][m] = basis[0][g]*basis[q][numBasisFns-1]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[0][g][0]*basis[q][numBasisFns-1]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[0][g]*basisDeriv[q][numBasisFns-1][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[0][g]*basis[q][numBasisFns-1]*basisDeriv[s][h][0]
                            m += 1
                        # Left
                        for f in range(numBasisFns-1, 0, -1):
                            self.basis[n][m] = basis[0][0]*basis[q][f]*basis[s][h]
                            self.basisDeriv[n][m][0] = basisDeriv[0][0][0]*basis[q][f]*basis[s][h]
                            self.basisDeriv[n][m][1] = basis[0][0]*basisDeriv[q][f][0]*basis[s][h]
                            self.basisDeriv[n][m][2] = basis[0][0]*basis[q][f]*basisDeriv[s][h][0]
                            m += 1
                        # Interior
                        for f in range(1, numBasisFns-1):
                            for g in range(1, numBasisFns-1):
                                self.basis[n][m] = basis[0][g]*basis[q][f]*basis[s][h]
                                self.basisDeriv[n][m][0] = basisDeriv[0][g][0]*basis[q][f]*basis[s][h]
                                self.basisDeriv[m][m][1] = basis[0][g]*basisDeriv[q][f][0]*basis[s][h]
                                self.basisDeriv[n][m][2] = basis[0][g]*basis[q][f]*basisDeriv[s][h][0]
                                m += 1
                    if not m == self.numCorners:
                        raise RuntimeError('Invalid 3D function tabulation')
                    n += 1
                # Interior
                for q in range(1, numQuadPts-1):
                    for r in range(1, numQuadPts-1):
                        self.quadPts[n][0] = quadpts[r]
                        self.quadPts[n][1] = quadpts[q]
                        self.quadPts[n][2] = quadpts[s]
                        self.quadWts[n]    = quadwts[r]*quadwts[q]*quadwts[s]
                        m = 0
                        for h in range(numBasisFns):
                            # Bottom
                            for g in range(0, numBasisFns-1):
                                self.basis[n][m] = basis[r][g]*basis[q][0]*basis[s][h]
                                self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[q][0]*basis[s][h]
                                self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[q][0][0]*basis[s][h]
                                self.basisDeriv[n][m][2] = basis[r][g]*basis[q][0]*basisDeriv[s][h][0]
                                m += 1
                            # Right
                            for f in range(0, numBasisFns-1):
                                self.basis[n][m] = basis[r][numBasisFns-1]*basis[q][f]*basis[s][h]
                                self.basisDeriv[n][m][0] = basisDeriv[r][numBasisFns-1][0]*basis[q][f]*basis[s][h]
                                self.basisDeriv[n][m][1] = basis[r][numBasisFns-1]*basisDeriv[q][f][0]*basis[s][h]
                                self.basisDeriv[n][m][2] = basis[r][numBasisFns-1]*basis[q][f]*basisDeriv[s][h][0]
                                m += 1
                            # Top
                            for g in range(numBasisFns-1, 0, -1):
                                self.basis[n][m] = basis[r][g]*basis[q][numBasisFns-1]*basis[s][h]
                                self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[q][numBasisFns-1]*basis[s][h]
                                self.basisDeriv[n][m][1] = basis[r][g]*basisDeriv[q][numBasisFns-1][0]*basis[s][h]
                                self.basisDeriv[n][m][2] = basis[r][g]*basis[q][numBasisFns-1]*basisDeriv[s][h][0]
                                m += 1
                            # Left
                            for f in range(numBasisFns-1, 0, -1):
                                self.basis[n][m] = basis[r][0]*basis[q][f]*basis[s][h]
                                self.basisDeriv[n][m][0] = basisDeriv[r][0][0]*basis[q][f]*basis[s][h]
                                self.basisDeriv[n][m][1] = basis[r][0]*basisDeriv[q][f][0]*basis[s][h]
                                self.basisDeriv[n][m][2] = basis[r][0]*basis[q][f]*basisDeriv[s][h][0]
                                m += 1
                            # Interior
                            for f in range(1, numBasisFns-1):
                                for g in range(1, numBasisFns-1):
                                    self.basis[n][m] = basis[r][g]*basis[q][f]*basis[s][h]
                                    self.basisDeriv[n][m][0] = basisDeriv[r][g][0]*basis[q][f]*basis[s][h]
                                    self.basisDeriv[m][m][1] = basis[r][g]*basisDeriv[q][f][0]*basis[s][h]
                                    self.basisDeriv[n][m][2] = basis[r][g]*basis[q][f]*basisDeriv[s][h][0]
                                    m += 1
                        if not m == self.numCorners:
                            raise RuntimeError('Invalid 3D function tabulation')
                        n += 1

            if not n == self.numQuadPts:
                raise RuntimeError('Invalid 3D quadrature')

            self.vertices = numpy.reshape(self.vertices, (self.numCorners, dim))
            self.quadPts = numpy.reshape(self.quadPts, (self.numQuadPts, dim))
            self.quadWts = numpy.reshape(self.quadWts, (self.numQuadPts))
            self.basis = numpy.reshape(self.basis, (self.numQuadPts, self.numCorners))
            self.basisDeriv = numpy.reshape(self.basisDeriv, (self.numQuadPts, self.numCorners, dim))
        
        return
