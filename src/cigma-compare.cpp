#include <stdio.h>
#include <iostream>
#include <vector>
#include "anyoption.h"
#include "../libcigma/dataset.h"
#include "HDF5_Reader.hpp"
#include "Indexed_tetra_set.hpp"
#include "Time_series.hpp"
#include "StringUtils.hpp"
#include "kdtree_tetrahedra.hpp"
#include "vect_stats.hpp"
#include "tet_sampling.hpp"
#include "Writers.hpp"

using namespace gtb;
using namespace std;

int compare_main(char *first, char *second, char *output);

void compare_scalar_fields(Indexed_tetra_set *its1,
                           Indexed_tetra_set *its2,
                           Scalar_time_series *sts1,
                           Scalar_time_series *sts2,
                           std::vector<float> &residuals);

void compare_vector_fields(Indexed_tetra_set *its1,
                           Indexed_tetra_set *its2,
                           Vector_time_series *vts1,
                           Vector_time_series *vts2,
                           std::vector<float> &residuals);



void print_usage(void)
{
    cerr << "Usage: cigma-compare --first=field.h5:/model/variables/displacement/step0" << endl
         << "                     --second=field2.h5:/model/variables/displacement/step0" << endl
         << "                     --output=residuals.vtk" << endl;
}

void print_mesh_info(Indexed_tetra_set *its)
{
    printf("num tetrahedra: %d\n", its->get_num_tetrahedra());
}



int main(int argc, char **argv)
{
    AnyOption *opt = new AnyOption();

    opt->setFlag("help", 'h');
    opt->setOption("first");
    opt->setOption("second");
    opt->setOption("output", 'o');

    opt->processCommandArgs(argc, argv);

    if (!opt->hasOptions())
    {
        print_usage();
        delete opt;
        return 0;
    }

    if (opt->getFlag("help") || opt->getFlag('h'))
    {
        print_usage();
        delete opt;
        return 0;
    }

    if (opt->getValue("first") != NULL)
    {
        cout << "first  = \"" << opt->getValue("first") << "\"" << endl;
    }
    else
    {
        cout << "Error: Please specify the first field with --first" << endl;
        delete opt;
        return -1;
    }


    if (opt->getValue("second") != NULL)
    {
        cout << "second = \"" << opt->getValue("second") << "\"" << endl;
    }
    else
    {
        cout << "Error: Please specify the second field with --second" << endl;
        delete opt;
        return -2;
    }

    if (opt->getValue("output") != NULL)
    {
        cout << "output = \"" << opt->getValue("output") << "\"" << endl;
    }
    else
    {
        cout << "Error: Please specify an output vtk file with --output" << endl;
        delete opt;
        return -3;
    }

    char *first  = opt->getValue("first");
    char *second = opt->getValue("second");
    char *output = opt->getValue("output");
    int exitcode;

    exitcode = compare_main(first, second, output);

    delete opt;
    return exitcode;
}



int compare_main(char *first, char *second, char *output)
{
    HDF5_Reader reader;
    Indexed_tetra_set *its1, *its2;
    Scalar_time_series *sts1, *sts2;
    Vector_time_series *vts1, *vts2;

    std::vector<string> path1, path2;

    SplitString(first, ":", path1, false);
    if (path1.size() != 2)
    {
        cerr << "Error: Please specify an argument of the form "
             << "--first 'file1.h5:/path/to/field'" << endl;
        return -1;
    }

    SplitString(second, ":", path2, false);
    if (path2.size() != 2)
    {
        cerr << "Error: Please specify an argument of the form "
             << "--second 'file2.h5:/path/to/field'" << endl;
        return -2;
    }

    const char *firstfile = path1[0].c_str();
    const char *first_loc = path1[1].c_str();

    const char *secondfile = path2[0].c_str();
    const char *second_loc = path2[1].c_str();

    int ierr;

    ierr = reader.get_time_series(firstfile, first_loc, &sts1, &vts1, &its1);
    if (ierr < 0)
    {
        return -3;
    }
    print_mesh_info(its1);

    ierr = reader.get_time_series(secondfile, second_loc, &sts2, &vts2, &its2);
    if (ierr < 0)
    {
        return -4;
    }
    print_mesh_info(its2);
    
    if (((sts1 != NULL) && (vts2 != NULL)) ||
        ((sts2 != NULL) && (vts1 != NULL)))
    {
        cerr << "Error: Incompatible fields: "
             << "Cannot compare scalar and vector fields "
             << endl;
        return -5;
    }

    std::vector<float> residuals;
    int nel = its1->get_num_tetrahedra();

    residuals.resize(nel);

    if ((sts1 != NULL) && (sts2 != NULL))
    {
        compare_scalar_fields(its1, its2, sts1, sts2, residuals);
    }
    else if ((vts1 != NULL) && (vts2 != NULL))
    {
        compare_vector_fields(its1, its2, vts1, vts2, residuals);
    }
    else 
    {
        cerr << "Error: Failed to initialize one of the fields" << endl;
        return -6;
    }

    ierr = residuals_write_vtk(output,
                               its1->get_vertices(),
                               its1->get_tetrahedra(),
                               residuals);
    if (ierr < 0)
    {
        cerr << "Error: Could not write to " << output << endl;
        return -7;
    }

    
    return 0;
}


void compare_scalar_fields(Indexed_tetra_set *its1,
                           Indexed_tetra_set *its2,
                           Scalar_time_series *sts1,
                           Scalar_time_series *sts2,
                           std::vector<float> &residuals)
{
    printf("Not supported yet!");
}

void compare_vector_fields(Indexed_tetra_set *its1,
                           Indexed_tetra_set *its2,
                           Vector_time_series *vts1,
                           Vector_time_series *vts2,
                           std::vector<float> &residuals)
{
    float max_tet_per_leaf = 1000;
    unsigned num_samples_per_tet = 1;

    printf("\n%s\n", "Building kdtree...");

    KDTet<unsigned> tree(max_tet_per_leaf, its2, vts2);
    tree.MakeTree();

    printf("%s\r", "Sampling inside the elements...");

    const std::vector<Point_3f> &vertices = its1->get_vertices();
    const std::vector<Tetrahedron> &tets = its1->get_tetrahedra();

    unsigned num_eval_misses = 0;
    std::vector<float> err(num_samples_per_tet);

    std::vector<float> errors;

    err.resize(num_samples_per_tet);

    for (unsigned ti = 0; ti < tets.size(); ti++)
    {
        const Tetrahedron &t = tets[ti];
        Vector_3f v0(vertices[t.get_vertex(0)]);
        Vector_3f v1(vertices[t.get_vertex(1)]);
        Vector_3f v2(vertices[t.get_vertex(2)]);
        Vector_3f v3(vertices[t.get_vertex(3)]);

        for (unsigned i = 0; i < num_samples_per_tet; i++)
        {
            Vector_3f pick = Pick(v0, v1, v2, v3);
            Point_3f sf(pick[0], pick[1], pick[2]);
            Point_3d sd(sf[0], sf[1], sf[2]);

            KDTet<unsigned>::TreeNode *parent;
            KDTet<unsigned>::LeafNode *node = tree.Find(sd, parent);

            Vector_3f val2;
            if (!tree.FindAndEvalTet(node, sf, &val2))
            {
                num_eval_misses++;
                continue;
            }

            Vector_3f val1;
            if (!its1->interpol_tetra(vts1, t, sf, &val1))
            {
                num_eval_misses++;
                continue;
            }

            Vector_3f point_residual = val1 - val2;
            err[i] = point_residual.squared_length();
            errors.push_back(err[i]);
        }
        residuals[ti] = vect_mean(err);

        if (ti % 1000 == 0)
        {
            printf("\r%s %.0f%%",
                   "Sampling inside the elements...",
                   (float)ti/tets.size() * 100);
            fflush(stdout);
        }
    }

    printf("\r%s             \n",
           "Sampling inside the elements...");


    //printf("tets.size() = %d\n", tets.size());
    //printf("residuals.size() = %d\n", residuals.size());

    printf("\nSampling:\n");
    printf("num samples per tet: \t%d\n", num_samples_per_tet);
    printf("total num samples  : \t%d\n", (int)errors.size());
    printf("ignored samples    : \t%d (%f%%)\n", num_eval_misses,
                                                 (float)num_eval_misses*100/vertices.size());

    float max_err = vect_max(errors);
    float mean_err = vect_mean(errors);
    float std_dev = vect_std_dev(errors, mean_err);
    float rms_err = sqrt(vect_mean_squared(errors));

    printf("\nField Errors:\n");
    printf("max:             \t%e\n", max_err);
    printf("mean:            \t%e\n", mean_err);
    printf("std dev:         \t%e\n", std_dev);
    printf("rms:             \t%e\n", rms_err);

    float max_res = vect_max(residuals);
    float mean_res = vect_mean(residuals);
    float std_res = vect_std_dev(residuals, mean_res);
    float rms_res = sqrt(vect_mean_squared(residuals));
    printf("\nResiduals:\n");
    printf("max:             \t%e\n", max_res);
    printf("mean:            \t%e\n", mean_res);
    printf("std dev:         \t%e\n", std_res);
    printf("rms:             \t%e\n", rms_res);

}
