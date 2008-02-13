#include "TextReader.h"
#include <cassert>
#include <cstdlib>

// ---------------------------------------------------------------------------
cigma::TextReader::TextReader()
{
    fp = NULL;
}

cigma::TextReader::~TextReader()
{
    close();
}

// ---------------------------------------------------------------------------
int cigma::TextReader::open(std::string filename)
{
    fp = fopen(filename.c_str(), "r");
    assert(fp != NULL);
    return 0; // XXX: change return value instead of using assert
}

void cigma::TextReader::close()
{
    if (fp != NULL)
    {
        fclose(fp);
    }
}


// ---------------------------------------------------------------------------

static bool read_dmat(FILE *fp, double **mat, int *rows, int *cols)
{
    assert(fp != NULL);

    int nrows, ncols;
    int ret;

    ret = fscanf(fp, "%d", &nrows);
    assert(ret == 1);
    assert(nrows > 0);

    ret = fscanf(fp, "%d", &ncols);
    assert(ret == 1);
    assert(ncols > 0);

    double *m = new double[nrows * ncols];

    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            int n = ncols*i + j;
            ret = fscanf(fp, "%lf", &m[n]);
            assert(ret == 1);
        }
    }

    *mat = m;
    *rows = nrows;
    *cols = ncols;

    return true;
}

static bool read_imat(FILE *fp, int **mat, int *rows, int *cols)
{
    assert(fp != NULL);

    int nrows, ncols;
    int ret;

    ret = fscanf(fp, "%d", &nrows);
    assert(ret == 1);
    assert(nrows > 0);

    ret = fscanf(fp, "%d", &ncols);
    assert(ret == 1);
    assert(ncols > 0);

    int *m = new int[nrows * ncols];

    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            int n = ncols*i + j;
            ret = fscanf(fp, "%d", &m[n]);
            assert(ret == 1);
        }
    }

    *mat = m;
    *rows = nrows;
    *cols = ncols;

    return true;
}

// ---------------------------------------------------------------------------

void cigma::TextReader::get_dataset(const char *loc, double **data, int *num, int *dim)
{
    read_dmat(fp, data, num, dim);
}

void cigma::TextReader::get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs)
{
    read_imat(fp, connectivity, nel, ndofs);
}

void cigma::TextReader::get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd)
{
    read_dmat(fp, coordinates, nno, nsd);
}


// ---------------------------------------------------------------------------
