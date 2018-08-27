#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "agrid.h"

/*--------------------------------------------------------------------------*/
void write_asum_to_file(struct rates* seisr, char *fbasenm, struct catalog * catv, float mag)
/*--------------------------------------------------------------------------*/
{ 
  FILE *fpout;
//  int nsrc, cnt, ierr, out2=0;
  int nsrc, cnt, ierr;
//  int nsrc, cnt;
  float diffth=1e-1;
  float asum[NV];
  char fileout[200];
//  struct header headr;

// get rate array and naming based on magnitude input values
  nsrc=seisr->nv;
// Magnitudes
  if ( fabsf(mag-0.0)<diffth ) {
    sprintf(fileout,"%s_M0.out", fbasenm);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt];
  }
  else if ( fabsf(mag-2.5)<diffth ) {
    sprintf(fileout,"%s_M2p5.out", fbasenm);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt]*powf(10,-(catv->bval)*mag);
  }
  else if ( fabsf(mag-2.7)<diffth ) {
    sprintf(fileout,"%s_M2p7.out", fbasenm);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt]*powf(10,-(catv->bval)*mag);
  }
  else if ( fabsf(mag-3.0)<diffth ) {
    sprintf(fileout,"%s_M3.out", fbasenm);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt]*powf(10,-(catv->bval)*mag);
  }
  else if ( fabsf(mag-3.5)<diffth ) {
    sprintf(fileout,"%s_M3p5.out", fbasenm);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt]*powf(10,-(catv->bval)*mag);
  }
  else if ( fabsf(mag-4.0)<diffth ) {
    sprintf(fileout,"%s_M4.out", fbasenm);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt]*powf(10,-(catv->bval)*mag);
  }
  else if ( fabsf(mag-4.7)<diffth ) {
    sprintf(fileout,"%s_M4p7.out", fbasenm);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt]*powf(10,-(catv->bval)*mag);
  }
  else if ( fabsf(mag-4.5)<diffth ) {
    sprintf(fileout,"%s_M4p5.out", fbasenm);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt]*powf(10,-(catv->bval)*mag);
  }
  else if ( fabsf(mag-5.0)<diffth ) {
    sprintf(fileout,"%s_M5.out", fbasenm);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt]*powf(10,-(catv->bval)*mag);
  }
  else {
//    out2=1;
    sprintf(fileout,"%s_M0.out", fbasenm);
    fprintf(stderr,"No option for M%.2f printing, writing to M0 file - %s\n", mag, fileout);
    for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum[cnt];
  }


// write results to file, fileout
  fpout=fopen(fileout,"wb+");
  ierr=fwrite(asum,4,nsrc,fpout);
  fclose(fpout);
//  fprintf(stderr,"Wrote M%.2f results to file, %s.\n", mag, fileout);
  

}

/*--------------------------------------------------------------------------*/  
void write_asum_to_csvfile(
    struct rates *seisr,
    char *fbasenm,
    struct catalog *catv,
    struct boundaries *boundR,
    float mag)
/*--------------------------------------------------------------------------*/  
{
  FILE *fpout;
  FILE *foutAscii;
  int nsrc, cnt;
  int iy, ix;
  float diffth=1e-1;
  char fileout[200];
  char fileoutAscii[200];
                                                                                
  // compute lon/lat array
  // get rate array and naming based on magnitude input values
  nsrc=seisr->nv;
  
  // Magnitudes
  if ( fabsf(mag-0.0)<diffth ) {
    sprintf(fileout,"%s_M0.csv", fbasenm);
    sprintf(fileoutAscii,"%s_M0.asc", fbasenm);

    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileout);
    fprintf(stderr,"M%.2f, writing to file - %s\n", mag, fileoutAscii);

    fpout=fopen(fileout,"w");
    foutAscii = fopen(fileoutAscii, "w");

    for(cnt=0; cnt<nsrc; cnt++) {
      iy=cnt/boundR->nsx;
      ix=cnt-iy*boundR->nsx;
      fprintf(
        fpout,
        "%9.3f, %8.3f, GR, %12.6e \n",
        boundR->minlon+ix*boundR->dlon,
        boundR->maxlat-iy*boundR->dlat,
        seisr->asum[cnt]);

      fprintf(
        foutAscii,
        "%9.3f %8.3f %12.6e \n",
        boundR->minlon+ix*boundR->dlon,
        boundR->maxlat-iy*boundR->dlat,
        seisr->asum[cnt]);
    }

    fclose(fpout);
    fclose(foutAscii);
  } else {
    fprintf(stderr, "No csvfile write option for M~=0\n");
  }

}

/*--------------------------------------------------------------------------*/
void write_unsmoothed_asum_to_file(struct rates* seisr, char *fbasenm)
/*--------------------------------------------------------------------------*/
{ 
  FILE *fpout;
//  int nsrc, cnt, ierr;
  int nsrc, cnt, ierr;
  float asum[NV];
  char fileout[200];
//  struct header headr;


// get rate array and naming based on magnitude input values
  nsrc=seisr->nv;
// no-smoothing
  sprintf(fileout,"%s_noSmooth.out", fbasenm);
  for(cnt=0; cnt<nsrc; cnt++) asum[cnt]=seisr->asum_noSm[cnt];

// write results to file, fileout
  fpout=fopen(fileout,"wb+");
  ierr=fwrite(asum,4,nsrc,fpout);
  fclose(fpout);
  fprintf(stderr,"Wrote un-smoothed results to file, %s.\n", fileout);
  

}


