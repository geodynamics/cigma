#include <iostream>
#include <cstdlib>
#include <cassert>
#include "FieldReader.h"
#include "FE_Field.h"
#include "PointField.h"
#include "NullField.h"
#include "AnnLocator.h"
#include "StringUtils.h"


using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

FieldReader::FieldReader()
{
    field = 0;
    fieldSet = 0;
    verbose = false;
}

FieldReader::~FieldReader()
{
    if (field != 0)
    {
        // XXX
    }
}

void FieldReader::setFieldSet(FieldSet *fs)
{
    this->fieldSet = fs;
}


// ---------------------------------------------------------------------------

void FieldReader::load_args(AnyOption *opt, const char *opt_prefix)
{
    assert(opt != 0);

    // remember the original option name
    this->fieldOption = opt_prefix;

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
                 << "--" << fieldOption
                 << endl;
            exit(1);
        }
        else if (((pointsReader.pointsPath == "") && (valuesReader.pointsPath != ""))
               ||((pointsReader.pointsPath != "") && (valuesReader.pointsPath == "")))
        {
            string o1 = fieldOption + "-points";
            string o2 = fieldOption + "-values";
            cerr << cmd_name << ": "
                 << "Please provide both options "
                 << "--" << o1 << " and "
                 << "--" << o2
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
                 << "Can't specify both "
                 << "--" << fieldOption
                 << " and "
                 << "--" << optstr << endl;
            exit(1);
        }
        if (valuesReader.pointsPath != "")
        {
            string optstr = fieldOption + "-values";
            cerr << cmd_name << ": "
                 << "Can't specify both "
                 << "--" << fieldOption
                 << " and "
                 << "--" << optstr << endl;
            exit(1);
        }
    }

    meshPartReader.validate_args(cmd_name);

    // 
    // Note that we don't need to call validate_args on either
    // pointsReader or valuesReader, as the equivalent checks
    // have been performed above.
    //

}

bool FieldReader::fieldPathIsZero()
{
    return (fieldPath == "zero");
}

// ---------------------------------------------------------------------------

void FieldReader::load_field()
{
    //
    // First thing to do is look up fieldPath in the fieldSet.
    //
    if (fieldSet != 0)
    {
        field = fieldSet->getField(fieldPath);
        if (field != 0)
        {
            if (verbose)
            {
                cout << endl;

                cout << fieldOption << " field path = "
                     << fieldPath
                     << endl;

                if ((field->n_dim() > 0) && (field->n_rank() > 0))
                {
                    cout << "rank " << field->n_rank()
                         << " on a " << field->n_dim() << "-d domain"
                         << endl;
                }
            }
            return;
        }
    }
    

    // 
    // XXX: for now, assume fieldPath is for setting an FE_Field()
    // To specify points, use the --{first,second}-{points,values} options.
    //
    //field = NewField(fieldPath.c_str());
    if (fieldPath != "")
    {
        //field = NewField(fieldPath.c_str());
        field = new FE_Field();

        string fieldLoc, fieldFile, fieldExt;
        parse_dataset_path(fieldPath, fieldLoc, fieldFile, fieldExt);

        double *dofs;
        int dofs_nno, dofs_valdim;

        FE_Field *fe_field = static_cast<FE_Field*>(field);


        /* Read dofs from file */
        int ierr;
        dofsReader = NewReader(fieldExt.c_str());
        ierr = dofsReader->open(fieldFile.c_str());
        if (ierr < 0)
        {
            cerr << "Error: Could not open " << fieldOption
                 << " field path '" << fieldFile << "'"
                 << endl;
            exit(1);
        }

        ierr = dofsReader->get_dataset(fieldLoc.c_str(), &dofs, &dofs_nno, &dofs_valdim);
        if (ierr < 0)
        {
            cerr << "Error: Could not open " << fieldOption
                 << " dataset '" << fieldLoc
                 << "' from " << fieldPath
                 << endl;
            exit(1);
        }
        dofsReader->close();

        /* Assign a dof handler */
        fe_field->dofHandler = new DofHandler();
        fe_field->dofHandler->set_data(dofs, dofs_nno, dofs_valdim);


        /* Read mesh from appropriate path */
        if (meshPartReader.meshPath == "")
        {
            meshPartReader.meshPath = fieldFile;

        }
        meshPartReader.load_mesh();
        assert(meshPartReader.meshPart != 0);


        /* Assign shape */
        fe_field->dim = meshPartReader.meshPart->nsd;
        fe_field->rank = dofs_valdim;

        /* Assign a mesh */
        fe_field->set_mesh(meshPartReader.meshPart);
        assert(fe_field->meshPart->cell != 0);
        
        /* Assign a locator */
        if (fe_field->meshPart->nel > 1000)
        {
            // Why was I testing for (fieldOption == "first")?
            AnnLocator *locator = new AnnLocator();
            fe_field->meshPart->set_locator(locator);
        }


        /* 
         * Next item to set must be fe_field->fe, but this can be deferred.
         * The caller is responsible for setting this field using the
         * appropriate set_fe() method.
         */


        /* Verbose output */
        if (verbose)
        {
            cout << endl;

            cout << fieldOption << " field path = "
                 << fieldPath
                 << endl;

            cout << fieldOption << " field dimensions = "
                 << "rank " << fe_field->n_rank() << ", "
                 << fe_field->meshPart->nel << " cells, "
                 << fe_field->meshPart->nno << " nodes, "
                 << fe_field->meshPart->cell->n_nodes() << " dofs/cell"
                 << endl;
        }
    }
    else if ((pointsReader.pointsPath != "") && (valuesReader.pointsPath != ""))
    {
        Points *points = 0;
        Points *values = 0;

        pointsReader.load_points();
        valuesReader.load_points();

        points = pointsReader.points;
        values = valuesReader.points;

        if ((points != 0) && (values != 0))
        {
            field = new PointField();

            PointField *pf = static_cast<PointField*>(field);
            pf->set_points(points->data, points->n_points(), points->n_dim());
            pf->set_values(values->data, values->n_points(), values->n_dim());

            if (points->n_points() > 1000)
            {
                AnnLocator *locator = new AnnLocator();
                locator->nnk = 20;
                pf->points->set_locator(locator);
            }

            if (verbose)
            {
                cout << endl;

                cout << fieldOption << " field points = "
                     << points->n_points() << " points, "
                     << points->n_dim() << " dim"
                     << endl;

                cout << fieldOption << " field values = "
                     << values->n_points() << " values, "
                     << values->n_dim() << " dim"
                     << endl;
            }
        }
        else
        {
            if (points == 0)
            {
                cerr << "Error: Could not load points from "
                     << pointsReader.pointsPath
                     << endl;
            }
            if (values == 0)
            {
                cerr << "Error: Could not load values from "
                     << valuesReader.pointsPath
                     << endl;
            }
            exit(1);
        }
    }
    else
    {
        // XXX: until we improve the factory method
        field = new NullField();
    }
}

// ---------------------------------------------------------------------------
