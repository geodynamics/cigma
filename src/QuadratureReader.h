#ifndef __QUADRATURE_READER_H__
#define __QUADRATURE_READER_H__

#include <string>
#include "AnyOption.h"
#include "Reader.h"
#include "QuadraturePoints.h"
#include "Cell.h"

class QuadratureReader
{
public:
    QuadratureReader();
    ~QuadratureReader();

public:
    void load_args(AnyOption *opt, const char *opt_prefix);
    void validate_args(const char *cmd_name);

public:
    void load_quadrature(cigma::Cell *cell);

public:
    std::string quadratureOrder;
    std::string quadraturePath;
    std::string pointsPath;
    std::string weightsPath;

public:
    cigma::Reader *reader;
    cigma::QuadraturePoints *quadrature;
};

#endif
