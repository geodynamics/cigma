#include <iostream>
#include <ctime>
#include <cassert>

#include "AnnLocator.h"
#include "MeshPart.h"
#include "VtkReader.h"
#include "Misc.h"
#include "StringUtils.h"

#include "Numeric.h"
#include "Cell.h"
#include "Tet.h"
#include "Hex.h"


using namespace std;
using namespace cigma;

int main(int argc, char *argv[])
{
    int npts = 100;

    string filename = "strikeslip_tet4_1000m_t0.vtk";
    string inputstr;

    if (argc > 1)
    {
        filename = argv[1];
    }
    if (argc > 2)
    {
        inputstr = argv[2];
        string_to_int(inputstr, npts);
        assert(npts > 0);
    }


    int i,j;

    int nno, nsd;
    double *coords;
    int nel, ndofs;
    int *connect;


    VtkReader reader;
    reader.open(filename);
    reader.get_coordinates(&coords, &nno, &nsd);
    reader.get_connectivity(&connect, &nel, &ndofs);


    MeshPart *meshPart;
    meshPart = new MeshPart();
    meshPart->set_coordinates(coords, nno, nsd);
    meshPart->set_connectivity(connect, nel, ndofs);
    meshPart->set_cell();

    AnnLocator *locator = new AnnLocator();
    meshPart->set_locator(locator);

    double minpt[nsd], maxpt[nsd];
    meshPart->get_bbox(minpt, maxpt);


    double *points = new double[npts * nsd];
    for (i = 0; i < npts; i++)
    {
        double *pt = &points[nsd*i];
        for (j = 0; j < nsd; j++)
        {
            pt[j] = pick_from_interval(minpt[j],maxpt[j]);
        }
    }



    //int k = locator->nnk;
    //int *candidateCells= new int[k];

    int *parentCells = new int[npts];

    for (i = 0; i < npts; i++)
    {
        parentCells[i] = -1;
    }


    time_t t0, t1;

    cout << "Starting...";
    time(&t0);
    for (i = 0; i < npts; i++)
    {
        bool found = false;
        double *pt = &points[nsd*i];



        /*
        locator->search(pt, candidateCells, k);

        for (j = 0; j < k; j++)
        {
            int e = candidateCells[j];

            meshPart->set_cell(e);

            double uvw[3];
            meshPart->cell->xyz2uvw(pt, uvw);
            found = meshPart->cell->interior(uvw[0], uvw[1], uvw[2]);
            if (found)
            {
                parentCells[i] = e;
                break;
            }
        } // */

        found = meshPart->find_cell(pt, &parentCells[i]);

        assert(found);
    }
    time(&t1);
    cout << "done!" << endl;



    double total_time = (t1 - t0);
    cout << "time: " << total_time << endl;

    cout << "bbox: ";
    for (j = 0; j < nsd; j++)
    {
        cout << minpt[j] << " ";
    }
    for (j = 0; j < nsd; j++)
    {
        cout << maxpt[j] << " ";
    }
    cout << endl;


    cout << "points: " << npts << endl;
    for (i = 0; i < npts; i++)
    {
        double *pt = &points[nsd*i];
        cout << i << " (";
        for (j = 0; j < nsd; j++)
        {
            cout << pt[j];
            if (j != nsd-1)
                cout << " ";
        }
        cout << ") -> " << parentCells[i] << endl;
    }


    delete [] points;
    delete [] parentCells;
    //delete [] candidateCells;


    return 0;
}
