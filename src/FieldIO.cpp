#include <iostream>
#include "FieldIO.h"
#include "AnnLocator.h"
#include "StringUtils.h"
#include "Misc.h"

using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------


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


// ---------------------------------------------------------------------------


void FieldIO::save()
{
    cout << "Calling FieldIO::save()" << endl;
    assert(field != 0);
    assert(false);
}

// ---------------------------------------------------------------------------
