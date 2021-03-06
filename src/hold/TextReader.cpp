#include "TextReader.h"
#include <cassert>
#include <cstdlib>
#include <cstring>

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
    fp = NULL;

    if (filename != "")
    {
        fp = fopen(filename.c_str(), "r");
    }

    // check for failure
    if (fp == NULL)
    {
        return -1;
    }
    
    return 0;
}

void cigma::TextReader::close()
{
    if (fp != NULL)
    {
        fclose(fp);
    }
}


// ---------------------------------------------------------------------------

static FILE *get_fp(const char *loc, FILE *default_fp)
{
    FILE *fp = default_fp;
    if ((loc != NULL) && (strcmp(loc, "") != 0))
    {
        fp = fopen(loc, "r");
    }
    return fp;
}

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
    FILE *loc_fp = get_fp(loc, fp);
    assert(loc_fp != NULL);
    read_dmat(loc_fp, data, num, dim);
}

void cigma::TextReader::get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs)
{
    // XXX: add support for sections (c.f. gmsh format) so we can scan a single file
    // for now, interpret loc argument to be an entirely new file
    FILE *loc_fp = get_fp(loc, fp);
    assert(loc_fp != NULL);
    read_imat(loc_fp, connectivity, nel, ndofs);
}

void cigma::TextReader::get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd)
{
    // XXX: add support for sections (c.f. gmsh format) so we can scan a single file
    // for now, interpret loc argument to be an entirely new file
    FILE *loc_fp = get_fp(loc, fp);
    assert(loc_fp != NULL);
    read_dmat(loc_fp, coordinates, nno, nsd);
}


// ---------------------------------------------------------------------------
