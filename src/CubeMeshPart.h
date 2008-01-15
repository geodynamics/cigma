#ifndef __CUBE_MESH_PART_H__
#define __CUBE_MESH_PART_H__

#include "MeshPart.h"

namespace cigma
{
    class CubeMeshPart;
}

class cigma::CubeMeshPart : public cigma::MeshPart
{
public:
    CubeMeshPart();
    ~CubeMeshPart();

public:
    bool calc_coordinates(int L, int M, int N);
    bool calc_hex8_connectivity();
    bool calc_tet4_connectivity();

public:
    bool find_cell(double globalPoint[3], int *cellIndex);

public:
    int L,M,N;

    int hexnel;
    int *hexconn;

    int tetnel;
    int *tetconn;
};

#endif
