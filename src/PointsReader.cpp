#include <iostream>
#include <cstdlib>
#include <cassert>
#include "PointsReader.h"
#include "StringUtils.h"

using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

PointsReader::PointsReader()
{
    points = 0;
}

PointsReader::~PointsReader()
{
}


// ---------------------------------------------------------------------------

void PointsReader::load_args(AnyOption *opt, const char *opt_prefix)
{
    assert(opt != 0);

    // remember the original option prefix
    this->pointsOption = "--";
    this->pointsOption += opt_prefix;

    // 
    // Note that in this case, the name of key to be read
    // is the whole prefix. The caller needs to determine
    // the appropriate name, which will typically correspond
    // to an option called --{first,second}-{points,values}
    //
    char *in;
    string optstr = opt_prefix;

    in = opt->getValue(optstr.c_str());
    if (in != 0)
    {
        this->pointsPath = in;
    }
}

void PointsReader::validate_args(const char *cmd_name)
{
    // 
    // Check for missing options
    //

    if (pointsPath == "")
    {
        cerr << cmd_name << ": "
             << "Missing option "
             << pointsOption
             << endl;
        exit(1);
    }
}


// ---------------------------------------------------------------------------

void PointsReader::load_points()
{
    int ierr;
    double *pts;
    int m,n;

    pts = 0;
    m = n = 0;


    if (pointsPath == "")
    {
        return;
    }

    parse_dataset_path(pointsPath, pointsLoc, pointsFile, pointsExt);


    pointsReader = NewReader(pointsExt.c_str());

    if ((pointsLoc == "") && (pointsReader->getType() == Reader::HDF_READER))
    {
        pointsLoc = "/points";
    }

    ierr = pointsReader->open(pointsFile.c_str());
    if (ierr < 0)
    {
        cerr << "Could not open points file " << pointsFile << endl;
        exit(1);
    }

    ierr = pointsReader->get_dataset(pointsLoc.c_str(), &pts, &m, &n);
    if (ierr < 0)
    {
        cerr << "Error: Could not open points dataset " << pointsLoc << endl;
        exit(1);
    }

    if (pts != 0)
    {
        points = new Points();
        points->set_data(pts, m, n);
    }

}

// ---------------------------------------------------------------------------
