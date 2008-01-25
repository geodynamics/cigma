#ifndef __LOCATOR_H__
#define __LOCATOR_H__


namespace cigma
{
    class Locator;
    class MeshPart;
}


class cigma::Locator
{
public:
    Locator();
    virtual ~Locator();

public:
    virtual void initialize(MeshPart *meshPart) = 0;
    virtual void search(double *globalPoint) = 0;

public:
    virtual int n_idx() = 0;
    virtual int idx(int i) = 0;

public:
    int nsd;    // spatial dimensions
};


#endif
