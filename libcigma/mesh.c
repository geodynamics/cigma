#include <stdlib.h>
#include <assert.h>
#include <hdf5.h>
#include "h5_io.h"
#include "mesh.h"
#include "dataset.h"



/*
 * Input :  mesh object, element number e
 * Output:  dof coordinates -- [ndof x nsd] matrix
 */
void mesh_coords(mesh_t *mesh, int e, double *dof_coords)
{
    
    double *coord = mesh->coords;
    int *conn = mesh->connect;
    int ndof = mesh->ndof;

    int i,j;
    for (i = 0; i < ndof; i++)
    {
        j = conn[ndof*e + i];
        dof_coords[3*i + 0] = coord[3*j + 0];
        dof_coords[3*i + 1] = coord[3*j + 1];
        dof_coords[3*i + 2] = coord[3*j + 2];
    }
}



int mesh_init_coords(mesh_t *mesh, int nno, int nsd)
{
    mesh->nno = nno;
    mesh->nsd = nsd;
    mesh->coords = (double *)malloc(nno * nsd * sizeof(double));
    return 0;
}

int mesh_init_connect(mesh_t *mesh, int nel, int ndof)
{
    mesh->nel  = nel;
    mesh->ndof = ndof;
    mesh->connect = (int *)malloc(nel * ndof * sizeof(int));
    return 0;
}

int mesh_free(mesh_t *mesh)
{
    if (mesh->connect != NULL)
        free(mesh->connect);

    if (mesh->coords != NULL)
        free(mesh->coords);

    return 0;
}



int mesh_open(mesh_t *mesh,
              const char *filename,
              const char *path)
{
    hid_t file_id;
    hid_t model_id;
    int ierr;

    /* open the file in read-write mode */
    file_id = h5_file_open(filename, "rw");
    if (file_id < 0)
    {
        return -1;
    }
    
    model_id = H5Gopen(file_id, path);
    if (model_id < 0)
    {
        H5Fclose(file_id);
        return -2;
    }

    //
    // TODO: read attributes in model_id to determine the following paths
    //

    /* read all coords */
    ierr = mesh_read_coords(mesh, model_id, "coordinates");
    if (ierr < 0)
    {
        H5Gclose(model_id);
        H5Fclose(file_id);
        return -3;
    }
    assert(mesh->nsd == 3);

    /* read all elements */
    ierr = mesh_read_connect(mesh, model_id, "connectivity");
    if (ierr < 0)
    {
        H5Gclose(model_id);
        H5Fclose(file_id);
        return -4;
    }

    H5Gclose(model_id);
    H5Fclose(file_id);
    return 0;
}

int mesh_open2(mesh_t *mesh,
              const char *filename,
              const char *coords_path,
              const char *connect_path)
{
    hid_t file_id;
    int ierr;

    /* open the file in read-write mode */
    file_id = h5_file_open(filename, "rw");
    if (file_id < 0)
    {
        return -1;
    }
    
    /* read all coords */
    ierr = mesh_read_coords(mesh, file_id, coords_path);
    if (ierr < 0)
    {
        H5Fclose(file_id);
        return -2;
    }
    assert(mesh->nsd == 3);

    /* read all elements */
    ierr = mesh_read_connect(mesh, file_id, connect_path);
    if (ierr < 0)
    {
        H5Fclose(file_id);
        return -3;
    }

    H5Fclose(file_id);
    return 0;
}



int mesh_read_coords(mesh_t *mesh, hid_t loc_id, const char *coords_path)
{
    return dataset_read2(loc_id, coords_path, H5T_NATIVE_DOUBLE,
                         (void **)&(mesh->coords), &(mesh->nno), &(mesh->nsd));
}

int mesh_read_connect(mesh_t *mesh, hid_t loc_id, const char *connect_path)
{

    return dataset_read2(loc_id, connect_path, H5T_NATIVE_INT,
                         (void **)&(mesh->connect), &(mesh->nel), &(mesh->ndof));
}

