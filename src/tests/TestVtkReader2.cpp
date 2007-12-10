#include <iostream>
#include <cassert>
#include "../VtkUgReader.h"

using namespace cigma;

int main(void)
{
    std::string filename = "strikeslip_tet4_1000m_t0.vtk";
    VtkUgReader *reader = new VtkUgReader();

    int nel, ndofs;
    int *connect;

    int nno, nsd;
    double *coords;

    int disp_nno, disp_dim;
    double *displacement;

    int scalars_nno, scalars_dim;
    double *scalars;

    reader->open(filename);
    reader->get_connectivity(&connect, &nel, &ndofs);
    reader->get_coordinates(&coords, &nno, &nsd);

    reader->get_vector_point_data("displacements_t0", &displacement, &disp_nno, &disp_dim);
    assert(nno == disp_nno);
    assert(nsd == 3);

    reader->get_scalar_point_data(0, &scalars, &scalars_nno, &scalars_dim);
    assert(nno = scalars_nno);
    assert(scalars_dim == 4);


    std::cout << "coords :: [ " << nno << " x " << nsd << " ]\n";
    std::cout << "connect :: [ " << nel << " x " << ndofs << " ]\n";
    std::cout << "displacement :: [ " << disp_nno << " x " << disp_dim << " ]\n";
    std::cout << "scalars :: [ " << scalars_nno << " x " << scalars_dim << " ]\n";

    /*
    for (int i = 0; i < nno; i++)
    {
        for (int j = 0; j < nsd; j++)
        {
            std::cout << coords[nsd*i + j] << " ";
        }
        std::cout << std::endl;
    } // */

    /*
    for (int i = 0; i < nel; i++)
    {
        for (int j = 0; j < ndofs; j++)
        {
            std::cout << connect[ndofs*i + j] << " ";
        }
        std::cout << std::endl;
    } // */

    /*
    for (int i = 0; i < disp_nno; i++)
    {
        for (int j = 0; j < disp_dim; j++)
        {
            std::cout << displacement[disp_dim * i + j] << " ";
        }
        std::cout << std::endl;
    } // */

    /*
    for (int i = 0; i < scalars_nno; i++)
    {
        for (int j = 0; j < scalars_dim; j++)
        {
            std::cout << scalars[scalars_dim * i + j] << " ";
        }
        std::cout << std::endl;
    } // */


    delete reader;
    return 0;
}
