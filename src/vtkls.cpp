#include <iostream>
#include <cstdlib>
#include <cassert>

#include "AnyOption.h"
//#include "VtkUgReader.h"

#include <vtkDataSetReader.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>


using namespace std;



// ---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    AnyOption *opt = new AnyOption();

    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " " << "file.vtk" << endl;
        exit(1);
    }
    cout << "Reading " << argv[1] << endl;

    // ------------------------------------------------------
    cout << "<< Creating vtkDataSetReader" << endl;
    vtkDataSetReader *reader = vtkDataSetReader::New();
    reader->SetFileName(argv[1]);

    bool is_pd = reader->IsFilePolyData();
    cout << "Is file poly data? " << is_pd << endl;
    bool is_sp = reader->IsFileStructuredPoints();
    cout << "Is file structured points? " << is_sp << endl;
    bool is_sg = reader->IsFileStructuredGrid();
    cout << "Is file structured grid? " << is_sg << endl;
    bool is_ug = reader->IsFileUnstructuredGrid();
    cout << "Is file unstructured grid? " << is_ug << endl;
    bool is_rg = reader->IsFileRectilinearGrid();
    cout << "Is file rectilinear grid? " << is_rg << endl;
    bool is_valid = is_pd || is_sp || is_sg || is_ug || is_rg;
    assert(is_valid);


    //cout << reader->GetHeader() << endl;
    

    reader->Update();
    //reader->PrintSelf(cout, 0);


    

    int i,n;

    n = reader->GetNumberOfFieldDataInFile();
    cout << "Number of field data = " << n << endl;
    for (i = 0; i < n; i++)
    {
        const char *name = reader->GetFieldDataNameInFile(i);
        cout << "\t" << name << endl;
    }

    n = reader->GetNumberOfScalarsInFile();
    cout << "Number of scalars = " << n << endl;
    for (i = 0; i < n; i++)
    {
        const char *name = reader->GetScalarsNameInFile(i);
        cout << "\t" << name << endl;
    }

    n = reader->GetNumberOfVectorsInFile();
    cout << "Number of vectors = " << n << endl;
    for (i = 0; i < n; i++)
    {
        const char *name = reader->GetVectorsNameInFile(i);
        cout << "\t" << name << endl;
    }

    n = reader->GetNumberOfTensorsInFile();
    cout << "Number of tensors = " << n << endl;
    for (i = 0; i < n; i++)
    {
        const char *name = reader->GetTensorsNameInFile(i);
        cout << "\t" << name << endl;
    }

    // ------------------------------------------------------
    cout << "<< Creating vtkDataSet" << endl;
    vtkDataSet *dataset = reader->GetOutput();
    //dataset->PrintSelf(cout, 0);

    int nno,nel;
    nno = dataset->GetNumberOfPoints();
    nel = dataset->GetNumberOfCells();
    cout << "Number of points = " << nno << endl;
    cout << "Number of cells = " << nel << endl;


    // ------------------------------------------------------
    cout << "<< Creating vtkPointData" << endl;
    vtkPointData *pointData = dataset->GetPointData();
    //pointData->PrintSelf(cout, 0);

    // ------------------------------------------------------
    cout << "<< Creating vtkCellData" << endl;
    vtkCellData *cellData = dataset->GetCellData();
    //cellData->PrintSelf(cout, 0);

    // ------------------------------------------------------
    cout << "<< Sleeping..." << endl;
    sleep(100);

    return 0;
}
