#include <iostream>
#include <iomanip>
#include "../Tet.h"
#include "../QuadraturePoints.h"

int main()
{
    using namespace std;
    using namespace cigma;

    int i,j;

    const int npts = 8;

    double qwts[npts] = {
        0.29583885,  0.12821632,  0.16925605,
        0.07335544,  0.29583885,  0.12821632,
        0.16925605,  0.07335544
    };

    double qpts[npts*3] = {
        -0.68663473, -0.72789005, -0.75497035,
        -0.83720867, -0.85864055,  0.08830369,
        -0.86832263,  0.13186633, -0.75497035,
        -0.93159441, -0.4120024 ,  0.08830369,
         0.16949513, -0.72789005, -0.75497035,
        -0.39245447, -0.85864055,  0.08830369,
        -0.50857335,  0.13186633, -0.75497035,
        -0.74470688, -0.4120024 ,  0.08830369
    };

    QuadraturePoints Q;
    Q.set_quadrature(qpts, qwts, npts, 3);

    /*
     * change default numeric format
     */
    cout << std::showpos
         << std::fixed
         << std::setprecision(8);


    /*
     * print reference points on tet
     */
    cout << "Reference points" << endl;
    for (i = 0; i < Q.n_points(); i++)
    {
        for (j = 0; j < Q.n_refdim(); j++)
        {
            cout << " " << Q.point(i,j);
        }
        cout << endl;
    }
    cout << endl;

    
    /* apply reference map to quadrature points
     * and print out the resulting global coordinates
     */ 
    
    cout << "Global coordinates" << endl;
    
    double verts[2][4*3] = {
        {0.0, 0.0, 0.0,
         1.0, 0.0, 0.0,
         0.0, 1.0, 0.0,
         0.0, 0.0, 1.0},
        {-1, -1, -1,
         +1, -1, -1,
         -1, +1, -1,
         -1, -1, +1}
    };

    Tet tet;
    tet.set_global_vertices(verts[1], 4, 3);
    Q.set_globaldim(3);
    Q.apply_refmap(&tet);

    for (i = 0; i < Q.n_points(); i++)
    {
        for (j = 0; j < Q.n_globaldim(); j++)
        {
            cout << " " << Q(i,j);
        }
        cout << endl;
    }

    return 0;
}
