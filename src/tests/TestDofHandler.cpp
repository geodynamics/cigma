#include <iostream>
#include "../DofHandler.h"

using namespace cigma;

int main(int argc, char *argv[])
{

    const int nno = 8;
    const int ndim = 3;
    double data[nno*ndim] = {
        100, 101, 102,  //0
        110, 111, 112, 
        120, 121, 122,
        130, 131, 132,
        200, 201, 202,  //4
        210, 211, 212,
        220, 221, 222,
        230, 231, 232
    };

    DofHandler *dof_handler = new DofHandler();

    dof_handler->set_data(data, nno, ndim);


    {
        double d[4*ndim];
        int nodes[4] = {2, 3, 6, 7};

        dof_handler->get_data(4, nodes, d);

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < ndim; j++)
            {
                std::cout << d[ndim*i + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    delete dof_handler;

    return 0;
}
