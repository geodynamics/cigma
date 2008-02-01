#ifndef __MISC_H__
#define __MISC_H__

/* Miscellaneous functions yet to be classified
 *
 */


#include <cstdlib>
#include <ctime>
#include <string>

#include "Cell.h"
#include "Quadrature.h"
#include "MeshPart.h"
#include "FE_Field.h"
#include "Reader.h"
#include "Writer.h"
#include "AnyOption.h"


/*
 * Misc Classes
 *
 */


class GeneralIO
{
public:
    cigma::Reader *reader;
    cigma::Writer *writer;

public:
    GeneralIO()
    {
        reader = 0;
        writer = 0;
    }
};

class MeshIO : public GeneralIO
{
public:
    std::string mesh_path;
    std::string coords_path;
    std::string connect_path;
    cigma::MeshPart *meshPart;

public:
    MeshIO();
    ~MeshIO();
    void load();
    void save();
};


class QuadratureIO : public GeneralIO
{
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



class FieldIO : public GeneralIO
{
public:
    MeshIO meshIO;
    QuadratureIO quadratureIO;
    std::string field_path;
    cigma::FE_Field *field;

public:
    FieldIO();
    ~FieldIO();
    void load();
    void save();
};



/* 
 * Misc Methods
 *
 */

double pick_from_interval(double a, double b);
void bbox_random_point(double minpt[3], double maxpt[3], double x[3]);

void configure_quadrature(AnyOption *opt, QuadratureIO *quadratureIO, const char *opt_prefix);
void configure_mesh(AnyOption *opt, MeshIO *meshIO, const char *opt_prefix);
void configure_field(AnyOption *opt, FieldIO *fieldIO, const char *opt_prefix);


#endif
