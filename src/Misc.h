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
#include "FE_Field.h"
#include "VtkUgReader.h"


double pick_from_interval(double a, double b);
void bbox_random_point(double minpt[3], double maxpt[3], double x[3]);


void load_mesh();
void load_quadrature();
void load_field();

void load_quadrature(cigma::Cell *cell,
                     cigma::Quadrature *quadrature);

void load_field(std::string inputfile,
                std::string location,
                cigma::VtkUgReader &reader,
                cigma::FE_Field *field);
#endif
