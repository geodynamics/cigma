#include "Writers.hpp"
#include <vector>
#include <stdio.h>


using namespace gtb;

int gtb::residuals_write_off(const char *filename,
                             const std::vector<Point_3f> &vertices,
                             const std::vector<Tetrahedron> &tets,
                             const std::vector<float> &residuals)
{
    FILE *fp;
    GTB_FOPEN(fp, filename, "w");
    
    fprintf(fp, "%d %d\n", vertices.size(), tets.size());

    for (unsigned i = 0; i < vertices.size(); i++)
    {
        fprintf(fp, "%f %f %f\n", vertices[i][0], vertices[i][1], vertices[i][2]);
    }

    for (unsigned ti = 0; ti < tets.size(); ti++)
    {
        const Tetrahedron &t = tets[ti];
        fprintf(fp, "%d %d %d %d %.10f\n",
                t.get_vertex(0),
                t.get_vertex(1),
                t.get_vertex(2),
                t.get_vertex(3),
                residuals[ti]);
    }

    fclose(fp);

    return 0;
}


int gtb::residuals_write_vtk(const char *filename,
                             const std::vector<Point_3f> &vertices,
                             const std::vector<Tetrahedron> &tets,
                             const std::vector<float> &residuals)
{
    const int ndof = 4;
    int nno = vertices.size();
    int nel = tets.size();

    FILE *fp;
    GTB_FOPEN(fp, filename, "w");

    fprintf(fp, "# vtk DataFile Version 3.0\n");
    fprintf(fp, "This line is a comment\n");
    fprintf(fp, "ASCII\n");

    fprintf(fp, "DATASET UNSTRUCTURED_GRID\n");

    fprintf(fp, "POINTS %d float\n", nno);
    for (int n = 0; n < nno; n++)
    {
        fprintf(fp, "%g %g %g\n",
                vertices[n][0],
                vertices[n][1],
                vertices[n][2]);
    }

    fprintf(fp, "CELLS %d %d\n", nel, (nel + nel*ndof));
    for (int e = 0; e < nel; e++)
    {
        const Tetrahedron &t = tets[e];
        fprintf(fp,
                "%d "
                "%d %d %d %d\n",
                ndof,
                t.get_vertex(0),
                t.get_vertex(1),
                t.get_vertex(2),
                t.get_vertex(3));
    }

    fprintf(fp, "CELL_TYPES %d\n", nel);
    for (int e = 0; e < nel; e++)
    {
        fprintf(fp, "10\n");
    }

    fprintf(fp, "CELL_DATA %d\n", nel);
    fprintf(fp, "SCALARS LocalResiduals float\n");
    fprintf(fp, "LOOKUP_TABLE default\n");
    for (int e = 0; e < nel; e++)
    {
        fprintf(fp, "%g\n", residuals[e]);
    }

    fclose(fp);
    return 0;
}
