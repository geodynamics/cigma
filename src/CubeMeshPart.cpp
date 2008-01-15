#include <cassert>
#include "CubeMeshPart.h"
#include "Numeric.h"

// ---------------------------------------------------------------------------

cigma::CubeMeshPart::
CubeMeshPart()
{
    L = M = N = 0;

    hexnel = 0;
    hexconn = 0;

    tetnel = 0;
    tetconn = 0;
}


cigma::CubeMeshPart::
~CubeMeshPart()
{
    if (coords != 0)  { delete [] coords; }
    if (hexconn != 0) { delete [] hexconn; }
    if (tetconn != 0) { delete [] tetconn; }
}


// ---------------------------------------------------------------------------

bool cigma::CubeMeshPart::calc_coordinates(int L, int M, int N)
{
    // save the cube dimensions (number of elements per side)
    this->L = L;
    this->M = M;
    this->N = N;

    int n;
    int nx, ny, nz;
    int nxstride, nystride, nzstride;

    double x, y, z;
    double dx, dy, dz;

    nno = (L+1)*(M+1)*(N+1);
    nsd = 3;
    coords = new double[nno * nsd];

    dx = 1.0 / L;
    dy = 1.0 / M;
    dz = 1.0 / N;

    nxstride = (M+1)*(N+1);
    nystride = (N+1);
    nzstride = 1;

    for (nx = 0; nx <= L; nx++)
    {
        x = nx * dx;
        for (ny = 0; ny <= M; ny++)
        {
            y = ny * dy;
            for (nz = 0; nz <= N; nz++)
            {
                z = nz * dz;
                n = nx * nxstride + ny * nystride + nz * nzstride;
                coords[nsd * n + 0] = x;
                coords[nsd * n + 1] = y;
                coords[nsd * n + 2] = z;
            }
        }
    }
    return true;
}

bool cigma::CubeMeshPart::calc_hex8_connectivity()
{
    assert(coords != 0);

    int e;
    int ex, ey, ez;
    int exstride, eystride, ezstride;

    int n;
    int nx, ny, nz;
    int nxstride, nystride, nzstride;

    int hex[8];

    hexnel = L*M*N;
    hexconn = new int[hexnel * 8];

    // strides for the connectivity array
    exstride = M*N;
    eystride = N;
    ezstride = 1;

    // strides for the coords array
    nxstride = (M+1)*(N+1);
    nystride = (N+1);
    nzstride = 1;

    for (ex = 0; ex < L; ex++)
    {
        for (ey = 0; ey < M; ey++)
        {
            for (ez = 0; ez < N; ez++)
            {
                //
                // Get (nx,ny,nz) for vertex v0
                //
                nx = ex;
                ny = ey;
                nz = ez;

                //
                // Local view of hex element
                //
                //       v7-------v6
                //      /|        /|
                //     / |       / |
                //    v3-------v2  |
                //    |  |     |   |
                //    |  v4----|--v5
                //    | /      |  /
                //    |/       | /
                //    v0-------v1
                //
                #define HEXGLOB(x,y,z) ((x)*nxstride + (y)*nystride + (z)*(nzstride))
                hex[0] = HEXGLOB(nx,   ny,   nz);
                hex[1] = HEXGLOB(nx+1, ny,   nz);
                hex[2] = HEXGLOB(nx+1, ny+1, nz);
                hex[3] = HEXGLOB(nx,   ny+1, nz);
                hex[4] = HEXGLOB(nx,   ny,   nz+1);
                hex[5] = HEXGLOB(nx+1, ny,   nz+1);
                hex[6] = HEXGLOB(nx+1, ny+1, nz+1);
                hex[7] = HEXGLOB(nx,   ny+1, nz+1);
                #undef HEXGLOB

                //
                // Finally, save the connectivity for the new hex elt
                //
                e = ex*exstride + ey*eystride + ez*ezstride;
                for (n = 0; n < 8; n++)
                {
                    hexconn[8*e + n] = hex[n];
                }
            }
        }
    }

    nel = hexnel;
    ndofs = 8;
    connect = hexconn;
    return true;
}

bool cigma::CubeMeshPart::calc_tet4_connectivity()
{
    assert(coords != 0);

    if (hexconn == 0)
    {
        calc_hex8_connectivity();
        assert(hexconn != 0);
    }

    int e, h, n;
    int ex, ey, ez, et;
    int exstride, eystride, ezstride, etstride;

    int hex[8];
    int tet[5][4];
    int localmap[5][4] = {{0,1,3,4},
                          {1,2,3,6},
                          {1,3,4,6},
                          {1,4,5,6},
                          {3,4,6,7}};

    // each old hex element turns into 5 tets
    tetnel = L*M*N*5;
    tetconn = new int[tetnel * 4];

    // strides for element
    exstride = M*N*5;
    eystride = N*5;
    ezstride = 5;
    etstride = 1;

    for (ex = 0; ex < L; ex++)
    {
        for (ey = 0; ey < M; ey++)
        {
            for (ez = 0; ez < N; ez++)
            {
                //
                // First, load <v0,...,v7> into hex array
                //
                h = (ex*exstride + ey*eystride + ez*ezstride)/5;
                for (n = 0; n < 8; n++)
                {
                    hex[n] = hexconn[8*h + n];
                }

                //
                // Tetrahedral partition of hex element
                //
                //       v7-------v6
                //      /|        /|
                //     / |       / |
                //    v3-------v2  |
                //    |  |     |   |
                //    |  v4----|--v5
                //    | /      |  /
                //    |/       | /
                //    v0-------v1
                //
                //  T0 = <v0, v1, v3, v4>
                //  T1 = <v1, v2, v3, v6>
                //  T2 = <v1, v3, v4, v6>
                //  T3 = <v1, v4, v5, v6>
                //  T4 = <v3, v4, v6, v7>
                //
                // To map from the local hex element to the local tet elements,
                // use the following
                //
                //  int localmap[5][4] = {{0,1,3,4},
                //                        {1,2,3,6},
                //                        {1,3,4,6},
                //                        {1,4,5,6},
                //                        {3,4,6,7}};
                //
                for (et = 0; et < 5; et++)
                {
                    for (n = 0; n < 4; n++)
                    {
                        tet[et][n] = hex[localmap[et][n]];
                    }
                }

                //
                // Finally, save the connectivity for the five new elts
                //
                for (et = 0; et < 5; et++)
                {
                    e = ex*exstride + ey*eystride + ez*ezstride + et*etstride;
                    for (n = 0; n < 4; n++)
                    {
                        tetconn[4*e + n] = tet[et][n];
                    }
                }
            }
        }
    }

    if (true)
    {
        delete [] hexconn;
        hexnel = 0;
        hexconn = 0;
    }

    nel = tetnel;
    ndofs = 4;
    connect = tetconn;
    return true;
}


// ---------------------------------------------------------------------------


static double tet_volume(double a[3], double b[3], double c[3], double d[3])
{
    //
    // (x0,y0,z0) = (a[0],a[1],a[2])
    // (x1,y1,z1) = (b[0],b[1],b[2])
    // (x2,y2,z2) = (c[0],c[1],c[2])
    // (x3,y3,z3) = (d[0],d[1],d[2])
    //
    //
    //           | x1-x0  y1-y0  z1-z0 |
    // V = (1/6) | x2-x0  y2-y0  z2-z0 |
    //           | x3-x0  y3-y0  z3-z0 |
    //
    //

    double mat[3][3] = {
        {b[0] - a[0], b[1] - a[1], b[2] - a[2]},
        {c[0] - a[0], c[1] - a[1], c[2] - a[2]},
        {d[0] - a[0], d[1] - a[1], d[2] - a[2]}
    };

    return cigma::det3x3(mat) / 6.0;
}


bool cigma::CubeMeshPart::find_cell(double globalPoint[3], int *cellIndex)
{
    double dx = 1.0 / L;
    double dy = 1.0 / M;
    double dz = 1.0 / N;

    int n;
    int nx, ny, nz;
    int nxstride, nystride, nzstride;

    int e;
    int ex, ey, ez, et;
    int exstride, eystride, ezstride, etstride;

    int hex[8];
    int tet[5][4];
    int localmap[5][4] = {{0,1,3,4},
                          {1,2,3,6},
                          {1,3,4,6},
                          {1,4,5,6},
                          {3,4,6,7}};

    // coordinates (nx,ny,nz) of vertex v0
    nx = globalPoint[0]/dx;
    ny = globalPoint[1]/dy;
    nz = globalPoint[2]/dz;
    assert(0 <= nx);
    assert(0 <= ny);
    assert(0 <= nz);
    assert(nx <  L);
    assert(ny <  M);
    assert(nz <  N);

    nxstride = (M+1)*(N+1);
    nystride = (N+1);
    nzstride = 1;

    ex = nx;
    ey = ny;
    ez = nz;

    *cellIndex = -1;
    bool found_cell = false;

    switch (ndofs)
    {
    case 8:
        // calculate strides
        exstride = M*N;
        eystride = N;
        ezstride = 1;

        // calculate cell id
        e = ex*exstride + ey*eystride + ez*ezstride;
        found_cell = true;
        break;

    case 4:
        // calculate strides
        exstride = M*N*5;
        eystride = N*5;
        ezstride = 5;
        etstride = 1;

        #define HEXGLOB(x,y,z) ((x)*nxstride + (y)*nystride + (z)*(nzstride))
        hex[0] = HEXGLOB(nx,   ny,   nz);
        hex[1] = HEXGLOB(nx+1, ny,   nz);
        hex[2] = HEXGLOB(nx+1, ny+1, nz);
        hex[3] = HEXGLOB(nx,   ny+1, nz);
        hex[4] = HEXGLOB(nx,   ny,   nz+1);
        hex[5] = HEXGLOB(nx+1, ny,   nz+1);
        hex[6] = HEXGLOB(nx+1, ny+1, nz+1);
        hex[7] = HEXGLOB(nx,   ny+1, nz+1);
        #undef HEXGLOB

        for (et = 0; et < 5; et++)
        {
            for (n = 0; n < 4; n++)
            {
                tet[et][n] = hex[localmap[et][n]];
            }
        }

        for (et = 0; et < 5; et++)
        {
            double *a, *b, *c, *d;
            a = &coords[3*tet[et][0]];
            b = &coords[3*tet[et][1]];
            c = &coords[3*tet[et][2]];
            d = &coords[3*tet[et][3]];

            double vol[4];
            
            vol[0] = tet_volume(a,b,c,globalPoint);
            if (vol[0] < 0) { continue; }
            
            vol[1] = tet_volume(a,d,b,globalPoint);
            if (vol[1] < 0) { continue; }
            
            vol[2] = tet_volume(b,d,c,globalPoint);
            if (vol[2] < 0) { continue; }

            vol[3] = tet_volume(a,c,d,globalPoint);
            if (vol[3] < 0) { continue; }

            e = ex*exstride + ey*eystride + ez*ezstride + et*etstride;
            found_cell = true;
            break;
        }

        break;
    default:
        assert(false);
        break;
    }

    if (found_cell)
    {
        assert(0 <= e);
        assert(e < nel);
        *cellIndex = e;
        return true;
    }

    return false;
}


// ---------------------------------------------------------------------------

