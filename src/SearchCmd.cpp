#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

#include "SearchCmd.h"
#include "StringUtils.h"

#include "MeshPart.h"
#include "AnnLocator.h"

#include "Reader.h"
#include "Timer.h"

#include "Writer.h"
#include "HdfWriter.h"


using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

SearchCmd::SearchCmd()
{
    name = "search";

    points = 0;
    meshPart = 0;
    coordsField = 0;
}

SearchCmd::~SearchCmd()
{
    // XXX
}

// ---------------------------------------------------------------------------

void SearchCmd::setupOptions(AnyOption *opt)
{

    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("   cigma search [options]");
    opt->addUsage("     --points    Path to search points");
    opt->addUsage("     --mesh      Path to mesh");
    opt->addUsage("     --output    Path to output dataset");

    /* setup flags and options */
    opt->setFlag("help", 'h');
    opt->setFlag("debug");

    // options for mesh
    opt->setOption("mesh");
    opt->setOption("mesh-coordinates");
    opt->setOption("mesh-connectivity");

    // options for points
    opt->setOption("points");

    // options for output
    opt->setOption("output",'o');

    // other options
    opt->setFlag("verbose",'v');
}


void SearchCmd::configure(AnyOption *opt)
{
    string inputstr;
    char *in;

    /* Check if --verbose was enabled */

    verbose = opt->getFlag("verbose");

    /* Determine the output option */
    in = opt->getValue("output");
    if (in != 0)
    {
        inputstr = in;
    }
    string outputLoc, outputExt;
    parse_dataset_path(inputstr, outputLoc, outputFile, outputExt);
    writer = NewWriter(outputExt.c_str());
    if (writer->getType() != Writer::HDF_WRITER)
    {
        cerr << "search: Please specify a target HDF5 file in --output option" << endl;
        exit(1);
    }


    /* Gather up the expected command line arguments */

    meshPartReader.load_args(opt, "mesh");
    pointsReader.load_args(opt, "points");

    meshPartReader.validate_args("search");
    pointsReader.validate_args("search");

    /* load mesh into memory */
    meshPartReader.load_mesh();
    pointsReader.load_points();

    points = pointsReader.points;
    meshPart = meshPartReader.meshPart;

    if (meshPart == 0)
    {
        cerr << "search: Could not load mesh!" << endl;
        exit(1);
    }

    if (points == 0)
    {
        cerr << "search: Could not load points!" << endl;
        exit(1);
    }

    coordsField = new FE_Field();
    coordsField->dim  = meshPart->nsd;
    coordsField->rank = meshPart->nsd;
    coordsField->meshPart = meshPart;
    coordsField->meshPart->set_cell();

    if ((coordsField->meshPart->nsd == 3) && (coordsField->meshPart->nno > 1000))
    {
        AnnLocator *locator = new AnnLocator();
        locator->nnk = 10;
        coordsField->meshPart->set_locator(locator);
    }

    return;
}


// ---------------------------------------------------------------------------

int SearchCmd::run()
{

    assert(points != 0);
    assert(meshPart != 0);


    int i,j;
    int e;
    const int nsd = meshPart->nsd;
    const int nel = meshPart->nel;
    const int npts = points->n_points();

    int *cellPointCount = new int[nel];   // how many points fall on given cell?
    int *pointCellCount = new int[npts];  // how many cells were searched for a given point?

    for (e = 0; e < nel; e++)
    {
        cellPointCount[e] = 0;
    }

    for (i = 0; i < npts; i++)
    {
        pointCellCount[i] = 0;
    }


    time_t t0,t1;
    time(&t0);

    int hitCount = 0;

    if (meshPart->locator != 0)
    {
        for (i = 0; i < points->n_points(); i++)
        {
            //double *pt = (*points)[i];
            double pt[3] = {0,0,0};
            for (j = 0; j < points->n_dim(); j++)
            {
                pt[j] = (*points)(i,j);
            }

            //bool found = meshPart->find_cell(pt, &parentCell);

            bool found = false;

            meshPart->locator->search(pt);
            for (j = 0; j < meshPart->locator->n_idx(); j++)
            {
                e = meshPart->locator->idx(j);
                if (e < 0)
                {
                    continue;
                }
                meshPart->select_cell(e);
                if (meshPart->cell->global_interior(pt))
                {
                    found = true;
                    cellPointCount[e]++;
                    pointCellCount[i] = j+1;
                    break;
                }
            }
            //*
            if (!found)
            {
                hitCount++;
                //cout << i << endl;
                for (e = 0; e < nel; e++)
                {
                    meshPart->select_cell(e);
                    if (meshPart->cell->global_interior(pt))
                    {
                        found = true;
                        cellPointCount[e]++;
                        pointCellCount[i] = e+1;
                        break;
                    }
                }
                assert(found);
            } // */
        }
        cout << "Locator missed " << hitCount << " points "
             << "(" << ((100.0*hitCount)/npts) << "%)"
             << endl;
    }
    else
    {
        for (i = 0; i < points->n_points(); i++)
        {
            double *pt = (*points)[i];
            bool found = false;
            for (e = 0; e < nel; e++)
            {
                meshPart->select_cell(e);
                if (meshPart->cell->global_interior(pt))
                {
                    found = true;
                    cellPointCount[e]++;
                    pointCellCount[i] = e+1;
                    break;
                }
            }
        }
    }

    time(&t1);
    double total_time = t1 - t0;
    cout << "Total time = " << total_time << endl;


    int ierr;

    HdfWriter *hdfWriter = static_cast<HdfWriter*>(writer);

    ierr = hdfWriter->open(outputFile.c_str());
    if (ierr < 0)
    {
        cerr << "search: Could not open output file " << outputFile << endl;
        exit(1);
    }

    string loc;

    loc = "/CellPointCount";
    ierr = hdfWriter->write_int_dataset(loc.c_str(), cellPointCount, nel, 1);
    if (ierr < 0)
    {
        cerr << "search: Could not write dataset " << loc << endl;
        exit(1);
    }

    loc = "/PointCellCount";
    ierr = hdfWriter->write_int_dataset(loc.c_str(), pointCellCount, npts, 1);
    if (ierr < 0)
    {
        cerr << "search: Could not write dataset " << loc << endl;
        exit(1);
    }

    ierr = hdfWriter->close();


    delete [] cellPointCount;
    delete [] pointCellCount;


    return 0;
}

// ---------------------------------------------------------------------------
