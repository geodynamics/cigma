#ifndef GTB_INDEXED_TETRA_SET_OFF_READER_INCLUDED
#define GTB_INDEXED_TETRA_SET_OFF_READER_INCLUDED

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

GTB_BEGIN_NAMESPACE

class Indexed_tetra_set;
class Tetrahedron;


class Indexed_tetra_set_off_reader {
public:
	Indexed_tetra_set_off_reader();

        //Indexed_tetra_set *read(FILE *in_off_fp, FILE *out_scalars_fp);
     
        void read(FILE *in_off_fp,
	     Indexed_tetra_set **out_its,
	     Scalar_time_series **out_sts);

protected:
 	std::vector<Point_3f> m_vertices;
	std::vector<Tetrahedron> m_tetrahedra;
};


GTB_END_NAMESPACE

#endif // GTB_INDEXED_TETRA_SET_OFF_READER_INCLUDED
