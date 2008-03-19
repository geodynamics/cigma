#include <stdio.h>
#include <stdlib.h>
#include "txtarray.h"


int txtarray_read1(const char *filename,
                   double **data, int *m, int *n)
{
    FILE *fp;
    char linebuf[500]; // 500 > 14 * (3 + 3 + 9 + 6)

    int i,j;
    double *x;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return -1;
    }

    if (fscanf(fp, "# %d %d\n", m, n) != 2)
    {
        return -2;
    }

    fgets(linebuf, 500, fp); // read header line

    *data = (double *)malloc(((*m)*(*n))*sizeof(double));

    for (i = 0; i < *m; i++)
    {
        x = &(*data)[(*n) * i];
        for (j = 0; j < *n; j++)
        {
            fscanf(fp, "%lf", &x[j]);
        }
    }
    
    fclose(fp);

    return 0;
}


int txtarray_read_stations(const char *filename,
                           double **stations,
                           int *nstations)
{
    int three;
    int ret;

    ret = txtarray_read1(filename, stations, nstations, &three);

    if (three != 3)
    {
        return -1;
    }

    return ret;
}


/*
 * TODO: can this family of functions be written using varargs?
 *
 *  txtarray_write(const char *filename,
 *                 const char *header,
 *                 int m,
 *                 double *data1, int n1,
 *                 double *data2, int n2,
 *                 ...);
 *
 */

int txtarray_write1(const char *filename, const char *header,
                    double *data, int m, int n)
{
    FILE *fp;
    int i,j;
    double *x;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        return -1;
    }

    fprintf(fp, "# %d %d\n", m, n);
    
    if (header != NULL)
        fprintf(fp, "# %s\n", header);
    else
        fprintf(fp, "# header\n");

    for (i = 0; i < m; i++)
    {
        x = &data[n*i];
        for (j = 0; j < n; j++)
        {
            fprintf(fp, " %0.12e", x[j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

    return 0;
}

int txtarray_write2(const char *filename,
                    const char *header,
                    int m,
                    double *data1, int n1,
                    double *data2, int n2)
{
    FILE *fp;
    int i,j;
    double *x;
    double *y;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        return -1;
    }

    fprintf(fp, "# %d %d %d\n", m, n1, n2);
    
    if (header != NULL)
        fprintf(fp, "# %s\n", header);
    else
        fprintf(fp, "# header\n");

    for (i = 0; i < m; i++)
    {
        x = &data1[n1 * i];
        for (j = 0; j < n1; j++)
        {
            fprintf(fp, " %0.12e", x[j]);
        }

        y = &data2[n2 * i];
        for (j = 0; j < n2; j++)
        {
            fprintf(fp, " %0.12e", y[j]);
        }
        
        fprintf(fp, "\n");
    }
    fclose(fp);
    
    return 0;
}

int txtarray_write4(const char *filename,
                    const char *header,
                    int m,
                    double *data1, int n1,
                    double *data2, int n2,
                    double *data3, int n3,
                    double *data4, int n4)
{
    FILE *fp;
    int i,j;
    double *x, *y, *z, *w;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        return -1;
    }

    fprintf(fp, "# %d %d %d %d %d\n", m, n1, n2, n3, n4);

    if (header != NULL)
        fprintf(fp, "# %s\n", header);
    else
        fprintf(fp, "# header\n");

    for (i = 0; i < m; i++)
    {
        x = &data1[n1 * i];
        for (j = 0; j < n1; j++)
        {
            fprintf(fp, " %0.12e", x[j]);
        }

        y = &data2[n2 * i];
        for (j = 0; j < n2; j++)
        {
            fprintf(fp, " %0.12e", y[j]);
        }
        
        z = &data3[n3 * i];
        for (j = 0; j < n3; j++)
        {
            fprintf(fp, " %0.12e", z[j]);
        }
        
        w = &data4[n4 * i];
        for (j = 0; j < n4; j++)
        {
            fprintf(fp, " %0.12e", w[j]);
        }
        
        fprintf(fp, "\n");
    }
    fclose(fp);

    return 0;
}

int txtarray_write_disp(const char *filename,
                        const char *header,
                        int nstations,
                        double *stations,
                        double *displacements)
{
    return txtarray_write2(filename, header, nstations,
                           stations, 3, displacements, 3);
}

int txtarray_write_deriv(const char *filename,
                         const char *header,
                         int nstations,
                         double *stations,
                         double *derivatives)
{
    return txtarray_write2(filename, header, nstations,
                           stations, 3, derivatives, 9);
}

int txtarray_write_stress(const char *filename,
                          const char *header,
                          int nstations,
                          double *stations,
                          double *stress)
{
    return txtarray_write2(filename, header, nstations,
                           stations, 3, stress, 6);
}

int txtarray_write_all(const char *filename,
                       const char *header,
                       int nstations,
                       double *stations,
                       double *displacements,
                       double *derivatives,
                       double *stress)
{
    return txtarray_write4(filename, header,
                           nstations,
                           stations, 3,
                           displacements, 3,
                           derivatives, 9,
                           stress, 6);
}

