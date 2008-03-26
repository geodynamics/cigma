#include <iostream>
#include <cstdlib>
#include <cassert>

#include "../Tri.h"

using namespace cigma;
using namespace std;

int main(void)
{
    Tri tricell;

    const int npts = 5;
    const int celldim = 2;
    const int ndofs = tricell.n_nodes();

    double refpts[npts*celldim] = {
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0,
        0.5, 0.5,
        1.0, 1.0
    };

    double tab[npts*ndofs];
    double tab_jet[npts*ndofs*celldim];
    tricell.shape(npts, refpts, tab);
    tricell.grad_shape(npts, refpts, tab_jet);

    for (int i = 0; i < npts; i++)
    {
        double *pt = &refpts[celldim*i];
        double u = pt[0];
        double v = pt[1];
        double w = 0.0;

        cout << i << " : ";

        cout << "x = ["
             << u << ","
             << v << "]  ";

        bool inside = tricell.interior(u,v,w);
        cout << "in = " << inside << "  ";

        double *phi = &tab[ndofs*i];
        cout << "phi = ["
             << phi[0] << ","
             << phi[1] << ","
             << phi[2] << "]  ";

        double *grad_phi = &tab_jet[i*ndofs*celldim];
        cout << "grad_phi = ["
             << "[" << grad_phi[2*0 + 0] << ","
                    << grad_phi[2*0 + 1] << "],"

             << "[" << grad_phi[2*1 + 0] << ","
                    << grad_phi[2*1 + 1] << "],"

             << "[" << grad_phi[2*2 + 0] << ","
                    << grad_phi[2*2 + 1] << "]"
             << "]  ";

        double J[3][3];
        double detJ = tricell.jacobian(u,v,w,J);
        cout << "detJ = " << detJ;

        double xyz[3] = {u,v,w};
        double uvw[3];
        tricell.xyz2uvw(xyz,uvw);
        cout << "  xyz2uvw -> ["
             << uvw[0] << ","
             << uvw[1] << ","
             << uvw[2] << "]";

        cout << endl;
    }

    return 0;
}
