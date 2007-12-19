#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cassert>

#include "../Numeric.h"
#include "../TextWriter.h"
#include "../VtkUgReader.h"
#include "../VtkMeshPart.h"

using namespace cigma;

// ---------------------------------------------------------------------------

inline double pick_from_interval(double a, double b)
{
    return a + (b-a) * rand()/(RAND_MAX + 1.0);
}

void bbox_random_point(double minpt[3], double maxpt[3], double x[3])
{
    const int nsd = 3;
    for (int i = 0; i < nsd; i++)
    {
        assert(minpt[i] <= maxpt[i]);
        x[i] = pick_from_interval(minpt[i], maxpt[i]);
    }
}

// ---------------------------------------------------------------------------

int main(void)
{
    TextWriter *writer = new TextWriter();
    writer->fp = stdout;

    VtkUgReader *reader = new VtkUgReader();
    std::string filename = "strikeslip_tet4_1000m_t0.vtk";
    reader->open(filename);

    double *coords;
    int nno, nsd;
    reader->get_coordinates(&coords, &nno, &nsd);

    int *connect;
    int nel, ndofs;
    reader->get_connectivity(&connect, &nel, &ndofs);

    VtkMeshPart *meshPart = new VtkMeshPart();

    meshPart->set_coordinates(coords, nno, nsd);
    meshPart->set_connectivity(connect, nel, ndofs);


    double minpt[3], maxpt[3];
    minmax(coords, nno, nsd, minpt, maxpt);
    //std::cout << std::setprecision(8);
    std::cout << "minpt = " << minpt[0] << " " << minpt[1] << " " << minpt[2] << std::endl;
    std::cout << "maxpt = " << maxpt[0] << " " << maxpt[1] << " " << maxpt[2] << std::endl;
    
    int i;
    int npts = 100;
    double *points = new double[npts * nsd];
    for (i = 0; i < npts; i++)
    {
        double *pt = &points[nsd*i];
        bbox_random_point(minpt, maxpt, pt);
        std::cout << pt[0] << " " << pt[1] << " " << pt[2] << std::endl;
    }


    delete reader;

    writer->fp = NULL;
    delete writer;

    return 0;
}
