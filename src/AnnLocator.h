#ifndef __ANN_LOCATOR_H__
#define __ANN_LOCATOR_H__

#include "ANN/ANN.h"
#include "MeshPart.h"


namespace cigma
{
    class AnnLocator;
}



class cigma::AnnLocator
{
public:
    AnnLocator();
    ~AnnLocator();

public:
    void initialize(MeshPart *meshPart);

public:
    void search(double *globalPoint, int *cellIndices, int k);

public:
    int nnk;            // number of nearest neighbors
    int npts;           // number of points (bounding boxes)
    int nsd;            // spatial dimensions
    int dim;            // dimension of bounding box (2 * nsd)
    double epsilon;     // tolerance (in bbox space)

public:
    ANNpointArray dataPoints;
    ANNkd_tree *kdtree;

public:
    ANNpoint queryPoint;    // query point for find()
    ANNidxArray nnIdx;      // near neighbor indices
    ANNdistArray nnDists;   // near neighbord distances

};



#endif
