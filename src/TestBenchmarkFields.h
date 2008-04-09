#ifndef __TestBenchmarkFields_h__
#define __TestBenchmarkFields_h__

#include "UserField.h"

namespace cigma
{
    namespace test_benchmarks
    {
        class Cube;
        class Sphere;
        class Square;
        class Circle;
    }
}

class cigma::test_benchmarks::Cube : public cigma::UserField
{
public:
    Cube();
    ~Cube();

public:
    int n_dim() { return 3; }
    int n_rank() { return 1; }

public:
    bool eval(double *x, double *value);

};


class cigma::test_benchmarks::Sphere : public cigma::UserField
{
public:
    Sphere();
    ~Sphere();

public:
    int n_dim() { return 3; }
    int n_rank() { return 3; }

public:
    bool eval(double *x, double *value);

};


class cigma::test_benchmarks::Square : public cigma::UserField
{
public:
    Square();
    ~Square();

public:
    int n_dim() { return 2; }
    int n_rank() { return 1; }

public:
    bool eval(double *x, double *value);

};


class cigma::test_benchmarks::Circle : public cigma::UserField
{
public:
    Circle();
    ~Circle();

public:
    int n_dim() { return 2; }
    int n_rank() { return 2; }

public:
    bool eval(double *x, double *value);

};


#endif
