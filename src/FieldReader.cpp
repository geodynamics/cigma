#include <iostream>
#include <cstdlib>
#include <cassert>
#include "FieldReader.h"

#include "FE_Field.h"
#include "PointField.h"
#include "AnnLocator.h"

#include "StringUtils.h"


using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

FieldReader::FieldReader()
{
    field = 0;
}

FieldReader::~FieldReader()
{
    if (field != 0)
    {
        // XXX:
    }
}

// ---------------------------------------------------------------------------

void FieldReader::load_args(AnyOption *opt, const char *opt_prefix)
{
    assert(opt != 0);

    // remember the original option name
    this->fieldOption = "--";
    this->fieldOption += opt_prefix;

    // deduce all implied command line options
    char *in;
    string optstr;

    // determine fieldPath
    optstr = opt_prefix;
    in = opt->getValue(optstr.c_str());
    if (in != 0)
    {
        fieldPath = in;
    }

    // determine mesh options
    optstr = opt_prefix;
    optstr += "-mesh";
    meshPartReader.load_args(opt, optstr.c_str());

    // determine points option
    optstr = opt_prefix;
    optstr += "-points";
    pointsReader.load_args(opt, optstr.c_str());

    // determine values option
    optstr = opt_prefix;
    optstr += "-values";
    valuesReader.load_args(opt, optstr.c_str());
}


void FieldReader::validate_args(const char *cmd_name)
{
    assert(this->fieldOption != "");

    //
    // Check for incompatible or inconsistent options
    //

    if (fieldPath == "")
    {
        if ((pointsReader.pointsPath == "") && (valuesReader.pointsPath == ""))
        {
            cerr << cmd_name << ": "
                 << "Please specify the option "
                 << fieldOption
                 << endl;
            exit(1);
        }
        else if ((pointsReader.pointsPath != "") || (valuesReader.pointsPath != ""))
        {
            string o1 = fieldOption + "-points";
            string o2 = fieldOption + "-values";
            cerr << cmd_name << ": "
                 << "Please provide the options "
                 << o1 << " and " << o2
                 << endl;
            exit(1);
        }
    }
    else
    {
        if (pointsReader.pointsPath != "")
        {
            string optstr = fieldOption + "-points";
            cerr << cmd_name << ": "
                 << "Can't specify both " << fieldOption
                 << " and " << optstr << endl;
            exit(1);
        }
        if (valuesReader.pointsPath != "")
        {
            string optstr = fieldOption + "-values";
            cerr << cmd_name << ": "
                 << "Can't specify both " << fieldOption
                 << " and " << optstr << endl;
            exit(1);
        }
    }

    meshPartReader.validate_args(cmd_name);

    // 
    // Note that we don't need to call validate_args on either
    // pointsReader or valuesReader
    //

}


// ---------------------------------------------------------------------------

void FieldReader::load_field()
{

    if (fieldPath == "")
    {
        return;
    }

    string fieldLoc, fieldFile, fieldExt;
    parse_dataset_path(fieldPath, fieldLoc, fieldFile, fieldExt);


    field = NewField(fieldPath.c_str());


    if (field->getType() == Field::NULL_FIELD)
    {
        cerr << "Error: Could not create a field dataset for " << fieldPath << endl;
        exit(1);
    }
    else if (field->getType() == Field::FE_FIELD)
    {
        double *dofs;
        int dofs_nno, dofs_valdim;

        FE_Field *fe_field = static_cast<FE_Field*>(field);

        dofsReader = NewReader(fieldExt.c_str());
        dofsReader->open(fieldFile.c_str());
        dofsReader->get_dataset(fieldLoc.c_str(), &dofs, &dofs_nno, &dofs_valdim);
        dofsReader->close();

        if (meshPartReader.meshPath == "")
        {
            meshPartReader.meshPath = fieldFile;

        }
        meshPartReader.load_mesh();
        assert(meshPartReader.meshPart != 0);

        fe_field->dim = meshPartReader.meshPart->nsd;
        fe_field->rank = dofs_valdim;

        fe_field->meshPart = meshPartReader.meshPart;
        fe_field->meshPart->set_cell();
        assert(fe_field->meshPart->cell != 0);

        /*
        if (fe_field->meshPart->nel > 1000)
        {
            if (fe_field->meshPart->nsd == 3)
            {
                AnnLocator *locator = new AnnLocator();
                fe_field->meshPart->set_locator(locator);
            }
        } */

        fe_field->dofHandler = new DofHandler();
        fe_field->dofHandler->set_data(dofs, dofs_nno, dofs_valdim);
    }
    else if (field->getType() == Field::POINT_FIELD)
    {
        double *pts;
        int npts, nsd;

        double *vals;
        int nvals,rank;

        assert(false);
    }
}

// ---------------------------------------------------------------------------
