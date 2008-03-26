#include <iostream>
#include <cstdlib>
#include <cassert>
#include "../TextWriter.h"
#include "../VtkReader.h"
#include "../Numeric.h"
using namespace cigma;

int main(void)
{
    TextWriter *writer = new TextWriter();
    writer->fp = stdout;


    VtkReader *reader = new VtkReader();
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

    double mat[3][3] = {
        { 0.49387228,  0.38626503,  0.85391322},
        { 0.76706402,  0.07299347,  0.79921455},
        { 0.81741773,  0.67283022,  0.52285359}
    };
    double invmat[3][3] = {
        {-2.077549  ,  1.54943045,  1.02460651},
        { 1.04894064, -1.82890268,  1.08248536},
        { 1.89817274, -0.06883558, -1.08225401}
    };
    double inv[3][3];
    inv3x3(mat, inv);
    int i,j;
    std::cout << "trueinv - inv = " << std::endl;
    for (i = 0; i < 3; i++)
    {
        std::cout << "    ";
        for (j = 0; j < 3; j++)
        {
            double daij = invmat[i][j] - inv[i][j];
            std::cerr << daij << " ";
        }
        std::cout << std::endl;
    }

    delete reader;

    writer->fp = NULL;
    delete writer;


    return 0;
}
