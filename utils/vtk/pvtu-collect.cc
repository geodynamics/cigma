#include <iostream>
#include <vtkXMLPUnstructuredGridReader.h>
#include <vtkXMLUnstructuredGridWriter.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " input.pvtu output.vtu" << endl;
        return 1;
    }

    vtkXMLPUnstructuredGridReader *pvtu_reader = vtkXMLPUnstructuredGridReader::New();
    pvtu_reader->SetFileName(argv[1]);
    pvtu_reader->Update();
    pvtu_reader->PrintSelf(cout, 0);

    vtkXMLUnstructuredGridWriter *vtu_writer = vtkXMLUnstructuredGridWriter::New();
    vtu_writer->SetInputConnection(pvtu_reader->GetOutputPort());
    vtu_writer->SetFileName(argv[2]);
    vtu_writer->Write();

    return 0;
}
