#include <cassert>
#include "VtkList.h"

#include "vtkDataSetReader.h"
#include "vtkXMLStructuredGridReader.h"

#include "vtkDataSet.h"
#include "vtkStructuredGrid.h"

#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"


using namespace std;


// ---------------------------------------------------------------------------

/*
VtkFileType getFileType(vtkDataSetReader *reader)
{
    // XXX: are these mutually exclusive?
    if (reader->IsFilePolyData())
        return VTK_FILE_POLYDATA;
    if (reader->IsFileStructuredPoints())
        return VTK_FILE_STRUCTURED_POINTS;
    if (reader->IsFileStructuredGrid())
        return VTK_FILE_STRUCTURED_GRID;
    if (reader->IsFileUnstructuredGrid())
        return VTK_FILE_UNSTRUCTURED_GRID;
    if (reader->IsFileRectilinearGrid())
        return VTK_FILE_RECTILINEAR_GRID;
    return VTK_FILE_NONE;

} // */


const char *getFileTypeName(VtkFileType fileType)
{
    if (fileType == VTK_FILE_POLYDATA)
        return "polydata";
    if (fileType == VTK_FILE_STRUCTURED_POINTS)
        return "structured points";
    if (fileType == VTK_FILE_STRUCTURED_GRID)
        return "structured grid";
    if (fileType == VTK_FILE_UNSTRUCTURED_GRID)
        return "unstructured grid";
    if (fileType == VTK_FILE_RECTILINEAR_GRID)
        return "rectilinear grid";
    return "none";
}


// ---------------------------------------------------------------------------

void vtkls(vtkDataSet *dataset)
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

void list_vtk(const char *filename)
{
    int outputType;
    vtkDataSetReader *reader = vtkDataSetReader::New();

    reader->SetFileName(filename);
    cout << "Reading " << filename << endl;

    int ierr = -1;

    ierr = reader->OpenVTKFile();   // does file exist?
    if (ierr == 0)
    {
        cerr << "Could not open " << filename << endl;
        exit(1);
    }

    ierr = reader->ReadHeader();    // is the vtk header present?
    if (ierr == 0)
    {
        cerr << "Unrecognized file " << filename << endl;
        exit(1);
    }

    outputType = reader->ReadOutputType();
    if (outputType < 0)
    {
        cerr << "Invalid VTK file? " << filename << endl;
        exit(1);
    }


    // ---------------------------------------------------
    reader->Update();
    //reader->PrintSelf(cout, 0);

    vtkDataSet *dataset = reader->GetOutput();
    //dataset->PrintSelf(cout, 0);

    const bool group_by_arrays = true;
    const bool group_by_class = false;

    if (group_by_arrays)
    {
        vtkls(dataset);
    }

    if (group_by_class)
    {
        int n, i;

        n = reader->GetNumberOfFieldDataInFile();
        for (i = 0; i < n; i++)
        {
            const char *name = reader->GetFieldDataNameInFile(i);
            //cout << "Number of field data = " << n << endl;
            cout << "FieldData[" << i << "] = " << name;
            cout << endl;
        }

        n = reader->GetNumberOfScalarsInFile();
        //cout << "Number of scalars = " << n << endl;
        for (i = 0; i < n; i++)
        {
            const char *name = reader->GetScalarsNameInFile(i);
            cout << "Scalars[" << i << "] = " << name;
            cout << endl;
        }

        n = reader->GetNumberOfVectorsInFile();
        //cout << "Number of vectors = " << n << endl;
        for (i = 0; i < n; i++)
        {
            const char *name = reader->GetVectorsNameInFile(i);
            cout << "Vectors[" << i << "] = " << name;
            cout << endl;
        }

        n = reader->GetNumberOfTensorsInFile();
        //cout << "Number of tensors = " << n << endl;
        for (i = 0; i < n; i++)
        {
            const char *name = reader->GetTensorsNameInFile(i);
            cout << "Tensors[" << i << "] = " << name;
            cout << endl;
        }
    }

    reader->Delete();

    return;
}


void list_vts(const char *filename)
{
    vtkXMLStructuredGridReader *reader = vtkXMLStructuredGridReader::New();

    int canReadFile = 0;
    canReadFile = reader->CanReadFile(filename);

    if (!canReadFile)
    {
        cerr << "Could not read " << filename << endl;
        exit(1);
    }

    reader->SetFileName(filename);
    cout << "Reading " << filename << endl;

    reader->Update();
    //reader->PrintSelf(cout, 0);

    vtkStructuredGrid *sgrid = reader->GetOutput();
    vtkDataSet *dataset = static_cast<vtkDataSet*>(sgrid);

    vtkls(dataset);
    
    reader->Delete();
}

// ---------------------------------------------------------------------------
