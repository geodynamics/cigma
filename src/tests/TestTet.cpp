#include <iostream>
#include <cstdlib>
#include <cassert>

#include "../Tet.h"

using namespace std;
using namespace cigma;

int main(void)
{
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

        cout << i << " : ";

        cout << "x = ["
             << u << ","
             << v << ","
             << w << "]  ";

        bool inside = tetcell.interior(u,v,w);
        cout << "in = " << inside << "  ";

        double *phi = &tab[ndofs*i];
        cout << "phi = ["
             << phi[0] << ","
             << phi[1] << ","
             << phi[2] << ","
             << phi[3] << "]  ";

        double *grad_phi = &tab_jet[i*ndofs*3];
        cout << "grad_phi = ["
             << "[" << grad_phi[3*0 + 0] << ","
                    << grad_phi[3*0 + 1] << ","
                    << grad_phi[3*0 + 2] << "],"

             << "[" << grad_phi[3*1 + 0] << ","
                    << grad_phi[3*1 + 1] << ","
                    << grad_phi[3*1 + 2] << "],"

             << "[" << grad_phi[3*2 + 0] << ","
                    << grad_phi[3*2 + 1] << ","
                    << grad_phi[3*2 + 2] << "],"

             << "[" << grad_phi[3*3 + 0] << ","
                    << grad_phi[3*3 + 1] << ","
                    << grad_phi[3*3 + 2] << "]"
             << "]  ";


        double J[3][3];
        double detJ = tetcell.jacobian(u,v,w,J);
        cout << "detJ = " << detJ;

        double xyz[3] = {u,v,w};
        double uvw[3];
        tetcell.xyz2uvw(xyz,uvw);
        cout << "  xyz2uvw -> ["
             << uvw[0] << ","
             << uvw[1] << ","
             << uvw[2] << "]";

        cout << endl;
    }


    

    // -----------------------------------------------------------------------
    
    //delete reader;

    return 0;
}
