#ifndef __EXTERNAL_FIELD_H__
#define __EXTERNAL_FIELD_H__

#include <string>
#include "Points.h"
#include "Field.h"


namespace cigma
{
    class ExternalField;
}


/**
 * @brief External Field object
 *
 */
class cigma::ExternalField : public Field
{
public:
    typedef void (*get_int_fn)(int*);
    typedef void (*eval_from_point_fn)(double*,double*);
    typedef void (*eval_from_points_fn)(int,double*,double*);

public:
    ExternalField();
    ~ExternalField();

public:
    int n_dim() { return dim; }
    int n_rank() { return rank; }

public:
    void load_library(std::string filename, std::string prefix);
    void close_library();

public:
    void eval(double *x, double *y);
    void eval(Points &domain, Points &range);

public:
    int dim;
    int rank;
    void *handle;
    get_int_fn ext_get_dim;
    get_int_fn ext_get_rank;
    eval_from_point_fn ext_eval;
    eval_from_points_fn ext_batch_eval;
};


#endif
