#include "TextWriter.h"
#include <cassert>
#include <cstdlib>

// ---------------------------------------------------------------------------
cigma::TextWriter::TextWriter()
{
    fp = NULL;
}

cigma::TextWriter::~TextWriter()
{
    close();
}

// ---------------------------------------------------------------------------

int cigma::TextWriter::open(std::string filename)
{
    fp = NULL;

    if (filename != "")
    {
        fp = fopen(filename.c_str(), "w");
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

void cigma::TextWriter::close()
{
    if (fp != NULL)
    {
        fclose(fp);
    }
}


// ---------------------------------------------------------------------------

void cigma::TextWriter::
write_field(FE_Field *field)
{
    assert(field != 0);
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

void cigma::TextWriter::write_connectivity(int *connectivity, int nel, int ndofs)
{
    write_imat(fp, connectivity, nel, ndofs);
}

void cigma::TextWriter::write_coordinates(double *coordinates, int nno, int nsd)
{
    write_dmat(fp, coordinates, nno, nsd);
}

void cigma::TextWriter::write_dofs(double *dofs, int num, int ndim)
{
    write_dmat(fp, dofs, num, ndim);
}

// ---------------------------------------------------------------------------
