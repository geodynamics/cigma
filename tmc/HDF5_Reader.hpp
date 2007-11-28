#ifndef GTB_HDF5_READER_INCLUDED
#define GTB_HDF5_READER_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_C_STDIO_INCLUDED
#include <stdio.h>
#define GTB_C_STDIO_INCLUDED
#endif

#ifndef GTB_POINT_3F_INCLUDED
#include "Point_3f.hpp"
#endif

#ifndef GTB_TIME_SERIES_INCLUDED
#include "Time_series.hpp"
#endif

#include <hdf5.h>

GTB_BEGIN_NAMESPACE

class Indexed_tetra_set;
class Tetrahedron;


class HDF5_Reader {
public:
    HDF5_Reader();

    int get_time_series(const char *filename,
                         const char *field_location,
                         Scalar_time_series **out_sts,
                         Vector_time_series **out_vts,
                         Indexed_tetra_set **out_its);

protected:
    std::vector<Point_3f> m_vertices;
    std::vector<Tetrahedron> m_tetrahedra;
};


GTB_END_NAMESPACE

#endif // GTB_HDF5_READER_INCLUDED
