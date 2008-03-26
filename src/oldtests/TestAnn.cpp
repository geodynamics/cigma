#include <iostream>
#include <fstream>

#include <cassert>
#include <cstdlib>
#include <ctime>

#include "ANN/ANN.h"

#include "MeshPart.h"
#include "MeshPart.h"
#include "VtkReader.h"

#include "Cell.h"
#include "Tet.h"
#include "Hex.h"
#include "Numeric.h"
#include "Misc.h"

#define NUM_QUERY_POINTS (10*1000)

using namespace std;
using namespace cigma;



class SpatialIndex
{
public:
    SpatialIndex();
    ~SpatialIndex();

public:
    void set_data(double *data, int npts, int dim);

public:
    bool find(double globalPoint[3], int *cellIndex);

public:
    int k;                  // number of nearest neighbors
    int npts;               // number of data points
    int dim;                // dimension of data point

    double epsilon;

    ANNpointArray dataPoints;
    ANNkd_tree *kdtree;

    ANNpoint queryPoint;    // query point
    ANNidxArray nnIdx;      // near neighbor indices
    ANNdistArray nnDists;   // near neighbor distances
};



SpatialIndex::SpatialIndex()
{
    npts = 0;
    dim = 0;

    dataPoints = 0;
    kdtree = 0;

    queryPoint = 0;

    k = 8;
    nnIdx = 0;
    nnDists = 0;

    epsilon = 0;
}


SpatialIndex::~SpatialIndex()
{
    if (kdtree != 0) delete kdtree;
    if (dataPoints != 0) annDeallocPts(dataPoints);
    if (nnIdx != 0) delete [] nnIdx;
    if (nnDists != 0) delete [] nnDists;
}

void SpatialIndex::set_data(double *data, int npts, int dim)
{
    assert(npts > 0);
    assert(dim > 0);

    this->npts = npts;
    this->dim = dim;

    dataPoints = annAllocPts(npts, dim);
    queryPoint = annAllocPt(dim);

    nnIdx = new ANNidx[k];
    nnDists = new ANNdist[k];

    int i,j;
    for (i = 0; i < npts; i++)
    {
        ANNpoint pt = dataPoints[i];
        for (j = 0; j < dim; j++)
        {
            pt[j] = data[dim*i + j];
        }
    }

    kdtree = new ANNkd_tree(dataPoints, npts, dim);
}

bool SpatialIndex::find(double globalPoint[3], int *cellIndex)
{
    queryPoint[0] = globalPoint[0];
    queryPoint[1] = globalPoint[1];
    queryPoint[2] = globalPoint[2];
    
    queryPoint[3] = globalPoint[0];
    queryPoint[4] = globalPoint[1];
    queryPoint[5] = globalPoint[2];

    const bool verbose = false;
    
    if (verbose)
    {
        cout << "Searching for (" << globalPoint[0] << " " << globalPoint[1] << " " << globalPoint[2] << ") ";
    }

    // search
    kdtree->annkSearch(queryPoint, k, nnIdx, nnDists, epsilon);

    // assign index/indices
    //*cellIndex = nnIdx[0];
    for (int i = 0; i < k; i++)
    {
        cellIndex[i] = nnIdx[i];
    }


    if (verbose)
    {
        cout << "-> ";
        cout << "index " << nnIdx[0] << " ";
        cout << "with dist " << nnDists[0] << endl;
    }

    return true;
}

int main(int argc, char *argv[])
{
    
    cout << "Testing ANN library..." << endl;

    int e,i,j;
    int cellIndex;
    int output_frequency = 100;

    //
    // Timing info
    //
    time_t t0, t1;
    double elapsed_mins;
    double remaining_mins;
    double total_mins;
    double progress;
    double rate;
    double points_per_sec;

    
    // 
    // Read mesh from file
    //
    string filename = "strikeslip_tet4_1000m_t0.vtk";
    if (argc > 1)
    {
        filename = argv[1];
    }
    int nno, nsd;
    double *coords;
    int nel, ndofs;
    int *connect;

    VtkReader reader;
    reader.open(filename);
    reader.get_coordinates(&coords, &nno, &nsd);
    reader.get_connectivity(&connect, &nel, &ndofs);
    assert(nsd == 3);

    MeshPart *meshPart;
    meshPart = new MeshPart();
    meshPart->set_coordinates(coords, nno, nsd);
    meshPart->set_connectivity(connect, nel, ndofs);
    meshPart->cell = 0;
    switch (ndofs)
    {
        case 4:
            meshPart->cell = new Tet();
            break;
        case 8:
            meshPart->cell = new Hex();
            break;
    }
    assert(meshPart->cell != 0);

    Cell *cell = meshPart->cell;

    // 
    // Get global bounding box
    //
    double global_minpt[3];
    double global_maxpt[3];
    cigma::minmax(coords, nno, nsd, global_minpt, global_maxpt);

    // 
    // Generate random query points
    //
    const int num_query_pts = NUM_QUERY_POINTS;
    double *query_points = new double[num_query_pts * 3];
    int *query_cells1 = new int[num_query_pts];

    cout << "Generating " << num_query_pts << " query points" << endl << endl;

    for (i = 0; i < num_query_pts; i++)
    {
        query_cells1[i] = 0;
        bbox_random_point(global_minpt, global_maxpt, &query_points[3*i]);
    }

    //
    // Brute force search -- obtain the right answers here, for
    // double-checking the spatial indexing method
    //

    //*
    cout << "Brute force method: " << endl;
    cout << "point cell points/sec elapsed eta total progress" << endl;

    time(&t0);
    t1 = t0;
    for (i = 0; i < num_query_pts; i++)
    {

        bool found = false;
        double *point = &query_points[3*i];

        if (i % output_frequency == 0)
        {
            cout << "(" << point[0] << " "
                        << point[1] << " "
                        << point[2] << ") ";
        }

        cellIndex = -1;
        for (e = 0; e < nel; e++)
        {
            double uvw[3];

            // update cell data
            meshPart->get_cell_coords(e, cell->globverts);

            cell->xyz2uvw(point, uvw);

            found = cell->interior(uvw[0], uvw[1], uvw[2]);

            if (found)
            {
                cellIndex = e;
                break;
            }
        }
        assert(found);

        query_cells1[i] = cellIndex;

        if (i % output_frequency == 0)
        {
            time(&t1);

            elapsed_mins = (t1 - t0) / 60.0;
            rate = elapsed_mins / (i + 1);
            points_per_sec = (1.0/60.0) / rate;
            remaining_mins = (num_query_pts - i) * rate;
            total_mins = num_query_pts * rate;
            progress = 100 * elapsed_mins / total_mins;

            cout << cellIndex << " "
                 << points_per_sec << " "
                 << elapsed_mins << " "
                 << remaining_mins << " "
                 << total_mins << " "
                 << progress << "% ";

            cout << "                                 "
                 << "                                 "
                 << "                                 "
                 << "\r";

            cout << std::flush;
        }
    }
    cout << endl;
    cout << "Total " << total_mins << endl;
    cout << endl;
    // */


    //
    // Calculate bounding boxes over each element
    //
    int numcellboxes = nel;
    int cellboxdim = nsd * 2;
    double *cellboxes = new double[numcellboxes * cellboxdim];
    for (e = 0; e < nel; e++)
    {
        double minpt[3], maxpt[3];
        double *bbox = &cellboxes[cellboxdim * e];

        // get cell data
        meshPart->get_cell_coords(e, cell->globverts);

        cigma::minmax(cell->globverts, cell->n_nodes(), cell->n_dim(), minpt, maxpt);
        
        bbox[0] = minpt[0];
        bbox[1] = minpt[1];
        bbox[2] = minpt[2];

        bbox[3] = maxpt[0];
        bbox[4] = maxpt[1];
        bbox[5] = maxpt[2];
    }


    // 
    // Search mesh using a spatial index
    //

    SpatialIndex *locator = new SpatialIndex();

    locator->set_data(cellboxes, numcellboxes, cellboxdim);
    delete [] cellboxes;

    int num_candidates = locator->k;
    int *query_cells2 = new int[num_query_pts * num_candidates];
    int *cellIndices = new int[num_candidates];

    cout << "Using kdtree spatial index...(searching for " << num_candidates << " neighbors)" << endl;
    cout << "point cell points/sec elapsed eta total progress" << endl;

    time(&t0);
    t1 = t0;
    for (i = 0; i < num_query_pts; i++)
    {
        double *point = &query_points[3*i];

        if (i % output_frequency == 0)
        {
            cout << "(" << point[0] << " "
                        << point[1] << " "
                        << point[2] << ") ";
        }

        locator->find(point, cellIndices);

        for (j = 0; j < num_candidates; j++)
        {
            query_cells2[num_candidates * i + j] = cellIndices[j];
        }


        if (i % output_frequency == 0)
        {
            time(&t1);

            elapsed_mins = (t1 - t0) / 60.0;
            rate = elapsed_mins / (i + 1);
            points_per_sec = (1.0/60.0) / rate;
            remaining_mins = (num_query_pts - i) * rate;
            total_mins = num_query_pts * rate;
            progress = 100 * elapsed_mins / total_mins;

            cout << cellIndices[0] << " "
                 << points_per_sec << " "
                 << elapsed_mins << " "
                 << remaining_mins << " "
                 << total_mins << " "
                 << progress << "% ";

            cout << "                                 "
                 << "                                 "
                 << "                                 "
                 << "\r";

            cout << std::flush;
        }
    }

    delete [] cellIndices;

    cout << endl;
    cout << "Total " << total_mins << endl;
    cout << endl;

    
    // 
    // Write out indices
    //
    ofstream indexfile;
    indexfile.open("foo.index");
    for (i = 0; i < num_query_pts; i++)
    {
        double *point = &query_points[3*i];

        indexfile << setw(8) << point[0] << " "
                  << setw(8) << point[1] << " "
                  << setw(8) << point[2] << " "
                  << setw(8) << query_cells1[i] << " ";

        for (j = 0; j < num_candidates; j++)
        {
            indexfile << setw(8) << query_cells2[num_candidates*i + j] << " ";
        }

        bool found = false;
        for (j = 0; j < num_candidates; j++)
        {
            int a = query_cells1[i];
            int b = query_cells2[num_candidates * i + j];
            if (a == b)
            {
                found = true;
            }
        }
        if (found)
        {
            indexfile << "yes";
        }
        else
        {
            indexfile << "no!";
        }
        indexfile << endl;
    }
    indexfile.close();


    //
    // Clean up
    //
    delete locator;
    delete [] query_points;
    delete [] query_cells1;
    delete [] query_cells2;


    return 0;
}
