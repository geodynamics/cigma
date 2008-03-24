#include <cassert>
#include <cstdlib>

#include "TextWriter.h"


using namespace cigma;


// ---------------------------------------------------------------------------

TextWriter::TextWriter()
{
    fp = NULL;
}

TextWriter::~TextWriter()
{
    close();
}


// ---------------------------------------------------------------------------

int TextWriter::open(const char *filename)
{
    if (filename != NULL)
    {
        fp = fopen(filename, "w");
    }
    else
    {
        fp = stdout;
    }

    if (fp == NULL)
    {
        return -1;
    }

    return 0;
}

int TextWriter::close()
{
    if (fp != NULL)
    {
        fclose(fp);
        fp = NULL;
    }
    return 0;
}


// ---------------------------------------------------------------------------

static bool write_dmat(FILE *fp, double *mat, int rows, int cols)
{
    assert(fp != NULL);
    assert(rows > 0);
    assert(cols > 0);

    fprintf(fp, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fprintf(fp, " %g", mat[cols*i + j]);
        }
        fprintf(fp, "\n");
    }

    return true;
}

static bool write_imat(FILE *fp, int *mat, int rows, int cols)
{
    assert(fp != NULL);
    assert(rows > 0);
    assert(cols > 0);

    fprintf(fp, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fprintf(fp, " %d", mat[cols*i + j]);
        }
        fprintf(fp, "\n");
    }

    return true;
}


// ---------------------------------------------------------------------------

int TextWriter::write_connectivity(const char *loc, int *connectivity, int nel, int ndofs)
{
    write_imat(fp, connectivity, nel, ndofs);
}

int TextWriter::write_coordinates(const char *loc, double *coordinates, int nno, int nsd)
{
    write_dmat(fp, coordinates, nno, nsd);
}

int TextWriter::write_dataset(const char *loc, double *data, int num, int ndim)
{
    write_dmat(fp, data, num, ndim);
}


// ---------------------------------------------------------------------------
