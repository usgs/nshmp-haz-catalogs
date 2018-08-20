#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agrid.h"
#include "agrid_functions_fortran.h"

/*--------------------------------------------------------------------------*/
void assign_variable_completeness_eqs(struct eqcat* cat, struct compl_levels* compl_vals)
/*--------------------------------------------------------------------------*/
{
  int cnt, cnt1;
  int year_mindist;
  float loneq, lateq, lonpt, latpt;
  float dist, az, baz, mindist;
 
// loop over all earthquakes and assign the completeness year from closest grid point  
// default to 
  for(cnt=0; cnt<cat->nv; cnt++) {
    cat->year_compl[cnt]=compl_vals->yr_comp[0];
    loneq=cat->lon[cnt];
    lateq=cat->lat[cnt];
    mindist=9999.0;
    for(cnt1=0; cnt1<compl_vals->nVarComp; cnt1++) {
      lonpt=compl_vals->lon_arr[cnt1];
      latpt=compl_vals->lat_arr[cnt1];
      delaz_(&lateq,&loneq,&latpt,&lonpt,&dist,&az,&baz);
      if ( dist<mindist ) {
        mindist=dist;
        year_mindist=compl_vals->yrCompl_arr[cnt1];
      }
    }
    if (mindist<200) cat->year_compl[cnt]=year_mindist;
//    fprintf(stdout,"compl-yr %.2f %.2f %d\n", loneq, lateq, cat->year_compl[cnt]);
  }
  fprintf(stdout,"Assigned variable completeness to earthquakes in catalog.\n");

// remove events that occur before variable completeness year
  cnt1=0;
  for(cnt=0; cnt<cat->nv; cnt++) {
    if ( (cat->year[cnt]) >= (cat->year_compl[cnt]) ) {
      cat->year[cnt1]=cat->year[cnt];
      cat->month[cnt1]=cat->month[cnt];
      cat->day[cnt1]=cat->day[cnt];
      cat->lineno[cnt1]=cat->lineno[cnt];
      cat->mag[cnt1]=cat->mag[cnt];
      cat->lon[cnt1]=cat->lon[cnt];
      cat->lat[cnt1]=cat->lat[cnt];
      cat->depth[cnt1]=cat->depth[cnt];
      cat->neff[cnt1]=cat->neff[cnt];
      cat->mag_sigma[cnt1]=cat->mag_sigma[cnt];
      cat->year_compl[cnt1]=cat->year_compl[cnt];
      fprintf(stderr,"%d %d %d %f\n", cat->year[cnt1], cat->month[cnt1], cat->day[cnt1], cat->mag[cnt1]);
      cnt1++;
    }
/*    else {
     fprintf(stderr,"Year discrepancy: %d %d, complete\n",cat->year[cnt], cat->year_compl[cnt]);
    } */
  }
  fprintf(stderr,"Reduced catalog by %d events (%d orig; %d variable-completeness).\n", cat->nv-cnt1, cat->nv, cnt1);
  cat->nv=cnt1;

}


/*--------------------------------------------------------------------------*/
int get_eqs_fm_catalog(char *fnm, struct eqcat* catn, struct compl_levels* compl, struct boundaries* reg, struct rates* seisr)
/*--------------------------------------------------------------------------*/
{
  FILE *fpcat;
  int cnt=0, cnt1, lineno=1;
//  int ix, iy, n;
  int yr, mon, day, hr, min;
  float mag, lon, lat, dep, sec, mag_adj, msig, neff,mconv;
  char buff[BUFFLEN];

// open file
  if( (fpcat = fopen(fnm, "r"))==NULL) {
    fprintf(stderr,"Earthquake catalog file not found, %s\n", fnm);
    return 0;
  }
  
// loop through catalog and select earthquakes that meet magnitude completeness thresholds and
// occur within the region
  fprintf(stderr,"Regional boundaries for eqs: min/max lon, lat: %d %d\n", reg->nsy, reg->nsx);
  while( fgets(buff,BUFFLEN,fpcat) ) {
//    sscanf(buff,"%f %f %f %f %d %d %d %d %d %f %f %f", &mag, &lon, &lat, &dep, &yr, &mon, &day, &hr, &min, &sec, &msig, &neff);
    sscanf(buff,"%f %f %f %f %d %d %d %d %d %f %f %f %f", &mconv, &lon, &lat, &dep, &yr, &mon, &day, &hr, &min, &sec, &msig, &mag, &neff);
    if ( (lon>=reg->minlon) && (lon<=reg->maxlon) &&  (lat>=reg->minlat) && (lat<=reg->maxlat) ) {
      for(cnt1=0; cnt1<compl->nlevels; cnt1++) {
//        fprintf(stderr,"mag/yr completeness level: %f %d\n", compl->mag_comp[cnt1], compl->yr_comp[cnt1]);
//        fprintf(stderr,"mag/yr completeness level: %f %d %d\n", compl->mag_comp[cnt1], compl->yr_comp[cnt1], compl->end_yr);
        if (compl->nlevels>1) fprintf(stderr,"There may be a problem with selection of earthquakes from catalog for multiple completeness levels.\n");
        mag_adj=compl->mag_comp[cnt1]-compl->dMag/2;
        if ( (mag>=mag_adj) && (yr>=compl->yr_comp[cnt1]) && (yr<=compl->end_yr) ) {
          catn->mag[cnt]=mag;
          catn->lon[cnt]=lon;
          catn->lat[cnt]=lat;
          catn->depth[cnt]=dep;
          catn->year[cnt]=yr;
          catn->month[cnt]=mon;
          catn->day[cnt]=day;
          catn->lineno[cnt]=lineno;
          catn->mag_sigma[cnt]=msig;
          catn->neff[cnt]=neff;
//          fprintf(stderr,"sel: %f %f %f %d %d %d %d\n", mag, lon, lat, yr, mon, day, lineno);
//          fprintf(stderr,"sel: %f %f %f %d %d %d %d\n", catn->mag[cnt], catn->lon[cnt], catn->lat[cnt], catn->year[cnt], catn->month[cnt], catn->day[cnt], catn->lineno[cnt]);
//          iy=(int)((reg->maxlat-lat)/reg->dlat);
//          ix=(int)((lon-reg->minlon)/reg->dlon);
//          n=iy*reg->nsx+ix+1;
//          seisr->asum_cnt[n]++;
          cnt++;
        }
      }
    }
    lineno++;
  }
  catn->nv=cnt;
  fclose(fpcat);

// check earthquake numbers fit within arrays
  if (catn->nv>NEQ) {
    fprintf(stderr,"Number of earthquakes in catalog (%d) - %s - greater than array size, NEQ (%d).", catn->nv, fnm, (int)NEQ);
    fprintf(stderr,"Modify NEQ and re-start code.\n");
    exit(1);
  }
  else {
    fprintf(stderr,"Selected %d earthquakes from catalog, %s (%d events, total)\n", catn->nv, fnm, lineno);
  }
  
  if ( (compl->variableCompleteness) == 1 ) {
    if ( (compl->nlevels) > 1 ) {
      fprintf(stderr,"ERROR: Code doesn't currently allow for variable completeness and multiple completeness levels.\n");
      exit(1);
    }
    assign_variable_completeness_eqs(catn, compl);
  }

  return 1;
}

