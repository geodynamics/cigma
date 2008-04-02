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
    verbose = false;
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
    assert(cell != 0);

    int i,j;

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
                quadrature->set_quadrature(qw, qx, nno, nsd);
                delete [] qx;
                delete [] qw;
            // */
        }
    }
    else
    {
        // assign reasonable defaults
        quadrature = new QuadraturePoints();
        cell->qr_default(&qw, &qx, &nq, &nd);
        quadrature->set_quadrature(qw, qx, nq, nd);
    }

    if ((qx != 0) && (qw != 0))
    {
        quadrature = new QuadraturePoints();
        quadrature->set_quadrature(qw, qx, nq, nd);
    }

    assert(quadrature != 0);
    assert(quadrature->n_points() > 0);
    assert(quadrature->n_dim() > 0);

    cout << "quadrature rule = "
         << quadrature->n_points()
         << " points on a "
         << quadrature->n_dim()
         << "-d cell"
         << endl;

    if (verbose)
    {
        for (i = 0; i < quadrature->n_points(); i++)
        {
            cout << "\t";
            cout << quadrature->weight(i);
            cout << "\t";
            for (j = 0; j < quadrature->n_dim(); j++)
            {
                cout << quadrature->refpoint(i,j) << " ";
            }
            cout << endl;
        }
    }

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
