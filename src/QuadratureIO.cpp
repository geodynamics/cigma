#include <iostream>
#include "QuadratureIO.h"
#include "StringUtils.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

void load_args(AnyOption *opt, QuadratureIO *quadratureIO, const char *opt_prefix)
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

void validate_args(QuadratureIO *quadratureIO, const char *cmd_name)
{
    assert(quadratureIO != 0);

    //
    // Check for incompatible/inconsistent options
    //

    if ((quadratureIO->points_path == "") && (quadratureIO->weights_path != ""))
    {
        cerr << cmd_name
             << ": You also need to set the option --rule-points"
             << endl;

        exit(1);
    }

    if ((quadratureIO->weights_path == "") && (quadratureIO->points_path != ""))
    {
        cerr << cmd_name
             << ": You also need to set the option --rule-weights"
             << endl;

        exit(1);
    }
    
    if ((quadratureIO->weights_path != "") && (quadratureIO->points_path != ""))
    {
        if (quadratureIO->quadrature_path != "")
        {
            cerr << cmd_name
                 << ": Using explicit points and weights (--rule not necessary)"
                 << endl;

            exit(1);
        }
    }

    if ((quadratureIO->quadrature_order != "")
            && ((quadratureIO->quadrature_path != "") 
            || ((quadratureIO->points_path != "") && (quadratureIO->weights_path != ""))))
    {
        cerr << cmd_name
             << ": Cannot use --rule-order with an explicit quadrature rule"
             << endl;

        exit(1);
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

    int i,j;
    int ierr;


    //
    // XXX: move these default rules into the appropriate Cell subclasses
    //

    // tri_qr(5)
    const int tri_nno = 9;
    const int tri_celldim = 2;
    const int tri_nsd = 3;
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
    const int quad_nsd = 3;
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
    const int tet_nsd = 3;
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
    const int hex_nsd = 3;
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


    int nq,nd;
    double *qx, *qw;

    nq = 0;
    nd = 0;
    qx = 0;
    qw = 0;


    if ((points_path != "") && (weights_path != ""))
    {
        Reader *points_reader;
        string points_loc, points_file, points_ext;
        parse_dataset_path(points_path, points_loc, points_file, points_ext);
        new_reader(&points_reader, points_ext);
        ierr = points_reader->open(points_file);
        assert(ierr == 0);
        cout << "quadrature points file = " << points_file << endl;
        points_reader->get_dataset(points_loc.c_str(), &qx, &nq, &nd);
        assert(nq > 0);
        assert(nd > 0);
        //points_reader->close();

        int wtdims[2];
        Reader *weights_reader;
        string weights_loc, weights_file, weights_ext;
        parse_dataset_path(weights_path, weights_loc, weights_file, weights_ext);
        new_reader(&weights_reader, weights_ext);
        ierr = weights_reader->open(weights_file);
        assert(ierr == 0);
        cout << "quadrature weights file = " << weights_file << endl;
        weights_reader->get_dataset(weights_loc.c_str(), &qw, &wtdims[0], &wtdims[1]);
        assert(wtdims[0] == nq);
        assert(wtdims[1] == 1);
        //weights_reader->close();
    }
    else if (quadrature_path != "")
    {
        int ierr;
        string quadrature_loc, quadrature_file, quadrature_ext;
        parse_dataset_path(quadrature_path, quadrature_loc, quadrature_file, quadrature_ext);
        new_reader(&reader, quadrature_ext);
        ierr = reader->open(quadrature_file);
        assert(ierr == 0);
        cout << "quadrature file = " << quadrature_file << endl;

        if (reader->getType() == Reader::HDF_READER)
        {
            string points_loc = quadrature_loc + "/points";
            cout << "quadrature points = " << points_loc << endl;
            reader->get_dataset(points_loc.c_str(), &qx, &nq, &nd);

            int wtdims[2];
            string weights_loc = quadrature_loc + "/weights";
            cout << "quadrature weights = " << weights_loc << endl;
            reader->get_dataset(weights_loc.c_str(), &qw, &wtdims[0], &wtdims[1]);
            assert(wtdims[0] == nq);
            assert(wtdims[1] == 1);
            
        }
        else if (reader->getType() == Reader::TXT_READER)
        {
            int rows, cols;
            double *wxdata;
            reader->get_dataset(0, &wxdata, &rows, &cols);
            nq = rows;
            nd = cols - 1;
            assert(nq >= 1);
            assert(nd >= 1);
            qx = new double[nq*nd];
            qw = new double[nq];
            for (i = 0; i < nq; i++)
            {
                qw[i] = wxdata[cols*i + 0];
                for (j = 0; j < nd; j++)
                {
                    qx[nd*i + j] = wxdata[cols*i + (j+1)];
                }
            }
            delete [] wxdata;
        }
        else if (reader->getType() == Reader::VTK_READER)
        {
            assert(false); // XXX: VtkReader not supported!
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
            quadrature->set_quadrature(tri_qpts, tri_qwts, tri_nno, tri_celldim);
            quadrature->set_globaldim(tri_nsd);
            break;
        case Cell::QUADRANGLE:
            quadrature = new Quadrature();
            quadrature->set_quadrature(quad_qpts, quad_qwts, quad_nno, quad_celldim);
            quadrature->set_globaldim(quad_nsd);
            break;
        case Cell::TETRAHEDRON:
            quadrature = new Quadrature();
            quadrature->set_quadrature(tet_qpts, tet_qwts, tet_nno, tet_celldim);
            quadrature->set_globaldim(tet_nsd);
            break;
        case Cell::HEXAHEDRON:
            quadrature = new Quadrature();
            quadrature->set_quadrature(hex_qpts, hex_qwts, hex_nno, hex_celldim);
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

    cout << "quadrature rule = "
         << quadrature->n_points() << " points, "
         << "celldim " << quadrature->n_refdim() << ", "
         << "dim " << quadrature->n_globaldim()
         << endl;

    // 
    // XXX: now we need to ensure that every point in our
    // quadrature rule is indeed contained inside our cell.
    //
    bool all_contained = true;
    for (i = 0; i < quadrature->n_points(); i++)
    {
        double refpt[3] = {0,0,0};
        for (j = 0; j < quadrature->n_refdim(); j++)
        {
            refpt[j] = quadrature->point(i,j);
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
        cerr << "QuadratureIO::load() error: "
             << "Not all points provided lie inside required reference cell"
             << endl;
        exit(1);
    }

    //
    // XXX: emit warning if there are any negative quadrature weights?
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
        cerr << "QuadratureIO::load() warning: "
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
