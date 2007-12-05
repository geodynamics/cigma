#include "TextIO.h"
#include <cassert>
#include <cstdlib>

//----------------------------------------------------------------------------

cigma::TextIO::TextIO()
{
    fp = NULL;
}

cigma::TextIO::~TextIO()
{
    close();
}

//----------------------------------------------------------------------------

void cigma::TextIO::open_read(std::string filename)
{
    fp = (FILE *)fopen(filename.c_str(), "r");
}

void cigma::TextIO::open_write(std::string filename)
{
    fp = (FILE *)fopen(filename.c_str(), "w");
}

void cigma::TextIO::close()
{
    if (fp != NULL)
    {
        fclose(fp);
    }
}

//----------------------------------------------------------------------------

static bool read_dmat(FILE *fp, double **mat, int *rows, int *cols)
{
    int ret;

    assert(fp != NULL);

    ret = fscanf(fp, "%d", rows);
    assert(ret == 1);
    assert(*rows > 0);
    
    ret = fscanf(fp, "%d", cols);
    assert(ret == 1);
    assert(*cols > 0);

    *mat = new double[(*rows) * (*cols)];

    for (int i = 0; i < (*rows); i++)
    {
        for (int j = 0; j < (*cols); j++)
        {
            int n = (*rows) * i + j;
            ret = fscanf(fp, "%lf", &((*mat)[n]));
            assert(ret == 1);
        }
    }
    
    return true;
}

static bool write_dmat(FILE *fp, double *mat, int rows, int cols)
{
    assert(fp != NULL);

    fprintf(fp, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fprintf(fp, " %g", mat[rows*i + j]);
        }
        printf("\n");
    }

    return true;
}

static bool write_imat(FILE *fp, int *mat, int rows, int cols)
{
    assert(fp != NULL);
    fprintf(fp, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fprintf(fp, " %d", mat[rows*i + j]);
        }
        fprintf(fp, "\n");
    }

    return true;
}

//----------------------------------------------------------------------------

bool cigma::TextIO::read_connectivity(int **connect, int *nel, int *ndofs)
{
    int ret;

    ret = fscanf(fp, "%d", nel);
    assert(ret == 1);
    assert(*nel > 0);

    ret = fscanf(fp, "%d", ndofs);
    assert(ret == 1);
    assert(*ndofs > 0);

    *connect = new int[(*nel) * (*ndofs)];

    for (int i = 0; i < (*nel); i++)
    {
        for (int j = 0; j < (*ndofs); j++)
        {
            ret = fscanf(fp, "%d", &((*connect)[(*ndofs)*i + j]));
            assert(ret == 1);
        }
    }

    return true;
}

bool cigma::TextIO::read_coordinates(double **coords, int *nno, int *nsd)
{
    return read_dmat(fp, coords, nno, nsd);
}

bool cigma::TextIO::read_dofs(double **dofs, int *nno, int *ndim)
{
    return read_dmat(fp, dofs, nno, ndim);
}

bool cigma::TextIO::read_points(double **points, int *npts, int *ndim)
{
    return read_dmat(fp, points, npts, ndim);
}

//----------------------------------------------------------------------------

void cigma::TextIO::write_header()
{
}

void cigma::TextIO::write_points(double *points, int npts, int ndim)
{
    write_dmat(fp, points, npts, ndim);
}

void cigma::TextIO::write_cells(int *cells, int nel, int ndofs)
{
    write_imat(fp, cells, nel, ndofs);
}

