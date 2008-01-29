#ifndef __HDF_READER_H__
#define __HDF_READER_H__

#include <string>
#include "hdf5.h"
#include "Reader.h"

#include "Quadrature.h"
#include "MeshPart.h"
#include "DofHandler.h"
#include "FE_Field.h"


namespace cigma
{
    class HdfReader;
}


class cigma::HdfReader : public Reader
{
public:
    HdfReader();
    ~HdfReader();

public:
    ReaderType getType() { return HDF_READER; }

public:
    void open(std::string filename);
    void close();

public:
    void get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd);
    void get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs);
    void get_dataset(const char *loc, double **data, int *num, int *dim);

public:
    void get_quadrature(Quadrature *quadrature, const char *loc);
    void get_quadrature(Quadrature *quadrature, const char *points_loc, const char *weights_loc);

public:
    void get_field(FE_Field *field, const char *loc);
    void get_mesh(MeshPart *meshPart, const char *coords_loc, const char *connect_loc);
    void get_mesh(MeshPart *meshPart, const char *loc);
    void get_dofs(DofHandler *dofHandler, const char *loc);

public:
    hid_t file_id;
};


#endif
