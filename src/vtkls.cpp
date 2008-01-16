#include <iostream>
#include <cstdlib>
#include <cassert>

#include "AnyOption.h"

#include <vtkDataSetReader.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

#include <vtkDataArray.h>

using namespace std;


typedef enum {
    VTK_FILE_NONE,
    VTK_FILE_POLYDATA,
    VTK_FILE_STRUCTURED_POINTS,
    VTK_FILE_STRUCTURED_GRID,
    VTK_FILE_UNSTRUCTURED_GRID,
    VTK_FILE_RECTILINEAR_GRID
} VtkFileType;

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
}

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
    return "";
}



// ---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    bool verbose = false;
    bool by_class = false;
    bool by_arrays = true;

    int i,n;

    // ------------------------------------------------------
    /*
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " " << "file.vtk" << endl;
        exit(1);
    }
    // */

    // ------------------------------------------------------

    AnyOption *opt = new AnyOption();

    opt->addUsage("Usage: vtkls file.vtk");

    opt->setFlag("help", 'h');
    opt->setFlag("verbose", 'v');
    opt->setFlag('c'); // sort by class
    opt->setFlag('a'); // sort by array

    opt->processCommandArgs(argc, argv);

    if (!opt->hasOptions())
    {
        opt->printUsage();
        delete opt;
        exit(1);
    }

    if (opt->getFlag("help"))
    {
        opt->printUsage();
        delete opt;
        exit(0);
    }

    if (opt->getFlag("verbose"))
    {
        verbose = true;
    }

    if (opt->getFlag('c') && !opt->getFlag('a'))
    {
        by_class = true;
        by_arrays = false;
    }
    if (opt->getFlag('a') && !opt->getFlag('c'))
    {
        by_arrays = true;
        by_class = false;
    }
    int nargs = opt->getArgc();
    if (nargs != 1)
    {
        opt->printUsage();
        delete opt;
        exit(1);
    }
    char *filename = opt->getArgv(0);


    // ------------------------------------------------------
    // 
    // Assuming legacy format
    //
    if (verbose)
    {
        cout << "<< Creating vtkDataSetReader" << endl;
    }
    vtkDataSetReader *reader = vtkDataSetReader::New();
    reader->SetFileName(filename);
    cout << "Reading " << filename << endl;

    int ierr;
    ierr = reader->OpenVTKFile();   // does file exist?
    if (ierr == 0)
    {
        cerr << "vtkls: Could not open " << filename << endl;
        exit(1);
    }

    ierr = reader->ReadHeader();    // is the vtk header present?
    if (ierr == 0)
    {
        cerr << "vtkls: Unrecognized file " << filename << endl;
        exit(1);
    }

    /* // Determine type of file (direct method)
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
    assert(is_valid); // */

    /* // Determine type of file (second method)
    VtkFileType fileType = getFileType(reader);
    const char *fileTypeName = getFileTypeName(fileType);
    cout << "Detected " << fileTypeName << " file" << endl;
    // */

    

    // ------------------------------------------------------

    reader->Update();
    //reader->PrintSelf(cout, 0);

    if (verbose)
    {
        cout << "<< Creating vtkDataSet" << endl;
    }
    vtkDataSet *dataset = reader->GetOutput();
    //dataset->PrintSelf(cout, 0);

    if (verbose)
    {
        cout << "<< Creating vtkPointData" << endl;
    }
    vtkPointData *pointData = dataset->GetPointData();
    //pointData->PrintSelf(cout, 0);

    if (verbose)
    {
        cout << "<< Creating vtkCellData" << endl;
    }
    vtkCellData *cellData = dataset->GetCellData();
    //cellData->PrintSelf(cout, 0);


    // ------------------------------------------------------
    int nno,nel;

    nno = dataset->GetNumberOfPoints();
    if (nno > 0)
    {
        cout << "Points = " << nno << endl;
    }

    nel = dataset->GetNumberOfCells();
    if (nel > 0)
    {
        cout << "Cells = " << nel << endl;
    }


    // ------------------------------------------------------
    int numArrays;
    int numTuples, numComponents;

    if (by_arrays)
    {
        //*
        numArrays = pointData->GetNumberOfArrays();
        if (numArrays > 0)
        {
            //cout << "PointData" << endl;
            //cout << "\tNumber of arrays = " << numArrays << endl;
            for (i = 0; i < numArrays; i++)
            {
                vtkDataArray *dataArray = pointData->GetArray(i);
                const char *name = pointData->GetArrayName(i);
                numTuples = dataArray->GetNumberOfTuples();
                numComponents = dataArray->GetNumberOfComponents();
                cout << "PointDataArray[" << i << "] = " << name;
                //cout << "  " << dataArray->GetClassName();
                cout << "  " << "(" << numTuples << " x " << numComponents << ")";
                cout << endl;
            }
        }

        numArrays = cellData->GetNumberOfArrays();
        if (numArrays > 0)
        {
            //cout << "CellData" << endl;
            //cout << "\tNumber of arrays = " << numArrays << endl;
            for (i = 0; i < numArrays; i++)
            {
                vtkDataArray *dataArray = cellData->GetArray(i);
                const char *name = cellData->GetArrayName(i);
                numTuples = dataArray->GetNumberOfTuples();
                numComponents = dataArray->GetNumberOfComponents();
                cout << "CellDataArray[" << i << "] = " << name;
                //cout << "  " << dataArray->GetClassName();
                cout << "  " << "(" << numTuples << " x " << numComponents << ")";
                cout << endl;
            }
        }
        // */
    }




    // ------------------------------------------------------

    if (by_class)
    {
        //*
        n = reader->GetNumberOfFieldDataInFile();
        //cout << "Number of field data = " << n << endl;
        for (i = 0; i < n; i++)
        {
            const char *name = reader->GetFieldDataNameInFile(i);
            cout << "FieldData[" << i << "] = " << name;
            cout << endl;
        }

        n = reader->GetNumberOfScalarsInFile();
        //cout << "Number of scalars = " << n << endl;
        for (i = 0; i < n; i++)
        {
            const char *name = reader->GetScalarsNameInFile(i);
            vtkDataArray *dataArray = pointData->GetArray(i);
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
        // */
    }



    // ------------------------------------------------------
    // Sleep for a bit so we can examine memory usage with htop
    //cout << "<< Sleeping..." << endl;
    //sleep(100);

    delete opt;
    return 0;
}
