#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "cervelli.h"
#include "txtarray.h"
#include "h5array.h"


static void linspace(double **x, double a, double b, int n)
{
    int i;
    double dx = (b - a)/n;
    double *y;

    y = (double *)malloc(n * sizeof(double));

    for (i = 0; i < n; i++)
    {
        y[i] = a + i*dx;
    }
    
    *x = y;
}


void strike_slip_ng(double *fault_info, int nfaults)
{
    int i;
    double *n;
    double taperd;

    double *fi;
    double fx1, fy1;
    double fx2, fy2;
    double dip, D, Pr, bd;
    double ss, ds, ts;

    //
    // define number of faults to taper with
    //
    taperd = 10.0;  // (16000.0 - 12000.0)/400
    linspace(&n, 12000.0, 16000.0, nfaults);

    for (i = 0; i < nfaults; i++)
    {
        fi = &fault_info[11*i];

        fx1 = 12000.0;
        fy1 = -n[i];

        fx2 = 12000.0;
        fy2 = n[i];

        dip = 90.0;
        D   = n[i];
        Pr  = 0.25;
        bd  = 0;
        ss  = -1.0/nfaults;
        ds  = 0;
        ts  = 0;

        fi[0]  = fx1;
        fi[1]  = fy1;
        fi[2]  = fx2;
        fi[3]  = fy2;
        fi[4]  = dip;
        fi[5]  = D;
        fi[6]  = Pr;
        fi[7]  = bd;
        fi[8]  = ss;
        fi[9]  = ds;
        fi[10] = ts;
    }

    free(n);
}

void reverse_slip_ng(double *fault_info, int nfaults)
{
    int i;
    double *n;
    double taperd;

    double *fi;
    double fx1, fy1;
    double fx2, fy2;
    double dip, D, Pr, bd;
    double ss, ds, ts;

    //
    // define number of faults to taper with
    //
    taperd = 10.0;  // (16000.0 - 12000.0)/400
    linspace(&n, 12000.0, 16000.0, nfaults);

    for (i = 0; i < nfaults; i++)
    {
        fi = &fault_info[11*i];

        fx1 = 4000.0;
        fy1 = -n[i];

        fx2 = 4000.0;
        fy2 = n[i];

        dip = 90.0;
        D   = n[i];
        Pr  = 0.25;
        bd  = 0;
        ss  = 0;
        ds  = 1.0/nfaults;
        ts  = 0;

        fi[0]  = fx1;
        fi[1]  = fy1;
        fi[2]  = fx2;
        fi[3]  = fy2;
        fi[4]  = dip;
        fi[5]  = D;
        fi[6]  = Pr;
        fi[7]  = bd;
        fi[8]  = ss;
        fi[9]  = ds;
        fi[10] = ts;
    }

    free(n);
}


/*
 * Note: Be careful with flag2. For a model with
 * 400 subfaults and 918980 stations, this function would try
 * to allocate
 *
 *  (918980*(3+3+9+6+1+400) + 400*(10+11)) * 8 bytes = 2958.8 MiB
 *
 * disabling flag2 gives us
 *
 *  (918980*(3+3+9+6+1) + 400*(10+11)) * 8 bytes = 154.3 MiB
 *  
 */
void okada_alloc(int nmod, int nstat,
                 double **fault_info,
                 double **models,
                 double **stations,
                 double **displacement,
                 double **derivatives,
                 double **stress,
                 double **flag,
                 double **flag2)
{
    if (fault_info != NULL)
        *fault_info = (double *)malloc((nmod * 11) * sizeof(double));

    if (models != NULL)
        *models = (double *)malloc((nmod * 10) * sizeof(double));

    if (stations != NULL)
        *stations = (double *)malloc((nstat * 3) * sizeof(double));

    if (displacement != NULL)
        *displacement = (double *)malloc((nstat * 3) * sizeof(double));

    if (derivatives != NULL)
        *derivatives = (double *)malloc((nstat * 9) * sizeof(double));

    if (stress != NULL)
        *stress = (double *)malloc((nstat * 6) * sizeof(double));

    if (flag != NULL)
        *flag = (double *)malloc((nstat * 1) * sizeof(double));

    if (flag2 != NULL)
        *flag2 = (double *)malloc((nstat * nmod) * sizeof(double));
}

void okada_free(double *fault_info, double *models, double *stations,
                double *displacement, double *derivatives, double *stress,
                double *flag, double *flag2)
{
    if (fault_info   != NULL) free(fault_info);
    if (models       != NULL) free(models);
    if (stations     != NULL) free(stations);
    if (displacement != NULL) free(displacement);
    if (derivatives  != NULL) free(derivatives);
    if (stress       != NULL) free(stress);
    if (flag         != NULL) free(flag);
    if (flag2        != NULL) free(flag2);
}



int txt_main(char *infile, char *outfile)
{
    int nsubfaults;
    double *subfaults;
    double *models;

    int nstations;
    double *stations;
    double *displacement;
    double *derivatives;
    double *stress;

    double *flag;
    //double *flag2;


    // elastic parameters;
    double mu = 0.25;
    double nu = 0.25;



    /* determine number of subfaults */
    nsubfaults = 400;


    /* read station coordinates */
    txtarray_read_stations(infile, &stations, &nstations);
    
    printf("Found %d stations\n", nstations);

    /*
     * Allocate memory. Skip stations array, since it has
     * already been allocated above. Also, do not allocate
     * flag2 array, since it is typically too large.
     */
    okada_alloc(nsubfaults, nstations,
                &subfaults, &models,
                NULL, &displacement, &derivatives, &stress,
                &flag, NULL);


    /*
     * Pick one of the following models
     */
    strike_slip_ng(subfaults, nsubfaults);
    //reverse_slip_ng(subfaults, nsubfaults);


    /*
     * Perform the actual calculation
     */
    calc_disp_cervelli(mu, nu,
                       models, subfaults, nsubfaults,
                       stations, nstations,
                       displacement, derivatives, stress,
                       flag, NULL);
    

    /*
     * Write all data to a text file
     */
    txtarray_write_all(outfile,
                       "sx sy sz "
                       "ux uy uz "
                       "uxx uxy uxz uyx uyy uyz uzx uzy uzz "
                       "sxx syy szz sxy sxz syz",
                       nstations,
                       stations,
                       displacement,
                       derivatives,
                       stress);

    
    /* free the allocated memory */
    okada_free(subfaults, models, stations,
               displacement, derivatives, stress,
               flag, NULL);

    return 0;
}



int split_filepath(const char *filepath, char *filename, char *path, int maxlen)
{
    char *x, *y;
    int i;

    y = strchr(filepath, ':');

    if (y == NULL)
    {
        strncpy(filename, filepath, maxlen);
        path[0] = '\0';
        return 1;
    }

    for (i = 0, x = (char *)filepath; (x != y) && (i < maxlen); i++, x++)
    {
        filename[i] = *x;
    }
    filename[i] = '\0';

    for(i = 0, x = y+1; (*x != '\0') && (i < maxlen); i++, x++)
    {
        path[i] = *x;
    }
    path[i] = '\0';

    return 0;
}

int h5_main(char *filepath1, char *filepath2)
{
    hid_t file_id;
    //hid_t group_id;
    herr_t status;

    char infile[150];
    char outfile[150];
    char stations_path[150];
    char output_group[150];

    int nsubfaults;
    double *subfaults;
    double *models;

    int nstations;
    double *stations;
    double *displacement;
    double *derivatives;
    double *stress;

    double *flag;
    //double flag2;

    int components;

    // elastic parameters
    double mu = 0.25;
    double nu = 0.25;


    /* determine number of subfaults */
    nsubfaults = 400;


    /*
     * Process filepaths
     */

    split_filepath(filepath1, infile, stations_path, 150);
    if ('\0' == stations_path[0])
    {
        fprintf(stderr, "Error: Could not split file and path from \"%s\"\n", filepath1);
        return -1;
    }

    split_filepath(filepath2, outfile, output_group, 150);
    if (output_group[0] != '\0')
    {
        fprintf(stderr, "Error: Writing to an output group currently not supported\n");
        return -2;
    }


    /*
     * Read station coordinates
     */


    file_id = H5Fopen(infile, H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0)
    {
        fprintf(stderr, "Error: Could not open file %s\n", infile);
        return -3;
    }

    status = h5array_read(file_id, stations_path, &stations, &nstations, &components);
    if (status < 0)
    {
        H5Fclose(file_id);
        return -4;
    }
    if (components != 3)
    {
        return -5;
    }

    status = H5Fclose(file_id);

    
    /*
     * Allocate memory. Skip stations array, since it has
     * already been allocated above. Also, do not allocate
     * flag2 array, since it is typically too large.
     */
    okada_alloc(nsubfaults, nstations,
                &subfaults, &models,
                NULL, &displacement, &derivatives, &stress,
                &flag, NULL);

    /*
     * Pick one of the following models
     */
    strike_slip_ng(subfaults, nsubfaults);
    //reverse_slip_ng(subfaults, nsubfaults);


    /*
     * Perform the actual calculation
     */
    calc_disp_cervelli(mu, nu,
                       models, subfaults, nsubfaults,
                       stations, nstations,
                       displacement, derivatives, stress,
                       flag, NULL);

    
    /*
     * Write all data to an HDF5 file (to root group for now)
     *
     * TODO: Specify the parent group of these datasets.
     *       Essentially, I need a function that creates
     *       all the necessary groups in a path.
     */
    
    file_id = H5Fcreate(outfile, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file_id < 0)
    {
        H5Fclose(file_id);
        fprintf(stderr, "Error: Could not write to file %s\n", outfile);
        return -6;
    }

    status = h5array_write(file_id, "/stations", stations, nstations, 3);
    if (status < 0)
    {
        H5Fclose(file_id);
        fprintf(stderr, "Error: Could not write dataset %s\n", "/stations");
    }

    status = h5array_write(file_id, "/displacement", displacement, nstations, 3);
    if (status < 0)
    {
        H5Fclose(file_id);
        fprintf(stderr, "Error: Could not write dataset %s\n", "/displacement");
    }
    
    status = h5array_write(file_id, "/derivatives", derivatives, nstations, 9);
    if (status < 0)
    {
        H5Fclose(file_id);
        fprintf(stderr, "Error: Could not write dataset %s\n", "/derivatives");
    }

    status = h5array_write(file_id, "/stress", stress, nstations, 6);
    if (status < 0)
    {
        H5Fclose(file_id);
        fprintf(stderr, "Error: Could not write dataset %s\n", "/stress");
    }

    status = H5Fclose(file_id);


    /* free the allocated memory */
    okada_free(subfaults, models, stations,
               displacement, derivatives, stress,
               flag, NULL);
    
    return 0;
}


int main(int argc, char *argv[])
{
    char arg1[300];
    char arg2[300];

    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s [hdf5 | txt] points.in points.out\n", argv[0]);
        return -1;
    }
    
    snprintf(arg1, 300, argv[2]);   // points.in or points.h5:/okada/stations
    snprintf(arg2, 300, argv[3]);   // points.out or data.h5

    if (strcmp(argv[1], "hdf5") == 0)
    {
        return h5_main(arg1, arg2);
    }
    else if (strcmp(argv[1], "txt") == 0)
    {
        return txt_main(arg1, arg2);
    }
    else
    {
        fprintf(stderr, "Error: Could not understand option \"%s\"\n", argv[3]);
    }


    return EXIT_FAILURE;
}

