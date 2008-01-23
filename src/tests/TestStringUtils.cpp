#include <iostream>
#include <iomanip>
#include "../StringUtils.h"

int main()
{
    // conversion functions
    {
        int i;
        double e;
        string_to_int("123456", i);
        string_to_double("2.718281828459045", e);

        std::cout << std::setprecision(16);
        std::cout << "i = " << i << std::endl;
        std::cout << "e = " << e << std::endl;
    }

    // path functions
    {
        using namespace std;
        std::string vtkpath = "/path/to/file.vtk:temperature";
        std::string hdfpath = "/path/to/file.h5:/path/to/dataset";
        std::string testpath = "/path/to/some/file.ext:foo:bar:baz";

        std::string path, location, filename, ext;

        path = testpath;
        parse_dataset_path(path, location, filename, ext);
        std::cout << "path      = '" << path << "'\n";
        std::cout << "filename  = '" << filename << "'\n";
        std::cout << "extension = '" << ext << "'\n";
        std::cout << "location  = '" << location << "'\n";

        string locdir, locbase;
        path_dirname(location, locdir);
        path_basename(location, locbase);
        cout << "location dirname  = '" << locdir << "'\n";
        cout << "location basename = '" << locbase << "'\n";
    }

    return 0;
}
