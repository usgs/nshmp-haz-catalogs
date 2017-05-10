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
int get_variableMc(char *fnm, struct all_poly* polygons, float *mag_out)
/*--------------------------------------------------------------------------*/
{
  FILE *fpMc, *fppoly;
  int cnt=0, cnt1=0, check_input=0;
  char polynm[200];
  char buff[BUFFLEN], buff2[BUFFLEN];

  if( (fpMc = fopen(fnm, "r"))==NULL) {
    fprintf(stderr,"Variable Mc file not found, %s\n", fnm);
    return 0;
  }
  fgets(buff,BUFFLEN,fpMc);
  sscanf(buff,"%f", mag_out);
  while( fgets(buff,BUFFLEN,fpMc) ) {
    sscanf(buff,"%s %f", polynm, &(polygons->elem[cnt].mag_Mc));
//    fprintf(stderr,"Opening file %s with Mc=%.2f\n", polynm, polygons->elem[cnt].mag_Mc);
    if( (fppoly = fopen(polynm, "r"))==NULL) {
      fprintf(stderr,"Polynomial file file not found, %s\n", polynm);
      exit(1);
    }
    cnt1=0;
    while( fgets(buff2,BUFFLEN,fppoly) ) {
      sscanf(buff2,"%f %f", &(polygons->elem[cnt].lon[cnt1]), &(polygons->elem[cnt].lat[cnt1]));
      cnt1++;
    }
    polygons->elem[cnt].npts=cnt1;
    fclose(fppoly);
    cnt++;
    polygons->npoly++;
  }
  fclose(fpMc);

// print output to screen
  if ( check_input ) { 
    for(cnt=0; cnt<polygons->npoly; cnt++){
      fprintf(stderr,"cnt=%d, npts=%d Mc=%.2f\n", cnt, polygons->elem[cnt].npts,polygons->elem[cnt].mag_Mc);
      for(cnt1=0; cnt1<polygons->elem[cnt].npts; cnt1++) {
        fprintf(stderr,"%f %f\n", polygons->elem[cnt].lon[cnt1], polygons->elem[cnt].lat[cnt1]);
      }
    }
  }
 
  return 1;
}

/*--------------------------------------------------------------------------*/
int assign_variableMc(float lon, float lat, struct all_poly* polygons_Mc, float mag_mc_out, float *val_Mc) 
/*--------------------------------------------------------------------------*/
{
  int cnt, cnt1, npts;
  float plon[NPPOLY], plat[NPPOLY];
  float mag=mag_mc_out;

// loop over all polygons
  
  for(cnt=0; cnt<polygons_Mc->npoly; cnt++){
//    fprintf(stderr,"cnt=%d, npts=%d Mc=%.2f\n", cnt, polygons->elem[cnt].npts,polygons->elem[cnt].mag_Mc);
    npts=polygons_Mc->elem[cnt].npts;
    for(cnt1=0; cnt1<polygons_Mc->elem[cnt].npts; cnt1++) {
//      fprintf(stderr,"%f %f\n", polygons->elem[cnt].lon[cnt1], polygons->elem[cnt].lat[cnt1]);
      plon[cnt1]=polygons_Mc->elem[cnt].lon[cnt1];
      plat[cnt1]=polygons_Mc->elem[cnt].lat[cnt1];
    }
    if ( lxyint_(&lon,&lat,plon,plat,&npts) ) { 
//      fprintf(stderr,"Coordinate (%.3f,%.3f) in polygon zc%d\n", lon, lat, cnt+1);
      mag=polygons_Mc->elem[cnt].mag_Mc;
      break;
    }
  }

  *val_Mc=mag;
  return 1;
}


/*--------------------------------------------------------------------------*/
int get_eqs_fm_catalog(char *fnm, struct eqcat* catn, struct compl_levels* compl, struct boundaries* reg, struct rates* seisr)
/*--------------------------------------------------------------------------*/
{
  FILE *fpcat;
  int cnt=0, cnt1, lineno=1;
//  int ix, iy, n;
  int yr, mon, day, hr, min;
  float mag, lon, lat, dep, sec, mag_adj, msig, neff;
  float mag_mc_out, magMc;
  char buff[BUFFLEN];
  struct all_poly polygons_Mc;

// check if there is variable Mc for the catalog
  if ( compl->variableMc ) {
    fprintf(stderr,"Using variable Mc for earthquake catalog.\n");
    fprintf(stderr,"No magnitude adjustment from variable Mc value to account for rounding.\n");
    polygons_Mc.npoly=0;
    if( ! get_variableMc(compl->variableMc_File, &polygons_Mc, &mag_mc_out) ) {
      fprintf(stderr,"Problem assigning variable Mc values.\n");
      exit(1);
    }
  }
  else {
    fprintf(stderr,"Completeness magnitude adusted by %.3f to account for rounding in the earthquake catalog\n", compl->dMag/2);
  }

// open file
  if( (fpcat = fopen(fnm, "r"))==NULL) {
    fprintf(stderr,"Earthquake catalog file not found, %s\n", fnm);
    return 0;
  }
  
// loop through catalog and select earthquakes that meet magnitude completeness thresholds and
// occur within the region
  fprintf(stderr,"Regional boundaries for eqs: min/max lon, lat: %d %d\n", reg->nsy, reg->nsx);
  while( fgets(buff,BUFFLEN,fpcat) ) {
    sscanf(buff,"%f %f %f %f %d %d %d %d %d %f %f %f", &mag, &lon, &lat, &dep, &yr, &mon, &day, &hr, &min, &sec, &msig, &neff);
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
// assign variable Mc from polygons
          if ( compl->variableMc ) {
            if ( ! assign_variableMc(lon,lat, &polygons_Mc, mag_mc_out, &magMc) ) {
              fprintf(stderr,"Problem assigning variable Mc to eq (%.3f, %.3f)\n", lon, lat);
            }
            catn->mag_Mc[cnt]=compl->mag_comp[0];
            if ( magMc > compl->mag_comp[0] ) {
              catn->mag_Mc[cnt]=magMc;
            }
          }
          else {
            catn->mag_Mc[cnt]=mag_adj;
          }
// only retain events that meet Mc criteria
          if ( catn->mag[cnt] >= catn->mag_Mc[cnt] ) {
            cnt++;
          }
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

