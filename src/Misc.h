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
    //void configure(AnyOption *opt, const char *cmd, const char *name);
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
    //void configure(AnyOption *opt, const char *cmd, const char *name);
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
    //void configure(AnyOption *opt, const char *cmd, const char *name);
    void load();
    void save();
};



/* 
 * Misc Methods
 *
 */

double pick_from_interval(double a, double b);
void bbox_random_point(double minpt[3], double maxpt[3], double x[3]);

void configure_quadrature(AnyOption *opt, QuadratureIO *quadratureIO);
void configure_mesh(AnyOption *opt, MeshIO *meshIO, const char *opt_prefix);
void configure_field(AnyOption *opt, FieldIO *fieldIO, const char *opt_prefix);


/*
void load_reader(cigma::Reader **reader, std::string ext);
void load_writer(cigma::Writer **writer, std::string ext);
*/


/*
void load_mesh();
void load_quadrature();
void load_quadrature(cigma::Cell *cell, cigma::Quadrature *quadrature);
void load_field();
void load_field(cigma::Reader *reader,
                cigma::FE_Field *field,
                std::string field_location);






void load_mesh(cigma::MeshPart *meshPart,
               cigma::Reader *reader,
               std::string mesh_path,
               std::string coords_loc,
               std::string connect_loc);


void load_quadrature(cigma::Quadrature *quadrature,
                     cigma::Cell *cell,
                     cigma::Reader *reader,
                     std::string arg_quadrature_path,
                     std::string arg_points_loc,
                     std::string arg_weights_loc,
                     std::string arg_order);

void load_field(cigma::FE_Field *field,
                cigma::MeshPart *meshPart,
                cigma::Reader *fieldReader,
                cigma::Reader *meshReader,
                std::string field_path,
                std::string mesh_path,
                std::string coords_path,
                std::string connect_path);
*/

#endif
