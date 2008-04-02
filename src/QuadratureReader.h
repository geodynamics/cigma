#ifndef __QUADRATURE_READER_H__
#define __QUADRATURE_READER_H__

#include <string>
#include "AnyOption.h"
//#include "Cell.h"
#include "MeshPartReader.h"
#include "QuadratureRule.h"


class QuadratureReader
{
public:
    QuadratureReader();
    ~QuadratureReader();

public:
    void load_args(AnyOption *opt, const char *opt_prefix);
    void validate_args(const char *cmd_name);

public:
    void set_mesh(cigma::MeshPart *meshPart);
    void load_mesh();

public:
    void load_quadrature();
    void check_interior();
    void warn_on_negative_weights();

public:
    std::string quadratureOrder;
    std::string quadraturePath;
    std::string pointsPath;
    std::string weightsPath;
    bool verbose;

public:
    cigma::QuadratureRule *quadrature;
    cigma::MeshPart *meshPart;
    MeshPartReader meshPartReader;
};


#endif
