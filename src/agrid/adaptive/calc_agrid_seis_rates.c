#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "agrid.h"
#include "agrid_functions.h"
#include "agrid_functions_fortran.h"


/* 
 * Code carries out smoothing of seismicity rates, using various smoothing methods
 * Modified from agrid_cumul_compl_eq.f
 * Original versions from A. Frankel, 1994, with modifications 
 * by C. Mueller and S. Harmsen
 * 
 * Morgan P. Moschetti
 * last modified: 8/1/2014
 * 
 */


/*--------------------------------------------------------------------------*/
int main (int argc, char *argv[])
/*--------------------------------------------------------------------------*/
{
  int stop_run=0, fnew=0;
//  float magv;
  float atotal[NV];
  char paramf[200], paramf_format[200], summf[200], outf[200];
  struct compl_levels cat_compl;
  struct boundaries region;
  struct catalog cat;
  struct smoothp smoothing_params;
  struct rates seis_rates;
  struct eqcat eq_catalog;


// check input arguments and assign command line arguments for parameter file, file formatting, summary file
  if ( check_args(argc,argv,paramf,summf,paramf_format,&stop_run,&fnew) ) exit(1);
  
// initialize parameters, then open and read parameters file
  initialize_params(&cat_compl, &region, &cat, &smoothing_params);
  if( ! read_params_new_old(paramf, &cat_compl, &region, &cat, &smoothing_params, outf,fnew) ) exit(1);
fprintf(stderr,"outfnm: %s\n", outf);
  write_params(&cat_compl, &region, &cat, &smoothing_params, outf);
  write_params_to_file(summf, &cat_compl, &region, &cat, &smoothing_params, outf);
  if ( stop_run == 1 ) exit(1);

// get source region parameters
  fprintf(stderr,"Number of source points: %d (%d x %d)\n",region.nsrc, region.nsx, region.nsy);
  if (region.nsrc > NV ) {
    fprintf(stderr,"nsrc (%d) > NV (%d)\n", region.nsrc, NV); exit(1);
  }

// initialize distances to neighbors and seismicity rate variables
  init_dist_rates(&eq_catalog,atotal,&seis_rates);

// read earthquakes from catalog and assign to grid cells
  if ( ! get_eqs_fm_catalog(cat.catnm, &eq_catalog, &cat_compl, &region, &seis_rates) ) {
    fprintf(stderr,"Error getting earthquakes from catalog. Exiting ...\n"); exit(1);
  }
  seis_rates.nv=region.nsrc;

// get distances from all earthquakes to 10-nearest neighbors
  get_eq_neighbor_dists(&eq_catalog);

// smooth seismicity rates, if smoothing is specified
  smooth_seis_rates(&seis_rates, &smoothing_params, &eq_catalog, &region, &cat_compl, &cat);
  write_eq_neighbor_dists(&eq_catalog, &smoothing_params);
// convert from earthquake numbers in each bin to earthquake rates, adjust completeness, if required
  convert_eq_nos_rates(&seis_rates, &cat_compl, &smoothing_params, &cat, &region);

// write results to file
//fprintf(stdout,"call to write_results_to_file - %s\n", outf);
  write_results_to_file(&smoothing_params, &seis_rates, &cat, &region, outf);

// code completion steps
  fprintf(stdout,"\nSummary of processing written to file, %s\n", summf);
  return 0;
}
