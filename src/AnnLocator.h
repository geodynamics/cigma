#ifndef __ANN_LOCATOR_H__
#define __ANN_LOCATOR_H__

#include "ANN/ANN.h"
#include "Locator.h"
#include "MeshPart.h"


namespace cigma
{
    class AnnLocator;
}


class cigma::AnnLocator : public cigma::Locator
{
public:
    AnnLocator();
    ~AnnLocator();

public:
    void initialize(MeshPart *meshPart);
    void search(double *globalPoint);

public:
    int n_idx();
    int idx(int i);

public:
    int nnk;            // number of nearest neighbors
    int npts;           // number of points (bounding boxes)
    int dim;            // dimension of bounding box (2 * nsd)
    double epsilon;     // tolerance (in bbox space)

public:
    ANNpointArray dataPoints;
    ANNkd_tree *kdtree;

public:
    ANNpoint queryPoint;    // query point for search()
    ANNidxArray nnIdx;      // near neighbor indices
    ANNdistArray nnDists;   // near neighbord distances

};


// ---------------------------------------------------------------------------

inline int cigma::AnnLocator::n_idx()
{
    return nnk;
}

inline int cigma::AnnLocator::idx(int i)
{
    //assert(0 <= i);
    //assert(i < nnk);
    return nnIdx[i];
}


// ---------------------------------------------------------------------------

#endif
