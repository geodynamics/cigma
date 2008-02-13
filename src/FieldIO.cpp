#include <iostream>
#include "FieldIO.h"
#include "AnnLocator.h"
#include "StringUtils.h"
#include "Misc.h"

using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------


void load_args(AnyOption *opt, FieldIO *fieldIO, const char *opt_prefix)
{
    assert(opt != 0);
    assert(fieldIO != 0);

    // remember the original option name
    fieldIO->field_option  = "--";
    fieldIO->field_option += opt_prefix;

    // deduce all implied command line options
    char *in;
    string field_name = opt_prefix;
    string mesh_name = field_name + "-mesh";


    in = opt->getValue(field_name.c_str());
    if (in != 0)
    {
        fieldIO->field_path = in;
    }

    load_args(opt, &(fieldIO->meshIO), mesh_name.c_str());

}

void validate_args(FieldIO *fieldIO, const char *cmd_name)
{
    assert(fieldIO != 0);
    assert(fieldIO->field_option != "");

    //
    // Check for incompatible/inconsistent options
    //

    if (fieldIO->field_path == "")
    {
        cerr << cmd_name
             << ": Please specify the option "
             << fieldIO->field_option
             << endl;
        exit(1);
    }

    validate_args(&(fieldIO->meshIO), cmd_name);

    // XXX: thinking about it...maybe Quadrature should be associated
    // with a mesh, not with FE_Field. Of course, the field->fe object should
    // only be initialized after we know which mesh to associate with
    //validate_args(&(fieldIO->quadratureIO), cmd_name);


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


// ---------------------------------------------------------------------------


void FieldIO::load()
{
    cout << "Calling FieldIO::load()" << endl;

    int dofs_nno, dofs_valdim;
    double *dofs;

    string dofs_loc, dofs_file, dofs_ext;

    if (field_path != "")
    {
        // 
        // XXX: How should one decide whether we need to instantiate
        // FE_Field, PointField, or an ExternalField?
        //


        // XXX: for now, assume we are instantiating an FE_Field.
        // note that we are also assuming that dofs_loc points to a dataset.
        // if dofs_loc points to an HDF5 group, we'll need to read its metadata
        // to determine how the data should be interpreted. even if dofs_loc
        // is a dataset, we should check whether it has its mesh metadata
        // properly set. lastly, the FunctionSpace attribute should always
        // when reading from HDF5 files.
        parse_dataset_path(field_path, dofs_loc, dofs_file, dofs_ext);
        new_reader(&reader, dofs_ext);
        reader->open(dofs_file);
        reader->get_dataset(dofs_loc.c_str(), &dofs, &dofs_nno, &dofs_valdim);
        //reader->close();


        if (meshIO.mesh_path == "")
        {
            // XXX: for now, assume that only --field-mesh is set,
            // and that it points to a file...i think we also need to
            // set coords_path and connect_path. lastly, don't forget
            // about --field-mesh-coordinates and --field-mesh-connectivity.
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


// ---------------------------------------------------------------------------


void FieldIO::save()
{
    cout << "Calling FieldIO::save()" << endl;
    assert(field != 0);
    assert(false);
}

// ---------------------------------------------------------------------------
