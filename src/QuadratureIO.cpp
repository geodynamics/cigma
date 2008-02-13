#include <iostream>
#include "QuadratureIO.h"
#include "StringUtils.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

void configure_quadrature(AnyOption *opt, QuadratureIO *quadratureIO, const char *opt_prefix)
{
    assert(opt != 0);
    assert(quadratureIO != 0);

    char *in;

    in = opt->getValue("rule-order");
    if (in != 0)
    {
        quadratureIO->quadrature_order = in;
    }

    in = opt->getValue("rule");
    if (in != 0)
    {
        quadratureIO->quadrature_path = in;
    }

    in = opt->getValue("rule-points");
    if (in != 0)
    {
        quadratureIO->points_path = in;
    }

    in = opt->getValue("rule-weights");
    if (in != 0)
    {
        quadratureIO->weights_path = in;
    }
}


// ---------------------------------------------------------------------------

QuadratureIO::QuadratureIO()
{
    quadrature = 0;
}

QuadratureIO::~QuadratureIO()
{
    if (quadrature != 0)
    {
        delete quadrature;
    }
}


// ---------------------------------------------------------------------------

void QuadratureIO::load(cigma::Cell *cell)
{
    cout << "Calling QuadratureIO::load()" << endl;

    assert(cell != 0);

    // XXX: change *_nsd to *_celldim since we are
    // talking about quadrature points in the appropriate
    // reference domain

    const int tri_nno = 1;
    const int tri_nsd = 3;
    double tri_qpts[tri_nno * tri_nsd] = {
        0.0, 0.0, 0.0
    };
    double tri_qwts[tri_nno] = {
        0.0
    };

    const int quad_nno = 1;
    const int quad_nsd = 3;
    double quad_qpts[quad_nno * quad_nsd] = {
        0.0, 0.0, 0.0
    };
    double quad_qwts[quad_nno] = {
        0.0
    };

    const int tet_nno = 8;
    const int tet_nsd = 3;
    double tet_qpts[tet_nno * tet_nsd] = {
        -0.68663473, -0.72789005, -0.75497035,
        -0.83720867, -0.85864055,  0.08830369,
        -0.86832263,  0.13186633, -0.75497035,
        -0.93159441, -0.4120024 ,  0.08830369,
         0.16949513, -0.72789005, -0.75497035,
        -0.39245447, -0.85864055,  0.08830369,
        -0.50857335,  0.13186633, -0.75497035,
        -0.74470688, -0.4120024 ,  0.08830369 };
    double tet_qwts[tet_nno] = {
        0.29583885,  0.12821632,  0.16925605,  0.07335544,  0.29583885,
        0.12821632,  0.16925605,  0.07335544 };
    for (int i = 0; i < tet_nno; i++)
    {
        for (int j = 0; j < tet_nsd; j++)
        {
            int n = tet_nsd * i + j;
            tet_qpts[n] += 1;
            tet_qpts[n] *= 0.5;
        }
    }

    const int hex_nno = 8;
    const int hex_nsd = 3;
    double hex_qpts[8*3] = {
        -0.57735027, -0.57735027, -0.57735027,
         0.57735027, -0.57735027, -0.57735027,
         0.57735027,  0.57735027, -0.57735027,
        -0.57735027,  0.57735027, -0.57735027,
        -0.57735027, -0.57735027,  0.57735027,
         0.57735027, -0.57735027,  0.57735027,
         0.57735027,  0.57735027,  0.57735027,
        -0.57735027,  0.57735027,  0.57735027 };
    double hex_qwts[8*3] = { 1.,  1.,  1.,  1.,  1.,  1.,  1.,  1. };


    int nq,nd;
    double *qx, *qw;

    nq = 0;
    nd = 0;
    qx = 0;
    qw = 0;

    if (quadrature_path != "")
    {
        string quadrature_loc, quadrature_file, quadrature_ext;
        parse_dataset_path(quadrature_path, quadrature_loc, quadrature_file, quadrature_ext);
        new_reader(&reader, quadrature_ext);

        if (reader->getType() == Reader::HDF_READER)
        {
            // Load quadrature from HDF5 file
        }
        else if (reader->getType() == Reader::TXT_READER)
        {
            // Load quadrature from 
        }
        else if (reader->getType() == Reader::VTK_READER)
        {
            // XXX: not supported!
            assert(false);
        }
    }
    else if (quadrature_order != "")
    {
        int order;
        string_to_int(quadrature_order, order);

        if (order > 0)
        {
            /* call FiatProxy
            //fiat->set(quadrature);
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
            quadrature = new Quadrature();
            quadrature->set_quadrature(tri_qpts, tri_qwts, tri_nno, tri_nsd);
            quadrature->set_globaldim(tri_nsd);
            break;
        case Cell::QUADRANGLE:
            quadrature = new Quadrature();
            quadrature->set_quadrature(quad_qpts, quad_qwts, quad_nno, quad_nsd);
            quadrature->set_globaldim(quad_nsd);
            break;
        case Cell::TETRAHEDRON:
            quadrature = new Quadrature();
            quadrature->set_quadrature(tet_qpts, tet_qwts, tet_nno, tet_nsd);
            quadrature->set_globaldim(tet_nsd);
            break;
        case Cell::HEXAHEDRON:
            quadrature = new Quadrature();
            quadrature->set_quadrature(hex_qpts, hex_qwts, hex_nno, hex_nsd);
            quadrature->set_globaldim(hex_nsd);
            break;
        default:
            break;
        }
    }

    if ((qx != 0) && (qw != 0))
    {
        quadrature = new Quadrature();
        quadrature->set_quadrature(qx, qw, nq, nd);
        quadrature->set_globaldim(3); // XXX: how to treat 2D case?
    }

    assert(quadrature != 0);
    assert(quadrature->n_points() > 0);
    assert(quadrature->n_refdim() > 0);
    assert(quadrature->n_globaldim() > 0);
}

// ---------------------------------------------------------------------------
