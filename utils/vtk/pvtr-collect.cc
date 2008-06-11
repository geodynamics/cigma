#include <iostream>
#include <vtkXMLPRectilinearGridReader.h>
#include <vtkXMLRectilinearGridWriter.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " input.pvtr output.vtr" << endl;
        return 1;
    }

    vtkXMLPRectilinearGridReader *pvtr_reader = vtkXMLPRectilinearGridReader::New();
    pvtr_reader->SetFileName(argv[1]);
    pvtr_reader->Update();
    pvtr_reader->PrintSelf(cout, 0);

    vtkXMLRectilinearGridWriter *vtr_writer = vtkXMLRectilinearGridWriter::New();
    vtr_writer->SetInputConnection(pvtr_reader->GetOutputPort());
    vtr_writer->SetFileName(argv[2]);
    vtr_writer->Write();

    return 0;
}
