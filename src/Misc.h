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


double pick_from_interval(double a, double b);
void bbox_random_point(double minpt[3], double maxpt[3], double x[3]);


void load_mesh();
void load_quadrature();
void load_field();

void load_reader(cigma::Reader **reader, std::string ext);
void load_writer(cigma::Writer **writer, std::string ext);

void load_quadrature(cigma::Cell *cell,
                     cigma::Quadrature *quadrature);

void load_field(cigma::Reader *reader,
                cigma::FE_Field *field,
                std::string field_location);

#endif
