#include <iostream>
#include <cstdlib>
#include <cassert>
#include "../TextWriter.h"
#include "../VtkUgReader.h"

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



    int *connect;
    int nel, ndofs;
    reader->get_connectivity(&connect, &nel, &ndofs);
    assert(ndofs == 4);
    std::cout << "connect :: [ " << nel << " x " << ndofs << " ]\n";
    //writer->write_connectivity(connect, 10, ndofs);


    double *displacement;
    int disp_nno, disp_dim;
    reader->get_vector_point_data("displacements_t0", &displacement, &disp_nno, &disp_dim);
    assert(disp_nno = nno);
    assert(disp_dim == 3);
    std::cout << "displacement :: [ " << disp_nno << " x " << disp_dim << " ]\n";
    //writer->write_dofs(displacement, 10, 3);


    double *scalars;
    int scalars_nno, scalars_dim;
    reader->get_scalar_point_data(0, &scalars, &scalars_nno, &scalars_dim);
    assert(scalars_nno == nno);
    assert(scalars_dim == 4);
    std::cout << "scalars :: [ " << scalars_nno << " x " << scalars_dim << " ]\n";
    //writer->write_dofs(scalars, 10, scalars_dim);


    delete reader;

    writer->fp = NULL;
    delete writer;


    return 0;
}
