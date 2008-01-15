#ifndef __CERVELLI_H__
#define __CERVELLI_H__


void calc_disp_cervelli(double mu, double nu,
                        double *models, double *fault_info, int nfaults,
                        double *stations, int nstations,
                        double *disp, double *deriv, double *stress,
                        double *flag, double *flag2);

#endif
