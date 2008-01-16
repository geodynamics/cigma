#ifndef __MISC_H__
#define __MISC_H__

/* Miscellaneous functions yet to be classified
 *
 */


#include <cstdlib>
#include <ctime>


double pick_from_interval(double a, double b);
void bbox_random_point(double minpt[3], double maxpt[3], double x[3]);


void load_quadrature();
void load_field();

#endif
