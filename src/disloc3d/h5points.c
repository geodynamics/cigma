#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>
#include "txtarray.h"
#include "h5array.h"

int main(int argc, char *argv[])
{
    char infile[100];
    char outfile[100];

    hid_t file_id;
    herr_t status;

    int nstations;
    double *stations;



    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s points.in points.out\n", argv[0]);
        return -1;
    }
    
    if (argc > 1)
    {
        snprintf(infile, 100, argv[1]);
    }
    else
    {
        snprintf(infile, 100, "points.in");
    }

    if (argc > 2)
    {
        snprintf(outfile, 100, argv[2]);
    }
    else
    {
        snprintf(outfile, 100, "points.out");
    }
    

    txtarray_read_stations(infile, &stations, &nstations); // allocates stations


    file_id = H5Fcreate(outfile, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file_id < 0)
    {
        H5Fclose(file_id);
        fprintf(stderr, "Error: Could not write to file %s\n", outfile);
        return -2;
    }
    

    status = h5array_write(file_id, "/stations", stations, nstations, 3);
    if (status < 0)
    {
        H5Fclose(file_id);
        fprintf(stderr, "Error: Could not write dataset /stations\n");
        return -3;
    }

    
    status = H5Fclose(file_id);

    free(stations);

    
    printf("Processed %d stations into dataset /stations in %s\n", nstations, outfile);

    
    return EXIT_SUCCESS;
}

