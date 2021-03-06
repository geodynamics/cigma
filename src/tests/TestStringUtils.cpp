#include <iostream>
#include <iomanip>
#include <vector>
#include "../StringUtils.h"

using namespace std;

int main()
{
    // conversion functions
    {
        int i;
        double e;
        string_to_int("123456", i);
        string_to_double("2.718281828459045", e);

        cout << setprecision(16);
        cout << "i = " << i << endl;
        cout << "e = " << e << endl;
        cout << endl;
    }

    // path functions
    {
        string vtkpath = "/path/to/file.vtk:temperature";
        string hdfpath = "/path/to/file.h5:/path/to/dataset";
        string testpath0 = ".:/path/to/file.ext";
        string testpath1 = "/path/to/file.ext";
        string testpath2 = "/path/to/file.ext:";
        string testpath3 = "/path/to/file.ext:foo";
        string testpath4 = "/path/to/file.ext:foo:";
        string testpath5 = "/path/to/file.ext:foo:bar";

        vector<string> paths;
        paths.push_back(vtkpath);
        paths.push_back(hdfpath);
        paths.push_back(testpath0);
        paths.push_back(testpath1);
        paths.push_back(testpath2);
        paths.push_back(testpath3);
        paths.push_back(testpath4);
        paths.push_back(testpath5);

        string path;
        string location, filename, ext;
        string locdir, locbase;

        vector<string>::iterator i;
        for (i = paths.begin(); i != paths.end(); ++i)
        {
            path = *i;

            parse_dataset_path(path, location, filename, ext);

            cout << "path      = '" << path << "'\n";
            cout << "location  = '" << location << "'\n";

            path_dirname(location, locdir);
            path_basename(location, locbase);

            cout << "location dirname  = '" << locdir << "'\n";
            cout << "location basename = '" << locbase << "'\n";

            cout << "filename  = '" << filename << "'\n";
            cout << "extension = '" << ext << "'\n";
            cout << endl;
        }
    }

    return 0;
}
