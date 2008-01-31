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

void configure_quadrature(AnyOption *opt, QuadratureIO *quadratureIO)
{
    assert(opt != 0);
    assert(quadratureIO != 0);

    char *in;

    in = opt->getValue("order");
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

void read_double_dset(HdfReader *reader, double **data, int *num, int *dim)
{
}

void read_double_dset(TextReader *reader, double **data, int *num, int *dim)
{
}

void read_double_dset(VtkReader *reader, double **data, int *num, int *dim)
{
}

void read_double_dset(Reader *reader, double **data, int *num, int *dim)
{

    if (reader->getType() == Reader::HDF_READER)
    {
        meshPart = new MeshPart();
        HdfReader *hdfReader = static_cast<HdfReader*>(reader);
        read_double_dset(hdfReader, data, num, dim);

    }
    else if (reader->getType() == Reader::TXT_READER)
    {
        meshPart = new MeshPart();
        TextReader *textReader = static_cast<TextReader*>(reader);
        read_double_dset(textReader, data, num, dim);

    }
    else if (reader->getType() == Reader::VTK_READER)
    {
        meshPart = new MeshPart();
        VtkReader *vtkReader = static_cast<VtkReader*>(reader);
        read_double_dset(vtkReader, data, num, dim);
    }

}

// ---------------------------------------------------------------------------

void read_int_dset(HdfReader *reader, int **data, int *num, int *dim)
{
}

void read_int_dset(TextReader *reader, int **data, int *num, int *dim)
{
}

void read_int_dset(VtkReader *reader, int **data, int *num, int *dim)
{
}

void read_int_dset(Reader *reader, int **data, int *num, int *dim)
{

    if (reader->getType() == Reader::HDF_READER)
    {
        meshPart = new MeshPart();
        HdfReader *hdfReader = static_cast<HdfReader*>(reader);
        read_int_dset(hdfReader, data, num, dim);

    }
    else if (reader->getType() == Reader::TXT_READER)
    {
        meshPart = new MeshPart();
        TextReader *textReader = static_cast<TextReader*>(reader);
        read_int_dset(textReader, data, num, dim);

    }
    else if (reader->getType() == Reader::VTK_READER)
    {
        meshPart = new MeshPart();
        VtkReader *vtkReader = static_cast<VtkReader*>(reader);
        read_int_dset(vtkReader, data, num, dim);
    }

}


// ---------------------------------------------------------------------------


void read_coords(Reader *reader, double **coords, int *nno, int *nsd)
{
    read_double_dset(reader, coords, nno, nsd);
}

void read_connect(Reader *reader, int **connect, int *nel, int *ndofs)
{
    read_int_dset(reader, connect, nel, ndofs);
}

void read_dofs(Reader *reader, double **dofs, int *nno, int *valdim)
{
    read_double_dset(reader, dofs, nno, valdim);
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

/*
void MeshIO::configure(AnyOption *opt, const char *cmd, const char *name)
{
    configure_mesh(opt, this, name);
    // check for required options
}*/

void MeshIO::load()
{
    const bool debug = true;

    string mesh_loc, mesh_file, mesh_ext;
    string coords_loc, coords_file, coords_ext;
    string connect_loc, connect_file, connect_ext;

    int nno, nsd;
    double *coords = 0;

    int nel, ndofs;
    int *connect = 0;

    nno = nsd = 0;
    nel = ndofs = 0;


    if (coords_path != "")
    {
        // XXX: use auto_ptr for this local reader, so we can throw exceptions
        Reader *coords_reader;
        parse_dataset_path(coords_path, coords_loc, coords_file, coords_ext);
        new_reader(&coords_reader, coords_ext);
        coords_reader->open(coords_file);

        //read_coords(coords_reader, &coords, &nno, &nsd);

        if (coords_reader->getType() == Reader::HDF_READER)
        {
            HdfReader *hdfReader = static_cast<HdfReader*>(coords_reader);
            hdfReader->get_coordinates(coords_loc, &coords, &nno, &nsd);
            hdfReader->close();
        }
        else if (coords_reader->getType() == Reader::TXT_READER)
        {
            TextReader *textReader = static_cast<TextReader*>(coords_reader);
            textReader->get_coordinates(&coords, &nno, &nsd);
            textReader->close();

        }
        else if (coords_reader->getType() == Reader::VTK_READER)
        {
            VtkReader *vtkReader = static_cast<VtkReader*>(coords_reader);
            vtkReader->get_coordinates(&coords, &nno, &nsd);
            vtkReader->close();
        }
    }

    if (connect_path != "")
    {
        // XXX: use auto_ptr for this local reader, so we can throw exceptions
        Reader *connect_reader;
        parse_dataset_path(connect_path, connect_loc, connect_file, connect_ext);
        new_reader(&connect_reader, connect_ext);
        connect_reader->open(connect_file);

        //read_connect(connect_reader, &nel, &ndofs);

        if (connect_reader->getType() == Reader::HDF_READER)
        {
            HdfReader *hdfReader = static_cast<HdfReader*>(connect_reader);
            hdfReader->get_coordinates(coords_loc, &coords, &nno, &nsd);
            hdfReader->close();
        }
        else if (connect_reader->getType() == Reader::TXT_READER)
        {
            TextReader *textReader = static_cast<TextReader*>(connect_reader);
            textReader->get_coordinates(&coords, &nno, &nsd);
            textReader->close();
        }
        else if (connect_reader->getType() == Reader::VTK_READER)
        {
            VtkReader *vtkReader = static_cast<VtkReader*>(connect_reader);
            vtkReader->get_coordinates(&coords, &nno, &nsd);
            vtkReader->close();
        }
    }

    if ((mesh_path != "") && ((coords == 0) || (connect == 0)))
    {
        // XXX: use auto_ptr for this local reader, so we can throw exceptions
        Reader *mesh_reader;
        parse_dataset_path(mesh_path, mesh_loc, mesh_file, mesh_ext);
        new_reader(&mesh_reader, mesh_ext);
        mesh_reader->open(mesh_file);

        if (mesh_reader->getType() == Reader::HDF_READER)
        {
            coords_loc = mesh_loc + "/coordinates";
            connect_loc = mesh_loc + "/connectivity";
            HdfReader *hdfReader = static_cast<HdfReader*>(mesh_reader);
            hdfReader->get_coordinates(coords_loc, &coords, &nno, &nsd);
            hdfReader->get_connectivity(connect_loc, &connect, &nel, &ndofs);
        }
        else if (mesh_reader->getType() == Reader::TXT_READER)
        {
            TextReader *textReader = static_cast<TextReader*>(mesh_reader);
            textReader->get_coordinates(&coords, &nno, &nsd);
            textReader->get_connectivity(&connect, &nel, &ndofs);
        }
        else if (mesh_reader->getType() == Reader::VTK_READER)
        {
            // read mesh from single vtk file
            VtkReader *vtkReader = static_cast<VtkReader*>(mesh_reader);
            vtkReader->get_coordinates(&coords, &nno, &nsd);
            vtkReader->get_connectivity(&connect, &nel, &ndofs);
        }

        reader->close();

        /*
        if (mesh_ext == ".h5")
        {
            assert(mesh_reader->getType() == Reader::HDF_READER);

            HdfReader *hdfReader = static_cast<HdfReader*>(mesh_reader);
            // open mesh_file in read only mode

            // assert that mesh_path points to an HDF5 group

            // read metadata from that group

            // if no metadata is available, look inside group
            // for two datasets named coordinates & connectivity
            
        }
        else if (mesh_ext == ".txt")
        {
            assert(reader->getType() == Reader::TXT_READER);

            // read mesh from single text file
        }
        else if (mesh_ext == ".vtk")
        {
            assert(reader->getType() == Reader::VTK_READER);

            // read mesh from single vtk file
            VtkReader *vtkReader = static_cast<VtkReader*>(mesh_reader);
            vtkReader->open(mesh_file);
            vtkReader->get_coordinates(&coords, &nno, &nsd);
            vtkReader->get_connectivity(&connect, &nel, &ndofs);
            vtkReader->close();
        } */
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

/*
void QuadratureIO::configure(AnyOption *opt, const char *cmd, const char *name)
{
    configure_quadrature(opt, this, name);
    // check for required options
}*/

void QuadratureIO::load(cigma::Cell *cell)
{
    assert(cell != 0);

    quadrature = new Quadrature();



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



    int order;
    string_to_int(arg_order, order);


    if (reader == 0)
    {
        assert(cell != 0);

        if (order < 0)
        {
            // assign defaults
            switch (cell->geometry())
            {
            case Cell::TRIANGLE:
                quadrature->set_quadrature(tri_qpts, tri_qwts, tri_nno, tri_nsd);
                quadrature->set_globaldim(tri_nsd);
                break;
            case Cell::QUADRANGLE:
                quadrature->set_quadrature(quad_qpts, quad_qwts, quad_nno, quad_nsd);
                quadrature->set_globaldim(quad_nsd);
                break;
            case Cell::TETRAHEDRON:
                quadrature->set_quadrature(tet_qpts, tet_qwts, tet_nno, tet_nsd);
                quadrature->set_globaldim(tet_nsd);
                break;
            case Cell::HEXAHEDRON:
                quadrature->set_quadrature(hex_qpts, hex_qwts, hex_nno, hex_nsd);
                quadrature->set_globaldim(hex_nsd);
                break;
            default:
                break;
            }
        }
        else
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
        switch (reader->getType())
        {
        case Reader::HDF_READER:
            assert(arg_quadrature_path != "");
            break;

        case Reader::TXT_READER:
            assert(arg_points_loc != "");
            assert(arg_weights_loc != "");
            break;
        default:
            break;
        }
    }

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

/*
void FieldIO::configure(AnyOption *opt, const char *cmd, const char *name)
{
    configure_field(opt, this, name);
    // check
}*/

void FieldIO::load()
{

    int nno, nsd;
    int nel, ndofs;
    double *coords;
    int *connect;

    int dofs_nno, dofs_valdim;
    double *dofs;


    // XXX: parse field_path here



    /* XXX: For the following two cases, I need a static initializer on
     * Reader class that instantiates the right subclass based on the
     * detected filetype,
     *
     * After that, here I also need to use a switch statement on the value
     * of reader->getReaderType() in order to downcast the Reader
     * object properly...
     *
     * Having done that, I can rely on function polymorphism to call the right
     * routine: e.g choose between LoadField(VtkReader *reader, ...),
     * LoadField(HdfReader *reader, ...), LoadField(TextReader *reader)
     *
     * For detecting the filetype, one could rely only on the extension,
     * or possibly check for a magic number at the beginning of the file.
     */
    switch (reader->getType())
    {
    case Reader::HDF_READER:
        //XXX: cast to HdfReader
        break;

    case Reader::TXT_READER:
        break;

    case Reader::VTK_READER:
        /* XXX: cast to VtkReader
        //fieldReader.open(inputfile);
        fieldReader->get_coordinates(&coords, &nno, &nsd);
        fieldReader->get_connectivity(&connect, &nel, &ndofs);
        //fieldReader->get_dofs(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
        //fieldReader->get_vector_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
        fieldReader->get_scalar_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
        //fieldReader->get_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
        // */
        break;

    default:
        break;
    }



    field->dim = nsd;
    field->rank = dofs_valdim;
    //field->meshPart = new cigma::VtkUgMeshPart();
    field->meshPart = new cigma::MeshPart();
    field->meshPart->set_coordinates(coords, nno, nsd);
    field->meshPart->set_connectivity(connect, nel, ndofs);


    // move to set_mesh()
    field->meshPart->set_cell();
    assert(field->meshPart->cell != 0);
    //* // XXX: Create locator only when necessary
    cigma::AnnLocator *locator = new cigma::AnnLocator();
    field->meshPart->set_locator(locator);
    // */


    /*
    switch (field->dim)
    {
    case 2:
        // 2D case
        switch (ndofs)
        {
        case 3:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Tri();
            field->fe->quadrature = new cigma::Quadrature();
            //field->fe->quadrature->set_quadrature(tri_qpts, tri_qwts, tri_nno, tri_nsd);
            //field->fe->quadrature->set_globaldim(tri_nsd);
            //field->fe->jxw = new double[tri_nno];
            //field->fe->basis_tab = new double[tri_nno * ndofs];
            //field->fe->basis_jet = new double[tri_nno * ndofs * tri_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        case 4:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Quad();
            field->fe->quadrature = new cigma::Quadrature();
            //field->fe->quadrature->set_quadrature(quad_qpts, quad_qwts, quad_nno, quad_nsd);
            //field->fe->quadrature->set_globaldim(quad_nsd);
            //field->fe->jxw = new double[quad_nno];
            //field->fe->basis_tab = new double[quad_nno * ndofs];
            //field->fe->basis_jet = new double[quad_nno * ndofs * quad_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        }
        break;

    case 3:
        // 3D case
        switch (ndofs)
        {
        case 4:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Tet();
            field->fe->quadrature = new cigma::Quadrature();
            //field->fe->quadrature->set_quadrature(tet_qpts, tet_qwts, tet_nno, tet_nsd);
            //field->fe->quadrature->set_globaldim(3);
            //field->fe->jxw = new double[tet_nno];
            //field->fe->basis_tab = new double[tet_nno * ndofs];
            //field->fe->basis_jet = new double[tet_nno * ndofs * tet_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        case 8:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Hex();
            field->fe->quadrature = new cigma::Quadrature();
            //field->fe->quadrature->set_quadrature(hex_qpts, hex_qwts, hex_nno, hex_nsd);
            //field->fe->quadrature->set_globaldim(hex_nsd);
            //field->fe->jxw = new double[hex_nno];
            //field->fe->basis_tab = new double[hex_nno * ndofs];
            //field->fe->basis_jet = new double[hex_nno * ndofs * hex_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        }
        break;
    }
    field->fe->cell = field->meshPart->cell;
    // */

    //XXX: move to field->set_quadrature(...)
    Quadrature *Q = new cigma::Quadrature();
    //load_quadrature(field->meshPart->cell, Q); XXX

    field->fe = new cigma::FE();
    field->fe->set_cell_quadrature(field->meshPart->cell, Q);


    assert(field->fe != 0);
    assert(field->fe->cell != 0);


    field->dofHandler = new cigma::DofHandler();
    field->dofHandler->meshPart = field->meshPart;
    field->dofHandler->set_data(dofs, dofs_nno, dofs_valdim);

    return;
}



void FieldIO::save()
{
    assert(field != 0);
}


// ---------------------------------------------------------------------------
