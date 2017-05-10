#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agrid.h"
#include "agrid_functions.h"

/*--------------------------------------------------------------------------*/
void load_poly_data_years(struct compl_levels* cat_comp, int end_yr_catalog)
/*--------------------------------------------------------------------------*/
{
   FILE *fplst, *fppoly;
   int cnt, cnt1, cnt2, nheaders, years_gap, year1, year2;
   int yr_comp;
   int ygap1, ygap2;
   char buff[BUFFLEN], nm_tmp[200];

   if( (fplst = fopen(cat_comp->catalogGapsFile, "r"))==NULL) {
     fprintf(stderr,"Cannot open file with list of polygons, %s\n", cat_comp->catalogGapsFile);
     exit(1);
   }
   fprintf(stderr,"Opened file with list of polygons, %s\n", cat_comp->catalogGapsFile);
// loop through list of all polygon files
   cnt=0;
   while(fgets(buff,BUFFLEN,fplst) ) {
     sscanf(buff,"%s %d %d %d %d", nm_tmp, &years_gap, &nheaders, &year1, &year2);
//     cat_comp->polygons.elem[cnt].nyears=years_gap;
     cat_comp->polygons.elem[cnt].year_start=year1;
     cat_comp->polygons.elem[cnt].year_end=year2;
// re-calculated years_gap parameter     
     if (cat_comp->nlevels == 1) {
//       fprintf(stderr,"Modifying catalog gap calculation...\n");
       yr_comp=cat_comp->yr_comp[0];
// end catalog before catalog gap
       if ( end_yr_catalog < year1 ) { 
         cat_comp->polygons.elem[cnt].nyears=0.0;
//         fprintf(stderr,"end before yr1\n");
         ygap1=1; ygap2=0;
       }
// completeness after catalog gap
       else if ( yr_comp > year2 ) { 
         cat_comp->polygons.elem[cnt].nyears=0.0;
//         fprintf(stderr,"completeness after gap\n");
         ygap1=1; ygap2=0;
       }
       else {
// some lost time from catalog
// starting time
         if ( yr_comp > year1 ) { 
           ygap1=yr_comp;
//           fprintf(stderr,"y1a ");
         }
         else {
           ygap1=year1;
//           fprintf(stderr,"y1b ");
         }
// ending time
         if ( end_yr_catalog > year2 ) { 
           ygap2=year2;
//           fprintf(stderr,"y2a\n");
         }
         else {
           ygap2=end_yr_catalog;
//           fprintf(stderr,"y2b\n");
         }
         cat_comp->polygons.elem[cnt].nyears=ygap2-ygap1+1;
       }
//       fprintf(stderr,"cat gap %d - %d compl: %d end_cat %d gap: %d (%d %d)\n", year1, year2, yr_comp, end_yr_catalog, cat_comp->polygons.elem[cnt].nyears, ygap1, ygap2);
       fprintf(stderr,"Catalog gap %d y, %s (%d-%d)\n", cat_comp->polygons.elem[cnt].nyears, nm_tmp, year1, year2);
     }
     else {
       fprintf(stderr,"Cannot modify catalog gap calculation with multiple levels of completeness.\n");
     }
     if( (fppoly  = fopen(nm_tmp, "r"))==NULL) {
       fprintf(stderr,"Cannot open polygon file, %s\n", nm_tmp);
       exit(1);
     }
     for(cnt1=0; cnt1<nheaders; cnt1++) fgets(buff,BUFFLEN,fppoly);
     cnt2=0;
     while(fgets(buff,BUFFLEN,fppoly) ) {
       sscanf(buff,"%f %f", &(cat_comp->polygons.elem[cnt].lon[cnt2]), &(cat_comp->polygons.elem[cnt].lat[cnt2]) );
       cnt2++;
     }
     cat_comp->polygons.elem[cnt].npts=cnt2;
     fclose(fppoly);
     cnt++;
   }
   cat_comp->polygons.npoly=cnt;
   fprintf(stderr,"Read %d polygons.\n", cat_comp->polygons.npoly);
}

/*--------------------------------------------------------------------------*/
void load_poly_data(struct compl_levels* cat_comp)
/*--------------------------------------------------------------------------*/
{
   FILE *fplst, *fppoly;
   int cnt, cnt1, cnt2, nheaders, years_gap;
   char buff[BUFFLEN], nm_tmp[200];

   if( (fplst = fopen(cat_comp->catalogGapsFile, "r"))==NULL) {
     fprintf(stderr,"Cannot open file with list of polygons, %s\n", cat_comp->catalogGapsFile);
     exit(1);
   }
   fprintf(stderr,"Opened file with list of polygons, %s\n", cat_comp->catalogGapsFile);
// loop through list of all polygon files
   cnt=0;
   while(fgets(buff,BUFFLEN,fplst) ) {
     sscanf(buff,"%s %d %d", nm_tmp, &years_gap, &nheaders);
     if( (fppoly  = fopen(nm_tmp, "r"))==NULL) {
       fprintf(stderr,"Cannot open polygon file, %s\n", nm_tmp);
       exit(1);
     }
     for(cnt1=0; cnt1<nheaders; cnt1++) fgets(buff,BUFFLEN,fppoly);
     cat_comp->polygons.elem[cnt].nyears=years_gap;
     cnt2=0;
     while(fgets(buff,BUFFLEN,fppoly) ) {
       sscanf(buff,"%f %f", &(cat_comp->polygons.elem[cnt].lon[cnt2]), &(cat_comp->polygons.elem[cnt].lat[cnt2]) );
       cnt2++;
     }
     cat_comp->polygons.elem[cnt].npts=cnt2;
     fclose(fppoly);
     cnt++;
   }
   cat_comp->polygons.npoly=cnt;
   fprintf(stderr,"Read %d polygons.\n", cat_comp->polygons.npoly);
}

/*--------------------------------------------------------------------------*/
int read_params_new_old(char *parf, struct compl_levels* cat_comp, struct boundaries* reg, 
  struct catalog* cat, struct smoothp* smp, char *outfnm, int new_style_formatting) 
/*--------------------------------------------------------------------------*/
{
  int retVal=0;

// read parameters
  if ( new_style_formatting ) {
//    retVal=read_params_new(parf, &cat_compl, &region, &cat, &smoothing_params, outf);
    retVal=read_params_new(parf, cat_comp, reg, cat, smp, outfnm);
    if ( (cat_comp->variableCompleteness) == 1 ) read_completenessVals(cat_comp);
    if ( ! retVal ) fprintf(stderr, "\nError reading parameter file, %s with newer-style formatting\n", outfnm); 
  }
  else {
//    retVal=read_params(parf, &cat_comp, &reg, &cat, &smp, outfnm);
    retVal=read_params(parf, cat_comp, reg, cat, smp, outfnm);
    if ( ! retVal ) fprintf(stderr, "\nError reading parameter file, %s with older-style formatting\n", outfnm); 
    fprintf(stderr,"Old-style formatting for parameter input is deprecated and is no longer supported. The option remains in the code for legacy comparisons but requires to user to modify the code to remove the exit statment in the read_write_params.c function.\n");
    exit(1);
  }
  cat_comp->end_yr=cat->end_yr;
  cat_comp->dMag=cat->dMag;

  return retVal;
}

/*--------------------------------------------------------------------------*/
void read_completenessVals(struct compl_levels* cat_comp)
/*--------------------------------------------------------------------------*/
{
  FILE *fp;
  int cnt=0;
  char buff[BUFFLEN];

  if( (fp = fopen(cat_comp->completenessFile, "r"))==NULL) {
    fprintf(stderr,"File with variable completeness values (lon-lat-year) not found, %s\n", cat_comp->completenessFile);
    exit(1);
  }
  while( fgets(buff,BUFFLEN,fp) ) {
    sscanf(buff, "%f %f %d", &(cat_comp->lon_arr[cnt]), &(cat_comp->lat_arr[cnt]), &(cat_comp->yrCompl_arr[cnt]) );
    cnt++;
  }
  fclose(fp);

//
  cat_comp->nVarComp=cnt;
  fprintf(stderr,"Read variable completeness values from file, %s, with %d points\n", cat_comp->completenessFile, cat_comp->nVarComp);

}


/*--------------------------------------------------------------------------*/
int read_params(char *parf, struct compl_levels* cat_comp, struct boundaries* reg, 
  struct catalog* cat, struct smoothp* smp, char *outfnm) 
/*--------------------------------------------------------------------------*/
{
  FILE *fpparam;
  int cnt;
  char buff[BUFFLEN], gauss_plaw[10];
  
// open file
  if( (fpparam = fopen(parf, "r"))==NULL) {
    fprintf(stderr,"Parameter file not found, %s\n", parf);
    return 0;
  }
  fprintf(stderr,"Reading from parameter file, %s\n", parf);

// read parameters
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%f %f", &reg->minlat, &reg->maxlat);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%f %f", &reg->minlon, &reg->maxlon);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%f %f", &reg->dlat, &reg->dlon);
// completeness levels
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%d", &cat_comp->nlevels);
  for (cnt=0; cnt<cat_comp->nlevels; cnt++) {
    fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%f %d", &cat_comp->mag_comp[cnt], &cat_comp->yr_comp[cnt]);
  }
// catalog end year, name, and min-magnitude/d-mag/b-value
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%d", &cat->end_yr);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%s", cat->catnm);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%f %f %f", &cat->minMag, &cat->dMag, &cat->bval);
// smoothing parameters
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%d", &smp->cumul_rates);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%d", &smp->adj_compl);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%d", &smp->apply_sm);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%d", &smp->adapt_sm);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%f %f", &smp->sigmin, &smp->sigmax);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%d", &smp->nnum);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%s", outfnm);
  fgets(buff,BUFFLEN,fpparam); sscanf(buff,"%s", gauss_plaw);
  if ( STREQ(gauss_plaw,"gauss") ) {
    smp->gauss_kern=1;
  }
  else if ( STREQ(gauss_plaw,"powerLaw") ) {
    smp->gauss_kern=0;
  }
  else {
    fprintf(stderr,"No kernel option for %s. User must enter gauss or powerLaw for kernel-type.\n", gauss_plaw);
    exit(1);
  }
//  fscanf(fpparam,"%d", &smp->nnum);
//  fscanf(fpparam,"%s", outfnm);
// close file
  fclose(fpparam);

// force to use effective-numbers of earthquakes
  smp->use_neff=1;
  fprintf(stderr,"Using effective numbers of earthquakes.\n");

// currently set fixed-R to sigmax value
  smp->sigfix=smp->sigmax;
  fprintf(stderr,"sigmax: %f\n", smp->sigmax);
  fprintf(stderr,"nnum : %d\n", smp->nnum);
  fprintf(stderr,"outname: %s\n", outfnm);

// get numbers of points in x-, y-directions
  reg->nsx=(int)( (reg->maxlon-reg->minlon)/reg->dlon +1);
  reg->nsy=(int)( (reg->maxlat-reg->minlat)/reg->dlat +1 );
  reg->nsrc=reg->nsx*reg->nsy;

  return 1;
}

/*--------------------------------------------------------------------------*/
int read_params_new(char *parf, struct compl_levels* cat_comp, struct boundaries* reg, 
  struct catalog* cat, struct smoothp* smp, char *outfnm) 
/*--------------------------------------------------------------------------*/
{
  FILE *fpparam;
  int cnt;
//  char buff[BUFFLEN];
  char parseaux[256], gauss_plaw[10];
  
// open file
  if( (fpparam = fopen(parf, "r"))==NULL) {
    fprintf(stderr,"Parameter file not found, %s\n", parf);
    return 0;
  }
  fprintf(stderr,"Reading from parameter file, %s - new formatting style.\n", parf);

// read parameters
  Parse_Text(fpparam, "Min_Lat",'f', &(reg->minlat) );
  Parse_Text(fpparam, "Max_Lat",'f', &(reg->maxlat) );
  Parse_Text(fpparam, "Min_Lon",'f', &(reg->minlon) );
  Parse_Text(fpparam, "Max_Lon",'f', &(reg->maxlon) );
  Parse_Text(fpparam, "Inc_Lat",'f', &(reg->dlat) );
  Parse_Text(fpparam, "Inc_Lon",'f', &(reg->dlon) );
  Parse_Text(fpparam, "Correct_Area_for_Latitude",'i', &(cat_comp->correctAreaForLat) );
  Parse_Text(fpparam, "Variable_Completeness",'i', &(cat_comp->variableCompleteness) );
  if ( (cat_comp->variableCompleteness) == 1 ) {
    Parse_Text(fpparam, "Completeness_File",'s', &(cat_comp->completenessFile) );
  }
// Completeness
  Parse_Text(fpparam, "N_Completeness_Levels",'i', &(cat_comp->nlevels) );
  for (cnt=0; cnt<cat_comp->nlevels; cnt++) {
    sprintf(parseaux,"Completeness_Level_yr_%d", cnt+1);
    Parse_Text(fpparam,parseaux,'i', &(cat_comp->yr_comp[cnt]) );
    sprintf(parseaux,"Completeness_Level_M_%d", cnt+1);
    Parse_Text(fpparam,parseaux,'f', &(cat_comp->mag_comp[cnt]) );
  }
// variable Mc
  Parse_Text(fpparam, "logical_Variable_Mc",'i', &(cat_comp->variableMc) );
  if ( (cat_comp->variableMc) == 1 ) {
    Parse_Text(fpparam, "Variable_Mc_File",'s', cat_comp->variableMc_File );
  }
// End of catalog
  Parse_Text(fpparam, "End_yr_Catalog",'i', &(cat->end_yr) );
// Gaps in catalog for CEUS induced seismicity
  Parse_Text(fpparam, "Catalog_Gaps",'i', &(cat_comp->catalogGaps) );
  if ( (cat_comp->catalogGaps) == 1 ) {
    Parse_Text(fpparam, "Catalog_Gaps_File",'s', &(cat_comp->catalogGapsFile) );
//    load_poly_data(cat_comp);
    load_poly_data_years(cat_comp, cat->end_yr);
  }
  Parse_Text(fpparam, "Catalog_Name",'s', &(cat->catnm) );
  Parse_Text(fpparam, "Output_Name",'s', outfnm);
  Parse_Text(fpparam, "Min_Magnitude",'f', &(cat->minMag) );
  Parse_Text(fpparam, "b_Value",'f', &(cat->bval) );
  Parse_Text(fpparam, "dMag_value",'f', &(cat->dMag) );
// logical values
  Parse_Text(fpparam, "logical_Effective_Num_Eqs",'i', &(smp->use_neff) );
  Parse_Text(fpparam, "logical_Cumulative_rates",'i', &(smp->cumul_rates) );
  Parse_Text(fpparam, "logical_Apply_smoothing",'i', &(smp->apply_sm) );
  if ( (smp->apply_sm) == 1 ) {
    Parse_Text(fpparam, "logical_Adjust_completeness",'i', &(smp->adj_compl) );
    Parse_Text(fpparam, "Gauss_PowerLaw_Kernel",'s', gauss_plaw);
    if ( STREQ(gauss_plaw,"gauss") ) {
      smp->gauss_kern=1;
    }
    else if ( STREQ(gauss_plaw,"powerLaw") ) {
      smp->gauss_kern=0;
    }
    else {
      fprintf(stderr,"No kernel option for %s. User must enter gauss or powerLaw for kernel-type.\n", gauss_plaw);
      exit(1);
    }
    Parse_Text(fpparam, "logical_Adaptive_smoothing",'i', &(smp->adapt_sm) );
    if ( (smp->adapt_sm) ) {
      Parse_Text(fpparam, "Adaptive_Neighbor_Number",'i', &(smp->nnum) );
      Parse_Text(fpparam, "Sigma_Min",'f', &(smp->sigmin) );
      Parse_Text(fpparam, "Sigma_Max",'f', &(smp->sigmax) );
      Parse_Text(fpparam, "logical_Avg_Neighbor",'i', &(smp->nnum2_log) );
      if (smp->nnum2_log) {
        Parse_Text(fpparam, "Adaptive_Neighbor_Number_2",'i', &(smp->nnum2) );
      }
    }
    else {
      Parse_Text(fpparam, "Sigma_Fix",'f', &(smp->sigfix) );
    }
  }
  fclose(fpparam);

// get numbers of points in x-, y-directions
  reg->nsx=(int)( (reg->maxlon-reg->minlon)/reg->dlon +1);
  reg->nsy=(int)( (reg->maxlat-reg->minlat)/reg->dlat +1 );
  reg->nsrc=reg->nsx*reg->nsy;

  return 1;

}

/*--------------------------------------------------------------------------*/
void write_params(struct compl_levels* cat_comp, struct boundaries* reg, 
  struct catalog* cat, struct smoothp* smp1, char *outfnm) 
/*--------------------------------------------------------------------------*/
{
  int cnt;
  
// write out parameters
  fprintf(stdout,"Latitudes: %.2f %.2f\n", reg->minlat, reg->maxlat);
  fprintf(stdout,"Longitudes: %.2f %.2f\n", reg->minlon, reg->maxlon);
  fprintf(stdout,"dlat, dlon: %.2f %.2f\n", reg->dlat, reg->dlon);
// completeness levels
  fprintf(stdout,"Number of magnitude completeness levels: %d\n  M yr\n", cat_comp->nlevels);
  for (cnt=0; cnt<cat_comp->nlevels; cnt++) {
    fprintf(stdout,"  %.2f %d\n", cat_comp->mag_comp[cnt], cat_comp->yr_comp[cnt]);
  }
// catalog end year, name, and min-magnitude/d-mag/b-value
  fprintf(stdout,"Last year of catalog: %d\n", cat->end_yr);
  fprintf(stdout,"Name of catalog file: %s\n", cat->catnm);
  fprintf(stdout,"Minimum-M/dM/b-value: %.2f %.2f %.3f\n", cat->minMag, cat->dMag, cat->bval);
  if ( cat_comp->correctAreaForLat ) {
    fprintf(stdout,"Correcting grid cell area for latitude.\n");
  }
  else {
    fprintf(stdout,"Not correcting grid cell area for latitude.\n");
  }
// effective/actual numbers earthquakes
  if (smp1->use_neff) {
    fprintf(stdout,"Using effective numbers of earthquakes: N>=1.\n");
  }
  else {
    fprintf(stdout,"Using actual numbers of earthquakes (not effective-numbers): N=1.\n");
  }
// smoothing parameters
  if (smp1->cumul_rates) {
    fprintf(stdout,"Output cumulative rates.\n");
  }
  else {
    fprintf(stdout,"Output incremental rates.\n");
  }
  if (smp1->adj_compl) {
    fprintf(stdout,"Adjust completeness levels.\n");
  }
  else {
    fprintf(stdout,"Don't adjust completeness levels.\n");
  }
  if (smp1->apply_sm) {
    fprintf(stdout,"Apply smoothing to seismicity rates... ");
// gauss/power-law
    if ( smp1->gauss_kern ) {
      fprintf(stdout,"  gaussian kernel ... ");
    }
    else {
      fprintf(stdout,"  power-law kernel ... ");
    }
    if (smp1->adapt_sm) {
      fprintf(stdout," adaptive smoothing - ");
      if (smp1->nnum2_log == 0) {
        fprintf(stdout,"neighbor number %d\n", smp1->nnum);
      }
      else {
        fprintf(stdout,"average of neighbor numbers %d, %d\n", smp1->nnum, smp1->nnum2);
      }
    }
    else {
      fprintf(stdout," fixed-radius smoothing: %.1f km\n", smp1->sigfix);
    }
  }
  else {
    fprintf(stdout,"Don't apply smoothing to seismicity rates.\n");
  }
  fprintf(stdout,"Seismicity rates (10^a) written to files, %s[_M0/3/4/5.out]\n", outfnm); 
}



/*--------------------------------------------------------------------------*/
void write_params_to_file(char *fnm, struct compl_levels* cat_comp, struct boundaries* reg, 
  struct catalog* cat, struct smoothp* smp1, char *outfnm) 
/*--------------------------------------------------------------------------*/
{
  FILE *fpout;
  int cnt;
  
// open summary file for writing
  fpout=fopen(fnm,"w");
  fprintf(fpout,"----------------------------------------------------------\n");
  fprintf(fpout,"---------  Summary of processing steps -------------------\n");
  fprintf(fpout,"----------------------------------------------------------\n\n");
  
// write out parameters
  fprintf(fpout,"Latitudes: %.2f %.2f\n", reg->minlat, reg->maxlat);
  fprintf(fpout,"Longitudes: %.2f %.2f\n", reg->minlon, reg->maxlon);
  fprintf(fpout,"dlat, dlon: %.2f %.2f\n", reg->dlat, reg->dlon);
// completeness levels
  fprintf(fpout,"Number of magnitude completeness levels: %d\n  M yr\n", cat_comp->nlevels);
  for (cnt=0; cnt<cat_comp->nlevels; cnt++) {
    fprintf(fpout,"  %.2f %d\n", cat_comp->mag_comp[cnt], cat_comp->yr_comp[cnt]);
  }
// catalog end year, name, and min-magnitude/d-mag/b-value
  fprintf(fpout,"Last year of catalog: %d\n", cat->end_yr);
  fprintf(fpout,"Name of catalog file: %s\n", cat->catnm);
  fprintf(fpout,"Minimum-M/dM/b-value: %.2f %.2f %.3f\n", cat->minMag, cat->dMag, cat->bval);
// effective/actual numbers earthquakes
  if (smp1->use_neff) {
    fprintf(fpout,"Using effective numbers of earthquakes: N>=1.\n");
  }
  else {
    fprintf(fpout,"Using actual numbers of earthquakes (not effective-numbers): N=1.\n");
  }
// smoothing parameters
  if (smp1->cumul_rates) {
    fprintf(fpout,"Output cumulative rates.\n");
  }
  else {
    fprintf(fpout,"Output incremental rates.\n");
  }
  if (smp1->adj_compl) {
    fprintf(fpout,"Adjust completeness levels.\n");
  }
  else {
    fprintf(fpout,"Don't adjust completeness levels.\n");
  }
  if (smp1->apply_sm) {
    fprintf(fpout,"Apply smoothing to seismicity rates... ");
// gauss/power-law
    if ( smp1->gauss_kern ) {
      fprintf(fpout,"  gaussian kernel ... ");
    }
    else {
      fprintf(fpout,"  power-law kernel ... ");
    }
    if (smp1->adapt_sm) {
      fprintf(fpout," adaptive smoothing - ");
      if (smp1->nnum2_log == 0) {
        fprintf(fpout,"neighbor number %d\n", smp1->nnum);
      }
      else {
        fprintf(fpout,"average of neighbor numbers %d, %d\n", smp1->nnum, smp1->nnum2);
      }
    }
    else {
      fprintf(fpout,"  fixed-radius smoothing: %.1f km\n", smp1->sigfix);
    }
  }
  else {
    fprintf(fpout,"Don't apply smoothing to seismicity rates.\n");
  }
  fprintf(fpout,"Seismicity rates (10^a) written to files, %s[_M0/3/4/5.out]\n", outfnm); 
  
  fclose(fpout);
}

