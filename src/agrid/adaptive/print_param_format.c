#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------------------*/
void print_param_format(void) 
/*--------------------------------------------------------------------------*/
{
  fprintf(stderr, "\n ====================================================\n");
  fprintf(stderr, " === Input file format (description and examples) ===\n");
  fprintf(stderr, " ====================================================\n");
  fprintf(stderr," latmin latmax\n"); 
  fprintf(stderr," lonmin lonmax\n"); 
  fprintf(stderr," dlat dlon\n"); 
  fprintf(stderr," no. of magnitude/year completeness levels\n"); 
  fprintf(stderr," magnitude - year (multiple lines for N>1)\n");
  fprintf(stderr," last year of catalog\n");
  fprintf(stderr," name of catalog file\n");
  fprintf(stderr," minM dMag b-value\n");
  fprintf(stderr," logical, output cumulative rates (1=cumul.; 0=incr.)\n");
  fprintf(stderr, " logical, adjust completeness (1=adjust; 0=no-adjust)\n");
  fprintf(stderr, " logical, apply smoothing (1=smooth; 0=no-smoothing)\n");
  fprintf(stderr, " logical, adaptive smoothing (1=adaptive; 0=fixed-R)\n");
  fprintf(stderr, " sigmin, sigmax (gaussian sigmas; sigmax for fixed-R)\n");
  fprintf(stderr, " neighbor number for adaptive smoothing (e.g., 1/2/5)\n");
  fprintf(stderr, " output file name (e.g., agrd_ceus.out)\n");
  fprintf(stderr, " logical to print summary of input parameters, only\n");
  fprintf(stderr, " ====================================================\n\n");
}
