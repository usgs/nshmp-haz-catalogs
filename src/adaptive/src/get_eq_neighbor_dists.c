#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agrid.h"
#include "agrid_functions_fortran.h"

// comparator function for floats
int compare_floats ( const void * a, const void * b) {
//  return ( *(int*)a - *(int*)b );
  return ( *(float*)a - *(float*)b );
}


/*--------------------------------------------------------------------------*/
void get_eq_neighbor_dists(struct eqcat* catn)
/*--------------------------------------------------------------------------*/
{
  int cnt, cnt1, cnt2, ncnt, narr;
//  int ord[NEQ];
  float lon1, lat1, lon2, lat2, dist, az, baz;
  float distarr[NEQ];

// sorting array has one less element than earthquake list array
  narr=catn->nv-1;
  for(cnt=0; cnt<NEQ; cnt++) distarr[cnt]=0.0;

// loop over all pairs of stations, get distances and sort for the 10-nearest events
//  fprintf(stderr,"nv=%d\n", catn->nv);
  for(cnt1=0; cnt1<catn->nv; cnt1++) {
    lon1=catn->lon[cnt1];
    lat1=catn->lat[cnt1];
    ncnt=0;
    for(cnt2=0; cnt2<catn->nv; cnt2++) {
      lon2=catn->lon[cnt2];
      lat2=catn->lat[cnt2];
      if ( (cnt1>cnt2) || (cnt1<cnt2) ) {
        delaz_(&lat1,&lon1,&lat2,&lon2,&dist,&az,&baz);
        distarr[ncnt]=dist;
//        fprintf(stderr,"dist/coords: %f %.2f %.2f %.2f %.2f %d %d\n", dist, lon1, lat1, lon2, lat2, cnt1, cnt2);
        ncnt++;
      }
    }
//    for(cnt2=0; cnt2<ncnt; cnt2++) fprintf(stderr,"dist: %f\n", distarr[cnt2]);

// sort distances in distarr array
    qsort(distarr,ncnt,sizeof(float),compare_floats);
//    for(cnt2=0; cnt2<ncnt; cnt2++) fprintf(stderr,"dist_sort: %f\n", distarr[cnt2]);

    for(cnt=0;cnt<(int)NNUM; cnt++) {
      catn->dist_nbr[cnt1][cnt]=distarr[cnt];
//      fprintf(stderr,"dist_nbr: %f %f\n", distarr[cnt], catn->dist_nbr[cnt1][cnt]);
    }
//    fprintf(stderr,"REMOVE LATER\n"); exit(1);
    for(cnt=0; cnt<NEQ; cnt++) distarr[cnt]=0.0;
  }

}

