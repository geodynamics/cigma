#include <iostream>
#include <cstdlib>
#include <cassert>

//#include "../TextWriter.h"
//#include "../VtkUgReader.h"
#include "../Tet.h"

using namespace cigma;

int main(void)
{
    //TextWriter writer;
    //writer.fp = stdout;

    //VtkUgReader *reader = new VtkUgReader();
    //reader->open("strikeslip_tet4_0500m_t0.vtk");
    
    // -----------------------------------------------------------------------

    Tet tetcell;

    int ndofs = tetcell.n_nodes();
    const int npts = 5;
    const int nsd = 3;

    double refpts[npts*nsd] = {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        0.5, 0.5, 0.5
    };
    
    double tab[npts*ndofs];
    double tab_jet[npts*ndofs*nsd];
    tetcell.shape(npts, refpts, tab);
    tetcell.grad_shape(npts, refpts, tab_jet);

    for (int i = 0; i < npts; i++)
    {
        double *pt = &refpts[nsd*i];
        double u = pt[0];
        double v = pt[1];
        double w = pt[2];

        std::cout << i << " : ";

        std::cout << "x = ("
                  << u << " "
                  << v << " "
                  << w << ")  ";

        bool inside = tetcell.interior(u,v,w);
        std::cout << "in = " << inside << "  ";

        double *phi = &tab[ndofs*i];
        std::cout << "phi = ("
                  << phi[0] << " "
                  << phi[1] << " "
                  << phi[2] << " "
                  << phi[3] << ")  ";

        double *grad_phi = &tab_jet[i*ndofs*3];
        std::cout << "grad_phi = ("
                  << "(" << grad_phi[3*0 + 0] << " "
                         << grad_phi[3*0 + 1] << " "
                         << grad_phi[3*0 + 2] << "),"

                  << "(" << grad_phi[3*1 + 0] << " "
                         << grad_phi[3*1 + 1] << " "
                         << grad_phi[3*1 + 2] << "),"

                  << "(" << grad_phi[3*2 + 0] << " "
                         << grad_phi[3*2 + 1] << " "
                         << grad_phi[3*2 + 2] << "),"

                  << "(" << grad_phi[3*3 + 0] << " "
                         << grad_phi[3*3 + 1] << " "
                         << grad_phi[3*3 + 2] << ")"
                  << ")  ";


        double J[3][3];
        double detJ = tetcell.jacobian(u,v,w,J);
        std::cout << "detJ = " << detJ;

        std::cout << std::endl;
    }


    

    // -----------------------------------------------------------------------
    
    //delete reader;

    return 0;
}
