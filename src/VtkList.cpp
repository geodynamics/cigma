#include <cassert>
#include <cstdlib>

#include "VtkList.h"
#include "VtkReader.h"

#include "vtkDataSet.h"
#include "vtkPointData.h"
#include "vtkCellData.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

static void list_data(vtkDataSet *dataset)
{
    assert(dataset != 0);

    vtkPointData *pointData = dataset->GetPointData();
    //pointData->PrintSelf(cout, 0);

    vtkCellData *cellData = dataset->GetCellData();
    //cellData->PrintSelf(cout, 0);


    // ---------------------------------------------------

    int nno, nel;

    nno = dataset->GetNumberOfPoints();
    nel = dataset->GetNumberOfCells();

    if (nno > 0)
    {
        cout << "Points = " << nno << endl;
    }

    if (nel > 0)
    {
        cout << "Cells = " << nel << endl;
    }


    // ---------------------------------------------------
    int i, n;
    int numArrays;
    int numTuples, numComponents;

    numArrays = pointData->GetNumberOfArrays();
    if (numArrays > 0)
    {
        for (i = 0; i < numArrays; i++)
        {
            vtkDataArray *dataArray = pointData->GetArray(i);
            const char *name = pointData->GetArrayName(i);
            numTuples = dataArray->GetNumberOfTuples();
            numComponents = dataArray->GetNumberOfComponents();
            cout << "PointDataArray[" << i << "] = " << name << " ";
            //cout << dataArray->GetClassName() << " ";
            cout << "(" << numTuples << " x " << numComponents << ")";
            cout << endl;
        }
    }

    numArrays = cellData->GetNumberOfArrays();
    if (numArrays > 0)
    {
        for (i = 0; i < numArrays; i++)
        {
            vtkDataArray *dataArray = cellData->GetArray(i);
            const char *name = cellData->GetArrayName(i);
            numTuples = dataArray->GetNumberOfTuples();
            numComponents = dataArray->GetNumberOfComponents();
            cout << "CellDataArray[" << i << "] = " << name << " ";
            //cout << dataArray->GetClassName() << " ";
            cout << "(" << numTuples << " x " << numComponents << ")";
            cout << endl;
        }
    }

    return;
}

// ---------------------------------------------------------------------------

void vtk_list(const char *filename)
{
    VtkReader vtkreader;
    vtkreader.open(filename);

    if (vtkreader.dataset != 0)
    {
        list_data(vtkreader.dataset);
    }
    else
    {
        cerr << "Could not read " << filename << endl;
        exit(1);
    }
}
