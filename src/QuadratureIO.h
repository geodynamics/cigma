#ifndef __QUADRATURE_IO_H__
#define __QUADRATURE_IO_H__

#include <string>
#include "AnyOption.h"
#include "Reader.h"
#include "Writer.h"
#include "Quadrature.h"
#include "Cell.h"

class QuadratureIO
{
public:
    cigma::Reader *reader;
    cigma::Writer *writer;

public:
    std::string quadrature_order;
    std::string quadrature_path;
    std::string points_path;
    std::string weights_path;
    cigma::Quadrature *quadrature;

public:
    QuadratureIO();
    ~QuadratureIO();
    void load(cigma::Cell *cell);
    void save();
};


void configure_quadrature(AnyOption *opt, QuadratureIO *quadratureIO, const char *opt_prefix);


#endif
