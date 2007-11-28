#ifndef __CIGMA_QRULE_H__
#define __CIGMA_QRULE_H__
#include "../libcigma/rule.h"

class QuadratureRule
{
public:
    QuadratureRule();
    ~QuadratureRule();

    void set();
    void read(FILE *fp);

public:
    rule_t *qr;
};

#endif
