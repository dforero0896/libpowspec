#ifndef _POWSPEC_H_
#define _POWSPEC_H_


#include "read_cata.h"
#include "multipole.h"

void free_pk_array(double *pk_array);
extern PK *compute_pk(CATA *cata, bool save_out, bool has_randoms, int argc, char *argv[]);
extern void build_catalog_sim(DATA* output, size_t ndata, double* sumw2, double* data_x, double* data_y, double* data_z, double* data_w);
extern void build_catalog_lc(size_t ndata, double* sumw2, double* wdata, double* data_x, double* data_y, double* data_z, double* data_w, double* data_fkp, double* data_nz);


#endif
