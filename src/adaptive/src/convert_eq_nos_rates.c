#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "agrid.h"


/*--------------------------------------------------------------------------*/
void convert_eq_nos_rates(struct rates* seisr, struct compl_levels* compl, struct smoothp* smp, struct catalog* cat, struct boundaries* reg)
/*--------------------------------------------------------------------------*/
{
  int cnt;
  float isum=0.0;
  float fac, sum0=0.0, sum3=0.0, sum4=0.0, sum5=0.0, sum_noSm=0.0;
  float arg_pow;

// get factors for converting cumulative to incremental rates 
// Herrmann's formula for converting cumulative to incremental
// set factor to zero to retain cumulative rates
  if ( smp->cumul_rates == 1 ) {
    fac=0.0;
  }
  else {
// 0.5-factor gives dMag about a magnitude value
    arg_pow=(cat->bval*cat->dMag*0.5);
    fac=log10f( powf(10,arg_pow) - powf(10,-arg_pow) );
    fprintf(stdout,"bval/dMag/arg_pow/powf: %f %f %f %f \n", cat->bval, cat->dMag, arg_pow, powf(10,arg_pow));
  }
  fprintf(stdout,"Additive factor (log) converting cumulative to incremental (cumulative fac=0): %.4f\n", fac);

// loop through rates at all points to modify rates
// apply to smoothed and earthquake count results
  fprintf(stdout,"nsrc pts: %d\n", seisr->nv);
  for (cnt=0; cnt<seisr->nv; cnt++) {
    if ( seisr->asum[cnt] > 0 ) {
//if (cnt==29375) fprintf(stderr,"asum smoothed (M4+), %f \n", seisr->asum[cnt]);
// convert from (cumulative) counts per cell to rates
      isum=isum+seisr->asum[cnt];
// convert to log(rate)(M=0), cumulative - this is now done in smooth_seis_rates.c
//      seisr->asum[cnt]=log10f(seisr->asum[cnt]) + cat->bval*cat->minMag;
//      seisr->asum_noSm[cnt]=log10f(seisr->asum_noSm[cnt]) + cat->bval*cat->minMag;
      seisr->asum[cnt]=log10f(seisr->asum[cnt]);
      seisr->asum_noSm[cnt]=log10f(seisr->asum_noSm[cnt]);
// convert cumulative to incremental rates, if specified
      seisr->asum[cnt]=seisr->asum[cnt]+fac;
      seisr->asum_noSm[cnt]=seisr->asum_noSm[cnt]+fac;
// convert back to rates, 10^a
      seisr->asum[cnt]=powf(10,seisr->asum[cnt]);
      seisr->asum_noSm[cnt]=powf(10,seisr->asum_noSm[cnt]);
      sum0=sum0+seisr->asum[cnt];
      sum3=sum3+seisr->asum[cnt]*powf(10,-(cat->bval)*3);
      sum4=sum4+seisr->asum[cnt]*powf(10,-(cat->bval)*4);
      sum5=sum5+seisr->asum[cnt]*powf(10,-(cat->bval)*5);
      sum_noSm=sum_noSm+seisr->asum_noSm[cnt];
    }
//if (cnt==29375) fprintf(stderr,"(rates, M0), final %f \n", seisr->asum[cnt]);
  }
/*  float maxr=0; int imaxr=0;
  for (cnt=0; cnt<seisr->nv; cnt++) {
    if (seisr->asum[cnt]>maxr) {
      maxr=seisr->asum[cnt];
      imaxr=cnt;
    }
  } 
 fprintf(stderr,"maxrate: %f %d\n", maxr, imaxr); */
  
// print summary rate info
//  fprintf(stdout,"%d eqs -> rates M0/3/4/5: %.4e %.4e %.4e %.4e\n", isum, sum0, sum3, sum4, sum5);
  fprintf(stdout,"%.3f eqs -> sum rates (no./yr), M0/3/4/5: %.1f %.1f %.1f %.1f\n", isum, sum0, sum3, sum4, sum5);

}
