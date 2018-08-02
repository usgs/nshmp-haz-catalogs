#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "agrid.h"
#include "agrid_functions.h"


/*--------------------------------------------------------------------------*/
int check_args(int argc, char **argv, char *paramf, char *summf, char *paramf_format, int *stop_run, int *newp)
/*--------------------------------------------------------------------------*/
{
/* CHECK INPUT ARGUMENTS */
  if (  (argc != 2) && (argc != 4) && (argc !=5)  ) {
    fprintf(stderr,"USAGE: %s [parameter file] [parameter file formatting (new/old)] [summary file name] [[ optional quit before run (1) ]]\n", argv[0]);
    fprintf(stderr,"For parameter file formatting, enter -h, as single command line argument.\n");
    fprintf(stderr,"Enter (new/old) to specify the formatting style for the parameter file.\n");
    fprintf(stderr,"Optional quit argument will stop run before seismicity rate calculations to permit check of parameters.\n"); exit(1);
  }
  sscanf(argv[1],"%s",paramf);
  if ( argc > 2) {
    sscanf(argv[2],"%s",paramf_format);
    sscanf(argv[3],"%s",summf);
    if (argc > 4) sscanf(argv[4],"%d", stop_run);
  }

// check for help option
  if ( STREQ(paramf,"-h") ) {
    print_param_format();
    return 1;
  }
// check pareter file formatting
  if ( STREQ(paramf_format,"new") ) {
    fprintf(stderr,"Using new parameter file formatting...\n");
    *newp=1;
    return 0;
  }
  else if ( STREQ(paramf_format,"old") ) {
    fprintf(stderr,"Using old parameter file formatting...\n");
    *newp=0;
    return 0;
  }
  else {
    fprintf(stderr,"User must enter 'new' or 'old' to specify the style of formatting for the parameter file - %s.\n", paramf_format);
    return 1;
  }
}


/*--------------------------------------------------------------------------*/
void write_results_to_file(struct smoothp* smp, struct rates* seisr, struct catalog * cat, char *outf)
/*--------------------------------------------------------------------------*/
{
  int cnt;
  float magv;
  float tmpsum=0.0;

// quick check of earthquake numbers
  for (cnt=0; cnt<seisr->nv; cnt++) {
    tmpsum=tmpsum+seisr->asum[cnt]*7;
  }
  fprintf(stderr,"tmpsum (no. M0+ eqs in 7 years) = %.4f\n", tmpsum);

// if a smoothed seismicity model is generated, output results at M0(10^a), M4, M5 and unsmoothed model
  if ( smp->apply_sm ) {
// M0, 10^a values
    magv=0.0;
    write_asum_to_file(seisr, outf, cat, magv);
// M0, 10^a, un-smoothed 
//    write_unsmoothed_asum_to_file(seisr, outf);

// M3.0, 10^(a-3.0b) values
/*
*    magv=3.0;
*    write_asum_to_file(seisr, outf, cat, magv);
// M3.5, 10^(a-3.5b) values
*    magv=3.5;
*    write_asum_to_file(seisr, outf, cat, magv);
// M4, 10^(a-4b) values
*    magv=4.0;
*    write_asum_to_file(seisr, outf, cat, magv);
// M4.5, 10^(a-4.5b) values
*    magv=4.5;
*    write_asum_to_file(seisr, outf, cat, magv);
*
// M5, 10^(a-5b) values
*    magv=5.0;
*    write_asum_to_file(seisr, outf, cat, magv);
*/    
  }
  else {
    magv=0.0;
    write_asum_to_file(seisr, outf, cat, magv);
    magv=cat->minMag;
    write_asum_to_file(seisr, outf, cat, magv);
  }
}



