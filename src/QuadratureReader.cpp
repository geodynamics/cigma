#include <iostream>
#include <cassert>
#include <cstdlib>
#include "QuadratureReader.h"
#include "StringUtils.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

QuadratureReader::QuadratureReader()
{
    quadrature = 0;
}

QuadratureReader::~QuadratureReader()
{
    if (quadrature != 0)
    {
        delete quadrature;
        quadrature = 0;
    }
}

// ---------------------------------------------------------------------------

void QuadratureReader::load_args(AnyOption *opt, const char *opt_prefix)
{
    assert(opt != 0);

    string prefix = opt_prefix;
    string opt_name;
    char *in;

    opt_name = prefix + "-order";
    in = opt->getValue(opt_name.c_str());
    if (in != 0)
    {
        this->quadratureOrder = in;
    }

    opt_name = prefix;
    in = opt->getValue(opt_name.c_str());
    if (in != 0)
    {
        this->quadraturePath = in;
    }

    opt_name = prefix + "-points";
    in = opt->getValue(opt_name.c_str());
    if (in != 0)
    {
        this->pointsPath = in;
    }

    opt_name = prefix + "-weights";
    in = opt->getValue(opt_name.c_str());
    if (in != 0)
    {
        this->weightsPath = in;
    }
}

void QuadratureReader::validate_args(const char *cmd_name)
{
    //
    // Check for incompatible/inconsistent options
    //

    if ((pointsPath == "") && (weightsPath != ""))
    {
        cerr << cmd_name << ": "
             << "Quadrature weights specified, but missing the quadrature points"
             << endl;
        exit(1);
    }

    if ((weightsPath == "") && (pointsPath != ""))
    {
        cerr << cmd_name << ": "
             << "Quadrature points specified, but missing the quadrature weights"
             << endl;
        exit(1);
    }

    if ((weightsPath != "") && (pointsPath != ""))
    {
        if (quadraturePath != "")
        {
            cerr << cmd_name << ": "
                 << "Quadrature points and weights already specified "
                 << "(don't need " << quadraturePath << ")" << endl;
            exit(1);
        }
    }

    if ((quadratureOrder != "")
            && ((quadraturePath != "")
            || ((pointsPath != "") && (weightsPath != ""))))
    {
        cerr << cmd_name << ": "
             << "No need to specify quadrature order (already provided points and weights)"
             << endl;
        exit(1);
    }
}

// ---------------------------------------------------------------------------

void QuadratureReader::load_quadrature(Cell *cell)
{
    //cout << "Calling QuadratureReader::load_quadrature()" << endl;

    assert(cell != 0);


    int i,j;

    //
    // XXX: move these default rules into the appropriate Cell subclasses
    //

    // tri_qr(5)
    const int tri_nno = 9;
    const int tri_celldim = 2;
    double tri_qpts[tri_nno * tri_celldim] = {
        -0.79456469, -0.82282408,
        -0.86689186, -0.18106627,
        -0.95213774,  0.57531892,
        -0.08858796, -0.82282408,
        -0.40946686, -0.18106627,
        -0.78765946,  0.57531892,
         0.61738877, -0.82282408,
         0.04795814, -0.18106627,
        -0.62318119,  0.57531892
    };
    double tri_qwts[tri_nno] = {
        0.22325768,  0.25471234,  0.07758553,  0.35721229,  0.40753974,
        0.12413685,  0.22325768,  0.25471234,  0.07758553
    };
    for (i = 0; i < tri_nno; i++)
    {
        for (j = 0; j < tri_celldim; j++)
        {
            int n = tri_celldim * i + j;
            tri_qpts[n] += 1;
            tri_qpts[n] *= 0.5;
        }
    }

    // quad_qr(7)
    const int quad_nno = 16;
    const int quad_celldim = 2;
    double quad_qpts[quad_nno * quad_celldim] = {
        -0.86113631, -0.86113631,
        -0.33998104, -0.86113631,
         0.33998104, -0.86113631,
         0.86113631, -0.86113631,
         0.86113631, -0.33998104,
         0.86113631,  0.33998104,
         0.86113631,  0.86113631,
         0.33998104,  0.86113631,
        -0.33998104,  0.86113631,
        -0.86113631,  0.86113631,
        -0.86113631,  0.33998104,
        -0.86113631, -0.33998104,
        -0.33998104, -0.33998104,
         0.33998104, -0.33998104,
        -0.33998104,  0.33998104,
         0.33998104,  0.33998104
    };
    double quad_qwts[quad_nno] = {
        0.12100299,  0.22685185,  0.22685185,  0.12100299,  0.22685185,
        0.22685185,  0.12100299,  0.22685185,  0.22685185,  0.12100299,
        0.22685185,  0.22685185,  0.4252933 ,  0.4252933 ,  0.4252933 ,
        0.4252933
    };


    // tet_qr(3)
    const int tet_nno = 8;
    const int tet_celldim = 3;
    double tet_qpts[tet_nno * tet_celldim] = {
        -0.68663473, -0.72789005, -0.75497035,
        -0.83720867, -0.85864055,  0.08830369,
        -0.86832263,  0.13186633, -0.75497035,
        -0.93159441, -0.4120024 ,  0.08830369,
         0.16949513, -0.72789005, -0.75497035,
        -0.39245447, -0.85864055,  0.08830369,
        -0.50857335,  0.13186633, -0.75497035,
        -0.74470688, -0.4120024 ,  0.08830369
    };
    double tet_qwts[tet_nno] = {
        0.29583885,  0.12821632,  0.16925605,  0.07335544,  0.29583885,
        0.12821632,  0.16925605,  0.07335544
    };
    for (i = 0; i < tet_nno; i++)
    {
        for (j = 0; j < tet_celldim; j++)
        {
            int n = tet_celldim * i + j;
            tet_qpts[n] += 1;
            tet_qpts[n] *= 0.5;
        }
    }

    // hex_qr(3)
    const int hex_nno = 8;
    const int hex_celldim = 3;
    double hex_qpts[hex_nno * hex_celldim] = {
        -0.57735027, -0.57735027, -0.57735027,
         0.57735027, -0.57735027, -0.57735027,
         0.57735027,  0.57735027, -0.57735027,
        -0.57735027,  0.57735027, -0.57735027,
        -0.57735027, -0.57735027,  0.57735027,
         0.57735027, -0.57735027,  0.57735027,
         0.57735027,  0.57735027,  0.57735027,
        -0.57735027,  0.57735027,  0.57735027
    };
    double hex_qwts[hex_nno] = { 1.,  1.,  1.,  1.,  1.,  1.,  1.,  1. };


    int nq, nd;
    double *qx, *qw;

    nq = nd = 0;
    qx = qw = 0;


    if ((pointsPath != "") && (weightsPath != ""))
    {
        int ierr;
        Reader *pointsReader;
        string pointsLoc, pointsFile, pointsExt;
        parse_dataset_path(pointsPath, pointsLoc, pointsFile, pointsExt);
        pointsReader = NewReader(pointsExt.c_str());
        ierr = pointsReader->open(pointsFile.c_str());
        if (ierr < 0)
        {
            cerr << "Could not open quadrature points file " << pointsFile << endl;
            exit(1);
        }
        cout << "quadrature points file = " << pointsFile << endl;
        ierr = pointsReader->get_dataset(pointsLoc.c_str(), &qx, &nq, &nd);
        if (ierr < 0)
        {
            cerr << "Could not access quadrature points in file " << pointsFile << endl;
            exit(1);
        }
        assert(nq > 0);
        assert(nd > 0);
        pointsReader->close();
        delete pointsReader;

        int wtdims[2];
        Reader *weightsReader;
        string weightsLoc, weightsFile, weightsExt;
        parse_dataset_path(weightsPath, weightsLoc, weightsFile, weightsExt);
        weightsReader = NewReader(weightsExt.c_str());
        ierr = weightsReader->open(weightsFile.c_str());
        if (ierr < 0)
        {
            cerr << "Could not open quadrature weights file " << weightsFile << endl;
            exit(1);
        }
        cout << "quadrature weights file = " << weightsFile << endl;
        ierr = weightsReader->get_dataset(weightsLoc.c_str(), &qw, &wtdims[0], &wtdims[1]);
        if (ierr < 0)
        {
            cerr << "Could not access quadrature weights in file " << weightsFile << endl;
            exit(1);
        }
        if (wtdims[0] != nq)
        {
            cerr << "Number of points and weights do not match!" << endl;
            exit(1);
        }
        if (wtdims[1] != 1)
        {
            cerr << "Array of weights is not one-dimensionsal!" << endl;
            exit(1);
        }
        weightsReader->close();
        delete weightsReader;
    }
    else if (quadraturePath != "")
    {
        int ierr;
        string quadratureLoc, quadratureFile, quadratureExt;
        parse_dataset_path(quadraturePath, quadratureLoc, quadratureFile, quadratureExt);
        reader = NewReader(quadratureExt.c_str());
        ierr = reader->open(quadratureFile.c_str());
        if (ierr < 0)
        {
            cerr << "Could not open file " << quadratureFile << endl;
            exit(1);
        }
        cout << "quadrature file = " << quadratureFile << endl;

        if (reader->getType() == Reader::HDF_READER)
        {
            string pointsLoc = quadratureLoc + "/points";
            cout << "quadrature points = " << pointsLoc << endl;
            ierr = reader->get_dataset(pointsLoc.c_str(), &qx, &nq, &nd);
            if (ierr < 0)
            {
                cerr << "Could not read quadrature points from location " << pointsLoc << endl;
                exit(1);
            }

            int wtdims[2];
            string weightsLoc = quadratureLoc + "/weights";
            cout << "quadrature weights = " << weightsLoc << endl;
            ierr = reader->get_dataset(weightsLoc.c_str(), &qw, &wtdims[0], &wtdims[1]);
            if (ierr < 0)
            {
                cerr << "Could not read quadrature weights from location " << weightsLoc << endl;
                exit(1);
            }
            if (wtdims[0] != nq)
            {
                cerr << "Number of points and weights do not match!" << endl;
                exit(1);
            }
            if (wtdims[1] != 1)
            {
                cerr << "Array of weights is not one-dimensional!" << endl;
                exit(1);
            }
        }
        else
        {
            cerr << "Cannot read explicit quadrature rule from a (" << quadratureExt << ") file." << endl;
            exit(1);
        }
    }
    else if (quadratureOrder != "")
    {
        int order;
        string_to_int(quadratureOrder, order);

        if (order > 0)
        {
            /* call FiatProxy
            fiat->set(quadrature);
                int npts,dim;
                double *qx,*qw;
                fiat->quadrature(geometry, order, &qx, &qw, &npts, &dim);
                quadrature->set_quadrature(qx, qw, nno, nsd);
                delete [] qx;
                delete [] qw;
            // */
        }
    }
    else
    {
        // assign reasonable defaults
        switch (cell->geometry())
        {
        case Cell::TRIANGLE:
            quadrature = new QuadraturePoints();
            quadrature->set_quadrature(tri_qpts, tri_qwts, tri_nno, tri_celldim);
            break;
        case Cell::QUADRANGLE:
            quadrature = new QuadraturePoints();
            quadrature->set_quadrature(quad_qpts, quad_qwts, quad_nno, quad_celldim);
            break;
        case Cell::TETRAHEDRON:
            quadrature = new QuadraturePoints();
            quadrature->set_quadrature(tet_qpts, tet_qwts, tet_nno, tet_celldim);
            break;
        case Cell::HEXAHEDRON:
            quadrature = new QuadraturePoints();
            quadrature->set_quadrature(hex_qpts, hex_qwts, hex_nno, hex_celldim);
            break;
        default:
            break;
        }
    }

    if ((qx != 0) && (qw != 0))
    {
        quadrature = new QuadraturePoints();
        quadrature->set_quadrature(qx, qw, nq, nd);
    }

    assert(quadrature != 0);
    assert(quadrature->n_points() > 0);
    assert(quadrature->n_dim() > 0);

    cout << "quadrature rule = "
         << quadrature->n_points()
         << " points with cell dimension "
         << quadrature->n_dim()
         << endl;

    // 
    // now we need to ensure that every point in our
    // quadrature rule is indeed contained inside our cell
    //
    bool all_contained = true;
    for (i = 0; i < quadrature->n_points(); i++)
    {
        double refpt[3] = {0,0,0};
        for (j = 0; j < quadrature->n_dim(); j++)
        {
            refpt[j] = quadrature->refpoint(i,j);
        }
        if (!cell->interior(refpt[0], refpt[1], refpt[2]))
        {
            all_contained = false;
            break;
        }
    }
    if (!all_contained)
    {
        // XXX: add more information here (e.g., actual reference cell used,
        // such as tri3, quad4, tet4, hex8, ...)
        cerr << "QuadratureReader error: "
             << "Some quadrature points lie outside required reference cell"
             << endl;
        exit(1);
    }

    //
    // emit warning if there are any negative quadrature weights?
    //
    bool some_neg_wts = false;
    for (i = 0; i < quadrature->n_points(); i++)
    {
        if (quadrature->weight(i) < 0)
        {
            some_neg_wts = true;
            break;
        }
    }
    if (some_neg_wts)
    {
        //
        // XXX: only emit warning. exit(1) is not necessary...
        //
        cerr << "QuadratureReader warning: "
             << "Some weights are negative"
             << endl;
    }

    //
    // Clean up any local allocations
    //
    if (qx != 0) delete [] qx;
    if (qw != 0) delete [] qw;
}

// ---------------------------------------------------------------------------
