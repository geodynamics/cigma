#ifndef __DISLOC3D_H__
#define __DISLOC3D_H__

void disloc3d(double *models, int nmod,
              double *stations, int nstat,
              double mu, double nu,
              double *uout, double *dout, double *sout,
              double *flag, double *flag2);

#endif
