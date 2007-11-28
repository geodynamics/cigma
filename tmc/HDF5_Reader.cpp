#ifndef GTB_HDF5_READER_INCLUDED
#include "HDF5_Reader.hpp"
#endif

#ifndef GTB_INDEXED_TETRA_SET_INCLUDED
#include "Indexed_tetra_set.hpp"
#endif

#ifndef GTB_TIME_SERIES_INCLUDED
#include "Time_series.hpp"
#endif

#include <hdf5.h>
#include "../libcigma/h5_io.h"
#include "../libcigma/h5_attr.h"
#include "../libcigma/dataset.h"
#include "../libcigma/mesh.h"

#include <iostream>

GTB_BEGIN_NAMESPACE

HDF5_Reader::HDF5_Reader()
{

}

int HDF5_Reader::get_time_series(const char *filename,
                                 const char *field_location,
                                 Scalar_time_series **out_sts,
                                 Vector_time_series **out_vts,
                                 Indexed_tetra_set **out_its)
{
    *out_sts = NULL;
    *out_vts = NULL;

    herr_t status;
    hid_t field_file_id;

    field_file_id = h5_file_open(filename, "rw");

    m_vertices.clear();
    m_tetrahedra.clear();

    hid_t dataset_id;
    dataset_id = H5Dopen(field_file_id, field_location);
    if (dataset_id < 0)
    {
        std::cerr << "Error: Could not open dataset " << field_location
                  << " in file " << filename << std::endl;
        H5Fclose(field_file_id);
        return -1;
    }

    char *mesh_location;
    status = get_str_attr(dataset_id, "MeshLocation", &mesh_location);
    if (status < 0)
    {
        std::cerr << "Error: Could not find metadata attribute 'MeshLocation'"
                  << " in dataset " << field_location
                  << " in file " << filename << std::endl;
        H5Fclose(field_file_id);
        return -2;
    }
    status = H5Dclose(dataset_id);

    int nno, ndim;
    double *dofs;
    int ierr;
    ierr = dataset_read2(field_file_id, field_location, H5T_NATIVE_DOUBLE,
                         (void **)&dofs, &nno, &ndim);

    if (ierr < 0)
    {
        std::cerr << "Error: Could not read dataset " << field_location
                  << " from " << filename << std::endl;
        status = H5Fclose(field_file_id);
        return -3;
    }

    std::vector<float> scalars;
    std::vector<Vector_3f> vectors;

    if (ndim == 1)
    {
        scalars.resize(nno);
        for (int i = 0; i < nno; i++)
        {
            scalars[i] = dofs[i];
        }
        *out_sts = new Scalar_time_series(scalars);
    }
    else if (ndim == 3)
    {
        vectors.resize(nno);
        for (int i = 0; i < nno; i++)
        {
            vectors[i] = Vector_3f(dofs[3*i], dofs[3*i+1], dofs[3*i+2]);
        }
        *out_vts = new Vector_time_series(vectors);
    }

    hid_t mesh_group_id;
    mesh_group_id = h5_group_open(field_file_id, mesh_location);
    if (mesh_group_id < 0)
    {
        std::cerr << "Error: Could not find " << mesh_location
                  << " in file " << filename << std::endl;
        return -4;
    }

    mesh_t mesh;
    ierr = mesh_read_coords(&mesh, mesh_group_id, "coordinates");
    if (ierr < 0)
    {
        std::cerr << "Error: Could not open coordinates dataset in group "
                  << mesh_location << " from file " << filename << std::endl;
        return -5;
    }

    ierr = mesh_read_connect(&mesh, mesh_group_id, "connectivity");
    if (ierr < 0)
    {
        std::cerr << "Error: Could not open connectivity dataset in group "
                  << mesh_location << " from file " << filename << std::endl;
        return -6;
    }

    free(mesh_location);

    status = H5Gclose(mesh_group_id);
    status = H5Fclose(field_file_id);

    m_vertices.resize(mesh.nno);
    for (int i = 0; i < mesh.nno; i++)
    {
        m_vertices[i] = Point_3f(mesh.coords[3*i],
                                 mesh.coords[3*i+1],
                                 mesh.coords[3*i+2]);
    }
    
    m_tetrahedra.resize(mesh.nel);
    for (int e = 0; e < mesh.nel; e++)
    {
        m_tetrahedra[e] = Tetrahedron(mesh.connect[4*e],
                                      mesh.connect[4*e+1],
                                      mesh.connect[4*e+2],
                                      mesh.connect[4*e+3]);
    }

    mesh_free(&mesh);

    *out_its = new Indexed_tetra_set(m_vertices, m_tetrahedra);

    return 0;
}

GTB_END_NAMESPACE
