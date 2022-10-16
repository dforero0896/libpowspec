/*******************************************************************************
* powspec.c: this file is part of the powspec program.

* powspec: C code for auto and cross power spectra evaluation.

* Github repository:
        https://github.com/cheng-zhao/powspec

* Copyright (c) 2020 Cheng Zhao <zhaocheng03@gmail.com>  [MIT license]

*******************************************************************************/

#include "define.h"
#include "load_conf.h"
#include "read_cata.h"
#include "cnvt_coord.h"
#include "genr_mesh.h"
#include "multipole.h"
#include "save_res.h"
#include <stdio.h>
#include <stdlib.h>


DATA* build_catalog_sim(size_t ndata, double* sumw2, double* wdata, double* data_x, double* data_y, double* data_z, double* data_w){
  DATA* output = malloc(sizeof(DATA) * ndata);
  for (int i = 0; i < ndata; i++){
    output[i].x[0] = data_x[i];
    output[i].x[1] = data_y[i];
    output[i].x[2] = data_z[i];
    output[i].w = data_w[i];
    wdata[0] += data_w[i];
  }
  return output;
}

DATA* build_catalog_lc(size_t ndata, double* sumw2, double* wdata, double* data_x, double* data_y, double* data_z, double* data_w, double* data_fkp, double* data_nz){
  DATA* output = malloc(sizeof(DATA) * ndata);
  double w;
   
  for (int i = 0; i < ndata; i++){
    w = data_w[i] * data_fkp[i];
    output[i].x[0] = data_x[i];
    output[i].x[1] = data_y[i];
    output[i].x[2] = data_z[i];
    output[i].w = data_w[i];
    wdata[0] += data_w[i];
    sumw2[0] += w * w;
    sumw2[1] += data_w[i] * data_fkp[i] * data_fkp[i] * data_nz[i];
  }
  return output;
}





PK *compute_pk(CATA *cata, bool save_out, bool has_randoms, int argc, char *argv[]) {
  //printf("The following arguments were passed to main():\n");
  //printf("argnum \t value \n");
  //for (int i = 0; i<argc; i++) printf("%d \t %s \n", i, argv[i]);
  //printf("\n");

  printf("%lf\n", cata->data[0][5].x[0]);
  printf("%lf\n", cata->data[0][5].x[1]);
  printf("%lf\n", cata->data[0][5].x[2]);
  printf("%lf\n", cata->data[0][5].w);
  printf("%lf\n", cata->wdata[0]);
  CONF *conf;
  if (!(conf = load_conf(argc, argv))) {
    printf(FMT_FAIL);
    P_EXT("failed to load configuration parameters\n");
    return NULL;
  }
  
  conf->ndata = cata->num; // Trigger XPk computation even if not saved
  if (cata->rand != NULL){
    conf->has_randoms = has_randoms;
  }
 
  if (cnvt_coord(conf, cata)) {
    printf(FMT_FAIL);
    P_EXT("failed to convert coordinates\n");
    conf_destroy(conf); cata_destroy(cata);
    return NULL;
  }

  MESH *mesh;
  if (!(mesh = genr_mesh(conf, cata))) {
    printf(FMT_FAIL);
    P_EXT("failed to generate the density fields\n");
    conf_destroy(conf); cata_destroy(cata);
    return NULL;
  }

  PK *pk;
  if (!(pk = powspec(conf, cata, mesh))) {
    printf(FMT_FAIL);
    P_EXT("failed to compute the power spectra\n");
    conf_destroy(conf); cata_destroy(cata); mesh_destroy(mesh);
    return NULL;
  }

  //for (int i = 0; i < pk->nbin;i++){
  //  for (int k = 0; k < cata->num; k++){
  //    printf("%lf ", pk->pl[k][1][i]);
  //  }
  //  printf("\n");
  //  
  //}


   if (save_out && save_res(conf, cata, mesh, pk)) {
     printf(FMT_FAIL);
     P_EXT("failed to write the output to file.\n");
     conf_destroy(conf);
     mesh_destroy(mesh); powspec_destroy(pk);
     return POWSPEC_ERR_SAVE;
   }

  conf_destroy(conf);
  mesh_destroy(mesh);
  
  return pk;
  
}
 