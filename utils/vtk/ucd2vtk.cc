#include <iostream>
#include <vtkAVSucdReader.h>
#include <vtkUnstructuredGridWriter.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " input.ucd output.vtk" << endl;
        return 1;
    }

    vtkAVSucdReader *ucd_reader = vtkAVSucdReader::New();
    ucd_reader->SetFileName(argv[1]);
    ucd_reader->Update();
    ucd_reader->PrintSelf(cout, 0);

    vtkUnstructuredGridWriter *ug_writer = vtkUnstructuredGridWriter::New();
    ug_writer->SetInputConnection(ucd_reader->GetOutputPort());
    ug_writer->SetFileName(argv[2]);
    ug_writer->Write();

    return 0;
}
