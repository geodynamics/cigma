#include <cassert>
#include "AnnLocator.h"


// ---------------------------------------------------------------------------

cigma::AnnLocator::AnnLocator()
{
    nnk = 8;
    epsilon = 0;

    npts = 0;
    dim = 0;

    dataPoints = 0;
    kdtree = 0;

    nnIdx = 0;
    nnDists = 0;

    locatorType = NULL_LOCATOR;
}

cigma::AnnLocator::~AnnLocator()
{
    if (kdtree != 0) delete kdtree;

    if (dataPoints != 0)
    {
        if (locatorType == CELL_LOCATOR)
        {
            annDeallocPts(dataPoints);
        }
    }

    if (nnIdx != 0) delete [] nnIdx;
    if (nnDists != 0) delete [] nnDists;
}


// ---------------------------------------------------------------------------

void cigma::AnnLocator::initialize(MeshPart *meshPart)
{
    assert(nnk > 0);

    npts = meshPart->nel;
    nsd = meshPart->nsd;
    dim = 2 * nsd;
    
    assert(npts > 0);
    assert(nsd > 0);

    dataPoints = annAllocPts(npts, dim);
    queryPoint = annAllocPt(dim);

    nnIdx = new ANNidx[nnk];
    nnDists = new ANNdist[nnk];

    int i,j;
    double minpt[nsd];
    double maxpt[nsd];

    for (i = 0; i < npts; i++)
    {
        ANNpoint pt = dataPoints[i];
        meshPart->select_cell(i);
        meshPart->cell->bbox(minpt, maxpt);
        for (j = 0; j < nsd; j++)
        {
            pt[nsd*0 + j] = minpt[j];
            pt[nsd*1 + j] = maxpt[j];
        }
    }

    kdtree = new ANNkd_tree(dataPoints, npts, dim);

    locatorType = CELL_LOCATOR;
}


// ---------------------------------------------------------------------------

void cigma::AnnLocator::initialize(Points *points)
{
    assert(nnk > 0);

    npts = points->n_points();
    dim  = points->n_dim();

    assert(npts > 0);
    assert(nsd > 0);

    // XXX watch out for when you change the ANNpoint type to float
    assert(sizeof(ANNpoint) == sizeof(double));

    dataPoints = (ANNpointArray)(points->data);
    queryPoint = annAllocPt(dim);

    nnIdx = new ANNidx[nnk];
    nnDists = new ANNdist[nnk];

    kdtree = new ANNkd_tree(dataPoints, npts, dim);

    locatorType = POINT_LOCATOR;
}


// ---------------------------------------------------------------------------

void cigma::AnnLocator::search(double *globalPoint)
{
    for (int i = 0; i < nsd; i++)
    {
        queryPoint[nsd*0 + i] = globalPoint[i];
        queryPoint[nsd*1 + i] = globalPoint[i];
    }

    kdtree->annkSearch(queryPoint, nnk, nnIdx, nnDists, epsilon);
}


// ---------------------------------------------------------------------------
