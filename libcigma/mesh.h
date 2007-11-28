#ifndef __CIGMA_MESH_H__
#define __CIGMA_MESH_H__
#include <hdf5.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    int nno;            // number of nodes
    int nsd;            // number of spatial dimensions
    int nel;            // number of elements
    int ndof;           // number of degrees of freedom per element
    double *coords;     // node coordinates
    int *connect;       // element connectivity
} mesh_t;



int mesh_init_coords(mesh_t *m, int nno, int nsd);
int mesh_init_connect(mesh_t *m, int nel, int ndof);
int mesh_free(mesh_t *m);


int mesh_open(mesh_t *m, const char *filename, const char *path);
int mesh_open2(mesh_t *m, const char *filename, const char *coords_path, const char *connect_path);


int mesh_read_coords(mesh_t *m, hid_t loc_id, const char *coords_path);
int mesh_read_connect(mesh_t *m, hid_t loc_id, const char *connect_path);


void mesh_coords(mesh_t *m, int e, double *dof_coords);



#ifdef __cplusplus
}
#endif
#endif /* __CIGMA_MESH_H__ */
