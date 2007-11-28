#ifndef GTB_WRITERS_INCLUDED
#define GTB_WRITERS_INCLUDED

#include "namespace.hpp"
#include "Point_3f.hpp"
#include "Tetrahedron.hpp"
#include "Time_series.hpp"

GTB_BEGIN_NAMESPACE


int residuals_write_off(const char *filename,
                        const std::vector<Point_3f> &vertices,
                        const std::vector<Tetrahedron> &tets,
                        const std::vector<float> &residuals);

int residuals_write_vtk(const char *filename,
                        const std::vector<Point_3f> &vertices,
                        const std::vector<Tetrahedron> &tets,
                        const std::vector<float> &residuals);


GTB_END_NAMESPACE


#endif // GTB_WRITERS_INCLUDED
