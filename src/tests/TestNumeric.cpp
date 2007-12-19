#include <iostream>
#include <cstdlib>
#include <cassert>
#include "../TextWriter.h"
#include "../VtkUgReader.h"
#include "../Numeric.h"
using namespace cigma;

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
    assert(nsd == 3);
    std::cout << "coords :: [ " << nno << " x " << nsd << " ]\n";
    //writer->write_coordinates(coords, 10, nsd);


    double min[3],max[3];
    minmax(coords, nno, nsd, min, max);
    std::cout << "bbox = ("
              << "(" << min[0] << ", " << min[1] << ", " << min[2] << "),"
              << "(" << max[0] << ", " << max[1] << ", " << max[2] << "))"
              << std::endl;
    
    double centroid[3];
    cigma::centroid(coords, nno, nsd, centroid);
    std::cout << "centroid = "
              << "(" << centroid[0] << ", " << centroid[1] << ", " << centroid[2] << ")"
              << std::endl;


    delete reader;

    writer->fp = NULL;
    delete writer;


    return 0;
}
