#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agrid.h"
#include "agrid_functions_fortran.h"


/*--------------------------------------------------------------------------*/
void write_eq_neighbor_dists(struct eqcat* catn, struct smoothp* smp)
/*--------------------------------------------------------------------------*/
{
  FILE *fpeq;
  int cnt, cnt1;

// open file for writing. add header
  fpeq=fopen("selected_eqs_neighbors.dat","w");
  if ( smp->apply_sm == 1 ) {
    fprintf(fpeq, "Lon Lat Mag Depth Year Month Day agrid-cell-no. distances(km)_to-neighbor:1 ");
    for (cnt=2; cnt<=(int)NNUM; cnt++) fprintf(fpeq, "%d ", cnt);
    fprintf(fpeq, "\n");
  }
  else {
    fprintf(fpeq, "Lon Lat Mag Depth Year Month Day agrid-cell-no\n");
  }
//  fprintf(stderr, "sigmin/max %f %f\n",smp->sigmin, smp->sigmax);  

// write earthquake parameters to file
// eq hypocenter params
  for(cnt=0; cnt<catn->nv; cnt++) {
    fprintf(fpeq, "%7.2f %6.2f %4.2f %7.2f %d %d %d %d ", catn->lon[cnt], catn->lat[cnt], catn->mag[cnt], catn->depth[cnt], catn->year[cnt], catn->month[cnt], catn->day[cnt], catn->agrid_id[cnt]);
// neighbor number
// modify neighbor distances to within min/max smoothing parameters
    for(cnt1=0; cnt1<(int)NNUM; cnt1++) {
      if ( catn->dist_nbr[cnt][cnt1] < smp->sigmin) catn->dist_nbr[cnt][cnt1]=smp->sigmin;
      if ( catn->dist_nbr[cnt][cnt1] > smp->sigmax) catn->dist_nbr[cnt][cnt1]=smp->sigmax;
    }
// write out results
    for(cnt1=0; cnt1<(int)NNUM; cnt1++) {
      if ( smp->apply_sm == 1 ) fprintf(fpeq, "%6.1f ", catn->dist_nbr[cnt][cnt1]);
    }
    fprintf(fpeq, "\n");
  }

// close and report file name
  fclose(fpeq);
  fprintf(stderr, "Wrote parameters for selected earthquakes to file, selected_eqs_neighbors.dat\n");
}

