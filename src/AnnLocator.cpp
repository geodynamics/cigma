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
}

cigma::AnnLocator::~AnnLocator()
{
    if (kdtree != 0) delete kdtree;
    if (dataPoints != 0) annDeallocPts(dataPoints);
    if (nnIdx != 0) delete [] nnIdx;
    if (nnDists != 0) delete [] nnDists;
}


// ---------------------------------------------------------------------------

void cigma::AnnLocator::initialize(MeshPart *meshPart)
{
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
        meshPart->set_cell(i);
        meshPart->cell->bbox(minpt, maxpt);

        ANNpoint pt = dataPoints[i];
        for (j = 0; j < nsd; j++)
        {
            pt[nsd*0 + j] = minpt[j];
            pt[nsd*1 + j] = maxpt[j];
        }
    }

    kdtree = new ANNkd_tree(dataPoints, npts, dim);
}


// ---------------------------------------------------------------------------

void cigma::AnnLocator::search(double *globalPoint, int *cellIndices, int k)
{
    int i;

    for (i = 0; i < nsd; i++)
    {
        queryPoint[nsd*0 + i] = globalPoint[i];
        queryPoint[nsd*1 + i] = globalPoint[i];
    }

    kdtree->annkSearch(queryPoint, k, nnIdx, nnDists, epsilon);

    for (i = 0; i < k; i++)
    {
        cellIndices[i] = nnIdx[i];
    }
}


// ---------------------------------------------------------------------------
