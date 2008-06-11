#include <iostream>
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkUnstructuredGridWriter.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " input.pvtu output.vtk" << endl;
        return 1;
    }

    vtkXMLPUnstructuredGridReader *pvtu_reader = vtkXMLPUnstructuredGridReader::New();
    pvtu_reader->SetFileName(argv[1]);
    pvtu_reader->Update();
    pvtu_reader->PrintSelf(cout, 0);

    vtkUnstructuredGridWriter *vtk_writer = vtkUnstructuredGridWriter::New();
    vtk_writer->SetInputConnection(pvtu_reader->GetOutputPort());
    vtk_writer->SetFileName(argv[2]);
    vtk_writer->Write();

    return 0;
}
