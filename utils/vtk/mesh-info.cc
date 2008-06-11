// C++ includes
#include <iostream>
#include <cassert>

// Boost includes
#include <boost/filesystem.hpp>

// VTK includes
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataArray.h>
#include <vtkCell.h>
#include <vtkIdList.h>

// ---------------------------------------------------------------------------

using namespace std;
namespace fs = boost::filesystem;


typedef enum {
    VTK, VTU, VTS, VTR, PVTU, PVTS, PVTR
} VtkReaderType;

// ---------------------------------------------------------------------------

double dist(double dx, double dy, double dz)
{
    return sqrt(dx*dx + dy*dy + dz*dz);
}

double BoxSize(double bounds[6])
{
    const double x0 = bounds[0];
    const double x1 = bounds[1];
    const double y0 = bounds[2];
    const double y1 = bounds[3];
    const double z0 = bounds[4];
    const double z1 = bounds[5];
    return dist(x1-x0, y1-y0, z1-z0);
}

void PrintSize(vtkAlgorithm *algorithm, VtkReaderType readerType)
{
    vtkDataSet *dataset = 0;
    vtkUnstructuredGridReader *vtk_ug_reader = 0;
    vtkXMLPUnstructuredGridReader *pvtu_reader = 0;

    algorithm->Update();

    switch (readerType)
    {
    case VTK:
        vtk_ug_reader = static_cast<vtkUnstructuredGridReader*>(algorithm);
        dataset = vtk_ug_reader->GetOutput();
        break;
    case PVTU:
        pvtu_reader = static_cast<vtkXMLPUnstructuredGridReader*>(algorithm);
        dataset = pvtu_reader->GetOutput();
        break;
    default:
        cerr << "Unsupported reader" << endl;
        assert(false);
        break;
    }

    if (false)
    {
        cout << "Grid information" << endl;
        dataset->Update();
        dataset->PrintSelf(cout, 4);
    }

    int n_points, n_cells;
    int e, emax;
    double h, hmax;

    hmax = 0.0;
    emax = -1;
    n_cells = dataset->GetNumberOfCells();
    n_points = dataset->GetNumberOfPoints();
    for (e = 1; e <= n_cells; e++)
    {
        double bounds[6];
        dataset->GetCellBounds(e, bounds);
        h = BoxSize(bounds);
        if (h > hmax)
        {
            hmax = h;
            emax = e;
        }
    }
    cout << "n = " << n_points << endl;
    cout << "e = " << n_cells << endl;
    cout << "h = " << hmax << endl;
}

// ---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " file.vtk" << endl;
        return 1;
    }

    string ext = fs::extension(argv[1]);
    cout << "Reading " << argv[1] << endl;

    if (ext == ".vtk")
    {
        vtkUnstructuredGridReader *reader = vtkUnstructuredGridReader::New();
        reader->SetFileName(argv[1]);
        PrintSize(reader, VTK);
    }
    else if (ext == ".pvtu")
    {
        vtkXMLPUnstructuredGridReader *reader = vtkXMLPUnstructuredGridReader::New();
        reader->SetFileName(argv[1]);
        PrintSize(reader, PVTU);
    }
    else
    {
        cerr << "Unsupported file extension: \"" << ext << "\"\n";
        return 1;
    }

    return 0;
}
