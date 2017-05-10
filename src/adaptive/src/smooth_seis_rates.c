#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "agrid.h"
#include "agrid_functions_fortran.h"
#define VTHR 1e-10

/*--------------------------------------------------------------------------*/
float get_sigma_value(struct smoothp* smp, struct eqcat* catn, int eqno)
/*--------------------------------------------------------------------------*/
{
  int neighbor_num=-999, neighbor_num_2=-999;
  float sigma;

// fixed-radius smoothing
  if (smp->adapt_sm == 0 ) {
    sigma=smp->sigfix;
  }
// adaptive-radius smoothing
  else {
// shift for c array indexing
    neighbor_num=smp->nnum-1;
    if ( smp->nnum2_log == 0 ) {
      sigma=catn->dist_nbr[eqno][neighbor_num];
    }
    else {
      neighbor_num_2=smp->nnum2-1;
      sigma=(catn->dist_nbr[eqno][neighbor_num]+catn->dist_nbr[eqno][neighbor_num_2])/2;
    }
    if (sigma < smp->sigmin) sigma=smp->sigmin;
  }
  
  return sigma;
}

/*--------------------------------------------------------------------------*/
float check_earthquake_catalog_gaps(struct eqcat* eqcatp, struct compl_levels* cat_compp, int eqno)
/*--------------------------------------------------------------------------*/
{
  int cnt, cnt1, np, nyrs;
  int eqyr, eqmon, eqday, eqlineno;
  float eqlon, eqlat, eqmag;
  float plon[NPP], plat[NPP];

// assign coordinates
  eqlon=eqcatp->lon[eqno];
  eqlat=eqcatp->lat[eqno];
  eqmag=eqcatp->mag[eqno];
  eqyr=eqcatp->year[eqno];
  eqmon=eqcatp->month[eqno];
  eqday=eqcatp->day[eqno];
  eqlineno=eqcatp->lineno[eqno];

// loop through polygons and check if eq epicenter is within polygons
  for(cnt=0;cnt<cat_compp->polygons.npoly; cnt++) {
    np=cat_compp->polygons.elem[cnt].npts;
    nyrs=cat_compp->polygons.elem[cnt].nyears;
    for(cnt1=0;cnt1<np;cnt1++) {
      plon[cnt1]=cat_compp->polygons.elem[cnt].lon[cnt1];
      plat[cnt1]=cat_compp->polygons.elem[cnt].lat[cnt1];
    }
    if ( lxyint_(&eqlon,&eqlat,plon,plat,&np) ) { 
//      fprintf(stderr,"EQ-reduced-time-for-rate: %.3f %.3f %d %d %d %f %d\n", eqlon, eqlat, eqyr, eqmon, eqday, eqmag, eqlineno);
      return nyrs;
    }
  }

  return 0;
}

/*--------------------------------------------------------------------------*/
float calc_seis_rate(struct smoothp* smpp, struct eqcat* eqcatp, struct compl_levels* cat_compp, 
	struct catalog* cat_parp, int eqno)
/*--------------------------------------------------------------------------*/
{
  int cnt;
  float num_fac, denom_fac, rate_fac, ratev, fac_exp, mag_Mc;
  float bvalue, beta, n_eff, mag_eq, denom_mod;
  float exp_rate;

// 
  bvalue=cat_parp->bval;
  beta=2.303*bvalue;
  mag_eq=eqcatp->mag[eqno];
  mag_Mc=eqcatp->mag_Mc[eqno];

// check if earthquake occurred in a region with catalog gaps
// this feature allows for removing potentially induced earthquakes from catalog but 
// correctly calculating the rates with reduced time windows
  if ( (cat_compp->catalogGaps) == 1 ) {
    denom_mod=check_earthquake_catalog_gaps(eqcatp,cat_compp,eqno);
  }
  else {
    denom_mod=0;
  }

// convert from numbers to rates here
  if ( smpp->adj_compl == 1 ) {
    num_fac=0.0;
    denom_fac=0.0;
    for(cnt=0; cnt<cat_compp->nlevels; cnt++) {
      fac_exp=exp(-beta*(cat_compp->mag_comp[cnt]));
      num_fac=num_fac+fac_exp;
      denom_fac=denom_fac+fac_exp*(cat_parp->end_yr-cat_compp->yr_comp[cnt]+1);
    }
  }
  else {
    num_fac=1.0;
    if ( cat_compp->variableCompleteness == 1 ) {
      denom_fac=(cat_parp->end_yr-eqcatp->year_compl[eqno]+1);
    }
    else {
      denom_fac=(cat_parp->end_yr-cat_compp->yr_comp[0]+1);
    }
    denom_fac=denom_fac-denom_mod;
  }
  if (denom_fac<0) { 
    fprintf(stderr,"Problem with rate calculation.\n");
    exit(1);
  }
  rate_fac=num_fac/denom_fac;

// apply effective-number-count here, if appropriate 
  if ( (smpp->use_neff) == 1 ) {
    n_eff=eqcatp->neff[eqno];
  }
  else {
    n_eff=1.00;
  }
// ratev corresponds to the annual rate of an M=0 earthquake (array element, eqno) in the catalog 
// this version of the code then converts this effective rate (from count or n_eff) and converts to number of M0 earthquakes
// ratev values are magnitude-corrected effective 10^a values for a single earthquake 
  exp_rate=cat_parp->bval*mag_Mc;
  n_eff=n_eff*powf(10, exp_rate);
  ratev=rate_fac*n_eff;
    
  return ratev;
}



/*--------------------------------------------------------------------------*/
void smooth_seis_rates(struct rates* seisr, struct smoothp* smp, struct eqcat* catn, struct boundaries* reg, struct compl_levels* cat_comp, struct catalog* cat_par)
/*--------------------------------------------------------------------------*/
{
  int cnt, ix, iy, ix1, iy1, n;
  int log_shift_eq, log_shift_pt, log_actual_eq_loc;
  int imaxv=0, tot_asum_int;
  float maxv=0.0, tot_asum, n_eff, eq_rate, beta, area_scaling;
  float lonEq, latEq, lonPt, latPt, sigma, avg_lat;
//  float lonEq1, latEq1;
  float dist, az, baz, fac_exp, sig2, dist2;
  float norm_term, kernel_sum, sum_eqs=0.0;

// logicals for adjusting earthquake points to grid cell locations and shifting grid cell 
  log_actual_eq_loc=0;
  log_shift_eq=1;
  log_shift_pt=0;

// 
  beta=2.303*cat_par->bval;

// check if smoothing is to be applied
/* -------------------------------------------------------- */
// NO SMOOTHING
// time duration normalization - adjust completeness levels by Wiechert
// adjusts sum for different completeness times, assuming magnitude thresholds differ by 1 m.u.
/* -------------------------------------------------------- */
  if ( smp->apply_sm == 0 ) { 
    for(cnt=0; cnt<catn->nv; cnt++) {
//      iy=(int)nearbyintf(((reg->maxlat-catn->lat[cnt])/reg->dlat));
//      ix=(int)nearbyintf(((catn->lon[cnt]-reg->minlon)/reg->dlon));
      iy=(int)roundf(((reg->maxlat-catn->lat[cnt])/reg->dlat));
      ix=(int)roundf(((catn->lon[cnt]-reg->minlon)/reg->dlon));
      n=iy*reg->nsx+ix;
      catn->agrid_id[cnt]=n;
// convert from numbers to rates here
      eq_rate=calc_seis_rate(smp, catn, cat_comp, cat_par, cnt);
      n_eff=eq_rate;

//  Sum earthquake integers, effective-numbers-earthquakes in asum_cnt and asum
      seisr->asum_cnt_int[n]++;
//HERE 
      seisr->asum_cnt[n]=seisr->asum_cnt[n]+n_eff;
      seisr->asum[n]=seisr->asum[n]+n_eff;
    }
    fprintf(stderr,"Earthquakes assigned to grid cells (no smoothing) - %d earthquakes. ", catn->nv);
    maxv=0.0; imaxv=0; tot_asum=0.0; tot_asum_int=0;
    for(cnt=0; cnt<seisr->nv; cnt++) {
      if (seisr->asum[cnt]>maxv) {
        maxv=seisr->asum[cnt];
        imaxv=cnt;
      } 
      tot_asum_int=tot_asum_int+seisr->asum_cnt_int[cnt];
      tot_asum=tot_asum+seisr->asum[cnt];
    }
    fprintf(stdout,"Earthquakes recovered from grid cells: total-rates,eff=%.2f, total-numbers=%d\n", tot_asum, tot_asum_int);
    fprintf(stdout,"Max number earthquakes in a cell: %.2f\n", maxv);
  }
/* -------------------------------------------------------- */
// SMOOTHING
/* -------------------------------------------------------- */
  else {

// assign eqs to grid cells for un-smoothed results and maximum, un-smoothed rate
    avg_lat=0.5*(reg->minlon+reg->maxlon);
    for(cnt=0; cnt<catn->nv; cnt++) {
//      iy=(int)nearbyintf(((reg->maxlat-catn->lat[cnt])/reg->dlat));
//      ix=(int)nearbyintf(((catn->lon[cnt]-reg->minlon)/reg->dlon));
      iy=(int)roundf(((reg->maxlat-catn->lat[cnt])/reg->dlat));
      ix=(int)roundf(((catn->lon[cnt]-reg->minlon)/reg->dlon));
      n=iy*reg->nsx+ix;
      catn->agrid_id[cnt]=n;
// convert from numbers to rates here
      eq_rate=calc_seis_rate(smp, catn, cat_comp, cat_par, cnt);
      n_eff=eq_rate;
// summing over asum
// get un-smoothed results for comparison
      seisr->asum_cnt_int[n]++;
      seisr->asum_cnt[n]=seisr->asum_cnt[n]+n_eff;
    }
    for(cnt=0; cnt<seisr->nv; cnt++) seisr->asum_noSm[cnt]=(float)seisr->asum_cnt[cnt];
// get maximum, un-smoothed rate
    maxv=0.0; imaxv=0; tot_asum=0.0; tot_asum_int=0;
    for(cnt=0; cnt<seisr->nv; cnt++) {
      if (seisr->asum_noSm[cnt]>maxv) {
        maxv=seisr->asum_noSm[cnt];
        imaxv=cnt;
      } 
      tot_asum_int=tot_asum_int+seisr->asum_cnt_int[cnt];
      tot_asum=tot_asum+seisr->asum_cnt[cnt];
    }
    fprintf(stdout,"Maximum, un-smoothed rate (eq nos/cell/yr), cell-number: %.2f %d\n", maxv, imaxv);
    fprintf(stdout,"Earthquakes recovered from grid cells: effective-rate=%.2f, #eqs=%d\n", tot_asum, tot_asum_int);
//    fprintf(stdout,"Max number earthquakes in a cell: %.2f\n", maxv);
//    fprintf(stderr,"unsmoothed nv: %d\n", seisr->nv);
//    fprintf(stderr,"maxv: %f \n", seisr->asum_noSm[35185]);

// SMOOTH GRID CELL NUMBERS
// loop over all earthquake sources to get smoothed rates
// assign eq location to center of cell
    fprintf(stderr,"Smoothing rates...\n");
    for(cnt=0; cnt<catn->nv; cnt++) {
      iy1=(int)roundf(((reg->maxlat-catn->lat[cnt])/reg->dlat));
      ix1=(int)roundf(((catn->lon[cnt]-reg->minlon)/reg->dlon));
//      iy1=(int)nearbyintf(((reg->maxlat-catn->lat[cnt])/reg->dlat));
//      ix1=(int)nearbyintf(((catn->lon[cnt]-reg->minlon)/reg->dlon));
//      iy1=(int)((reg->maxlat-catn->lat[cnt])/reg->dlat);
//      ix1=(int)((catn->lon[cnt]-reg->minlon)/reg->dlon);
// convert from numbers to rates here
      eq_rate=calc_seis_rate(smp, catn, cat_comp, cat_par, cnt);
      n_eff=eq_rate;
// check location parameters
      if ( (ix1<0) || (ix1>=reg->nsx) || (iy1<0) || (iy1>=reg->nsy) ) {
        fprintf(stderr,"Error with earthquake location parameters, lon/lat/ix/iy/nsx/nsy: %.3f %.3f %d %d (%d %d)\n", catn->lon[cnt], catn->lat[cnt], ix1, iy1, reg->nsx, reg->nsy);
        exit(1);
      }

// determine location of earthquake epicenter
// original point location
//      lonEq1=catn->lon[cnt];
//      latEq1=catn->lat[cnt];
      if ( log_actual_eq_loc==1 ) {
        lonEq=catn->lon[cnt];
        latEq=catn->lat[cnt];
      }
      else {
// epicenters determined from cell origin
        if ( log_shift_eq==1 ) {
          lonEq=reg->minlon+(ix1+0.5)*reg->dlon;
          latEq=reg->maxlat-(iy1+0.5)*reg->dlat;
        }
// epicenter at center of cell - shifted 0.5 increment
        else {
          lonEq=reg->minlon+(ix1)*reg->dlon;
          latEq=reg->maxlat-(iy1)*reg->dlat;
        }
      }

// optional test of earthquake locations
/*      delaz_(&latEq,&lonEq,&latEq1,&lonEq1,&dist,&az,&baz);
      fprintf(stderr,"EqDist %.2f %.2f %.2f %.2f %.2f %d %d\n", dist, lonEq1, latEq1, lonEq, latEq, ix1, iy1);
      exit(1); 	*/	


// loop over grid points and assign numbers
// get normalization term, first
      norm_term=0.0;
      kernel_sum=0.0;
      sigma=get_sigma_value(smp, catn, cnt);
      sig2=2*sigma*sigma;
      for(ix=0; ix<reg->nsx; ix++) {
        for(iy=0; iy<reg->nsy; iy++) {
          n=iy*reg->nsx+ix;
// shift cell origin to center cell
          if ( log_shift_pt==1 ) {
            lonPt=reg->minlon+(ix+0.5)*reg->dlon;
            latPt=reg->maxlat-(iy+0.5)*reg->dlat;
          }
// original location
          else {
            lonPt=reg->minlon+(ix)*reg->dlon;
            latPt=reg->maxlat-(iy)*reg->dlat;
          }
// distances from earthquakes to cells
          delaz_(&latEq,&lonEq,&latPt,&lonPt,&dist,&az,&baz);
          dist2=dist*dist;
          area_scaling=cos(latPt*DEG2RAD)/cos(latEq*DEG2RAD);
          if ( (smp->gauss_kern) == 1 ) {
            fac_exp=expf(-1*dist2/sig2);
          }
          else {
            fac_exp=powf((dist2+sigma*sigma),-1.5);
          }
          kernel_sum=kernel_sum+fac_exp*area_scaling;
//          if ( fac_exp > VTHR ) {
//            norm_term=norm_term+fac_exp;
//          }
//  if (n==29375) fprintf(stderr,"wt_norm: %.2f %.2f %.4e %d %d %.2f %.2f\n", dist, sigma, fac_exp, ix, iy, lonPt, latPt);
//          fprintf(stderr,"area-scaling/latPt/latEq: %.2f %.2f %.2f\n", area_scaling, latPt, latEq);
        }
      }
      norm_term=1/kernel_sum;
//fprintf(stderr,"wt_norm: %.4f %.2f %d\n", norm_term, sigma, cnt);
//      fprintf(stderr,"norm term: %.2f %.2f %d %.4f %.2f\n", catn->lon[cnt], catn->lat[cnt], cnt, norm_term, sigma);
/*
if (norm_term>1) {
  fprintf(stderr,"error norm term: %f\n", norm_term); exit(1);
}
else {
  fprintf(stderr,"norm term: %.2f %.2f %d %.2f %.2f\n", catn->lon[cnt], catn->lat[cnt], cnt, norm_term, sigma);
}
*/
// assign normalized weights 
      float eq_sum=0.0;
      for(ix=0; ix<reg->nsx; ix++) {
        for(iy=0; iy<reg->nsy; iy++) {
          n=iy*reg->nsx+ix;
// original location
          if ( log_shift_pt==0 ) {
            lonPt=reg->minlon+(ix)*reg->dlon;
            latPt=reg->maxlat-(iy)*reg->dlat;
          }
          else {
// shift cell origin to center cell
            lonPt=reg->minlon+(ix+0.5)*reg->dlon;
            latPt=reg->maxlat-(iy+0.5)*reg->dlat;
          }
// earthquake location - calculate distances
          delaz_(&latEq,&lonEq,&latPt,&lonPt,&dist,&az,&baz);
          dist2=dist*dist;
          area_scaling=cos(latPt*DEG2RAD)/cos(latEq*DEG2RAD);
          if ( (smp->gauss_kern) == 1 ) {
            fac_exp=n_eff*norm_term*expf(-1*dist2/sig2);
          }
          else {
            fac_exp=n_eff*norm_term*powf((dist2+sigma*sigma),-1.5);
          }
// scaling for variable-cell-area-with-latitude
          if ( cat_comp->correctAreaForLat ) {
//            fprintf(stderr,"correcting for grid cell area\n");
            eq_sum=eq_sum+fac_exp*area_scaling;
            seisr->asum[n]=seisr->asum[n]+fac_exp*area_scaling;
          }
          else {
            eq_sum=eq_sum+fac_exp;
            seisr->asum[n]=seisr->asum[n]+fac_exp;
          }
//          if ( fac_exp > norm_term*VTHR ) {
//fprintf(stderr,"sum_iter(sum/cnt/dist/sig/fac_exp): %.4f %d %.2f %.2f %.4e\n", eq_sum, cnt, dist, sigma, fac_exp);
          
//  if (n==29375) fprintf(stderr,"wt: %.2f %.2f %.4e %.4f %f %d %d n=%d\n", dist, sigma, fac_exp, norm_term, seisr->asum[n], ix, iy, cnt);
        }
      }				// end loops over gridpoints ix/iy
//fprintf(stderr,"sum eq: %.4f %d\n", eq_sum, cnt);
    }				// end loop earthquakes
// get maximum, smoothed rate
    maxv=0.0; imaxv=0;
    for(cnt=0; cnt<seisr->nv; cnt++) {
      if (seisr->asum[cnt]>maxv) {
        maxv=seisr->asum[cnt];
        imaxv=cnt;
      } 
    }
    fprintf(stdout,"Maximum, smoothed rate (eq nos/cell); grid-cell number: %.2f %d\n", maxv, imaxv);
// sum earthquake numbers to ensure that smoothing preserves numbers
    sum_eqs=0.0;
    for(cnt=0; cnt<seisr->nv; cnt++) {
      sum_eqs=sum_eqs+seisr->asum[cnt];
    }
    fprintf(stderr,"Over %d cells, %.2f earthquakes recovered, of %.2f effective events smoothed (%d actual).\n", seisr->nv, sum_eqs, tot_asum, catn->nv);
    fprintf(stderr,"Effective events are annual rates M>=0 (%.2f M>=3.0)\n", sum_eqs*powf(10,-cat_par->bval*3.0));
  } 				// end smoothing option


}

