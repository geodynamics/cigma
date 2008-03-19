#ifndef __TXTARRAY_H__
#define __TXTARRAY_H__


int txtarray_read1(const char *filename,
                   double **data, int *m, int *n);

int txtarray_read_stations(const char *filename,
                           double **stations,
                           int *nstations);


int txtarray_write1(const char *filename, const char *header,
                    double *data, int m, int n);

int txtarray_write2(const char *filename,
                    const char *header,
                    int m,
                    double *data1, int n1,
                    double *data2, int n2);

int txtarray_write4(const char *filename,
                    const char *header,
                    int m,
                    double *data1, int n1,
                    double *data2, int n2,
                    double *data3, int n3,
                    double *data4, int n4);

int txtarray_write_disp(const char *filename,
                        const char *header,
                        int nstations,
                        double *stations,
                        double *displacements);

int txtarray_write_deriv(const char *filename,
                         const char *header,
                         int nstations,
                         double *stations,
                         double *derivatives);

int txtarray_write_stress(const char *filename,
                          const char *header,
                          int nstations,
                          double *stations,
                          double *displacements);

int txtarray_write_all(const char *filename,
                       const char *header,
                       int nstations,
                       double *stations,
                       double *displacements,
                       double *derivatives,
                       double *stress);

#endif
