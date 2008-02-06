#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>

#include "Misc.h"
#include "AnnLocator.h"
#include "StringUtils.h"

#include "HdfReader.h"
#include "TextReader.h"
#include "VtkReader.h"

#include "HdfWriter.h"
#include "TextWriter.h"
#include "VtkWriter.h"

using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

double pick_from_interval(double a, double b)
{
    return a + (b - a) * rand()/(RAND_MAX + 1.0);
}


void bbox_random_point(double minpt[3], double maxpt[3], double x[3])
{
    const int nsd = 3;
    for (int i = 0; i < nsd; i++)
    {
        assert(minpt[i] <= maxpt[i]);
        x[i] = pick_from_interval(minpt[i], maxpt[i]);
    }
}


// ---------------------------------------------------------------------------

void new_reader(Reader **reader, string ext)
{
    if (ext == ".h5")
    {
        *reader = new HdfReader();
        return;
    }

    if (ext == ".txt")
    {
        *reader = new TextReader();
        return;
    }

    if (ext == ".vtk")
    {
        *reader = new VtkReader();
        return;
    }

}

void new_writer(Writer **writer, string ext)
{
    if (ext == ".h5")
    {
        *writer = new HdfWriter();
        return;
    }

    if (ext == ".txt")
    {
        *writer = new TextWriter();
        return;
    }

    if (ext == ".vtk")
    {
        *writer = new VtkWriter();
        return;
    }

}


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


void configure_mesh(AnyOption *opt, MeshIO *meshIO, const char *opt_prefix)
{
    assert(opt != 0);
    assert(meshIO != 0);

    char *in;
    string optstr;
    string mesh_name = opt_prefix;

    in = opt->getValue(mesh_name.c_str());
    if (in != 0)
    {
        meshIO->mesh_path = in;
    }

    optstr = mesh_name + "-coordinates";
    in = opt->getValue(optstr.c_str());
    if (in != 0)
    {
        meshIO->coords_path = in;
    }

    optstr = mesh_name + "-connectivity";
    in = opt->getValue(optstr.c_str());
    if (in != 0)
    {
        meshIO->connect_path = in;
    }
}

void configure_field(AnyOption *opt, FieldIO *fieldIO, const char *opt_prefix)
{
    assert(opt != 0);
    assert(fieldIO != 0);

    char *in;
    string field_name = opt_prefix;
    string mesh_name = field_name + "-mesh";

    in = opt->getValue(field_name.c_str());
    if (in != 0)
    {
        fieldIO->field_path = in;
    }

    configure_mesh(opt, &(fieldIO->meshIO), mesh_name.c_str());

}

// ---------------------------------------------------------------------------

MeshIO::MeshIO()
{
    meshPart = 0;
}

MeshIO::~MeshIO()
{
    if (meshPart != 0)
    {
        // XXX: traverse meshPart structure and delete everything
    }
}

void MeshIO::load()
{
    cout << "Calling MeshIO::load()" << endl;

    string mesh_loc, mesh_file, mesh_ext;
    string coords_loc, coords_file, coords_ext;
    string connect_loc, connect_file, connect_ext;

    int nno, nsd;
    double *coords = 0;

    int nel, ndofs;
    int *connect = 0;

    nno = nsd = 0;
    nel = ndofs = 0;

    // XXX: use auto_ptr for the local readers, so we can throw exceptions
    if (coords_path != "")
    {
        Reader *coords_reader;
        parse_dataset_path(coords_path, coords_loc, coords_file, coords_ext);
        new_reader(&coords_reader, coords_ext);
        coords_reader->open(coords_file);
        coords_reader->get_coordinates(coords_loc.c_str(), &coords, &nno, &nsd);
        //coords_reader->close();
    }

    if (connect_path != "")
    {
        Reader *connect_reader;
        parse_dataset_path(connect_path, connect_loc, connect_file, connect_ext);
        new_reader(&connect_reader, connect_ext);
        connect_reader->open(connect_file);
        connect_reader->get_connectivity(connect_loc.c_str(), &connect, &nel, &ndofs);
        //connect_reader->close();
    }

    if ((mesh_path != "") && ((coords == 0) || (connect == 0)))
    {
        Reader *mesh_reader;
        parse_dataset_path(mesh_path, mesh_loc, mesh_file, mesh_ext);
        new_reader(&mesh_reader, mesh_ext);
        mesh_reader->open(mesh_file);
        if (coords == 0)
        {
            if (mesh_reader->getType() == Reader::HDF_READER)
            {
                coords_loc = mesh_loc + "/coordinates";
            }
            mesh_reader->get_coordinates(coords_loc.c_str(), &coords, &nno, &nsd);
        }
        if (connect == 0)
        {
            if (mesh_reader->getType() == Reader::HDF_READER)
            {
                connect_loc = mesh_loc + "/connectivity";
            }
            mesh_reader->get_connectivity(connect_loc.c_str(), &connect, &nel, &ndofs);
        }
        //mesh_reader->close();
    }

    if ((coords != 0) && (connect != 0))
    {
        meshPart = new MeshPart();

        meshPart->nno = nno;
        meshPart->nsd = nsd;
        meshPart->coords = coords;

        meshPart->nel = nel;
        meshPart->ndofs = ndofs;
        meshPart->connect = connect;
    }
    else
    {
        if (coords == 0)
        {
            cerr << "MeshIO::load() error: Could not find mesh coordinates";
            cerr << endl;
        }
        if (connect == 0)
        {
            cerr << "MeshIO::load() error: Could not find mesh connectivity";
            cerr << endl;
        }
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

        }
        else if (reader->getType() == Reader::TXT_READER)
        {

        }
        else if (reader->getType() == Reader::VTK_READER)
        {

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

FieldIO::FieldIO()
{
    field = 0;
}

FieldIO::~FieldIO()
{
    if (field != 0)
    {
        // XXX: traverse field structure and delete everything
    }
}

void FieldIO::load()
{
    cout << "Calling FieldIO::load()" << endl;

    int dofs_nno, dofs_valdim;
    double *dofs;

    string dofs_loc, dofs_file, dofs_ext;

    if (field_path != "")
    {
        parse_dataset_path(field_path, dofs_loc, dofs_file, dofs_ext);
        new_reader(&reader, dofs_ext);
        reader->open(dofs_file);
        reader->get_dataset(dofs_loc.c_str(), &dofs, &dofs_nno, &dofs_valdim);
        //reader->close();


        if (meshIO.mesh_path == "")
        {
            meshIO.mesh_path = dofs_file;
        }

        meshIO.load();
        assert(meshIO.meshPart != 0);


        field = new FE_Field();

        field->dim = meshIO.meshPart->nsd;
        field->rank = dofs_valdim;
        
        field->meshPart = meshIO.meshPart;
        field->meshPart->set_cell();
        assert(field->meshPart->cell != 0);

        // XXX: Instantiate Locator only when necessary!
        // XXX: Decide threshold based on number of elements?
        if (field->meshPart->nel > 1000)
        {
            AnnLocator *locator = new AnnLocator();
            field->meshPart->set_locator(locator);
        }

        field->dofHandler = new DofHandler();
        field->dofHandler->set_data(dofs, dofs_nno, dofs_valdim);
    }

    return;
}



void FieldIO::save()
{
    cout << "Calling FieldIO::save()" << endl;
    assert(field != 0);
    assert(false);
}


// ---------------------------------------------------------------------------
