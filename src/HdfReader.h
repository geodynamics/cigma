#ifndef __HDF_READER_H__
#define __HDF_READER_H__

#include <string>
#include "hdf5.h"
#include "Reader.h"

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
    void open(std::string filename);
    void close();

public:
    void set_coordinates_path(const char *loc);
    void set_connectivity_path(const char *loc);
    void set_dataset_path(const char *loc);

public:
    void get_coordinates(double **coordinates, int *nno, int *nsd);
    void get_connectivity(int **connectivity, int *nel, int *ndofs);
    void get_dataset(double **data, int *num, int *dim);

public:
    std::string coords_path;
    std::string connect_path;
    std::string dataset_path;

    hid_t file_id;
    hid_t coords_id;
    hid_t connect_id;
    hid_t dataset_id;

    hid_t coords_type;
    hid_t connect_type;
    hid_t dataset_type;
};


#endif
