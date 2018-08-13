#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agrid.h"


/*--------------------------------------------------------------------------*/
void init_dist_rates(struct eqcat* catn, float atot[], struct rates* seisr)
/*--------------------------------------------------------------------------*/
{
  int cnt, cnt1;
//  float initv=1e-12;

// initialize rates and distances to zero
  for(cnt=0; cnt<(int)NV; cnt++) {
    atot[cnt]=0.0;
    seisr->asum_cnt_int[cnt]=0;
    seisr->asum_cnt[cnt]=0.0;
    seisr->asum[cnt]=0.0;
/*    seisr->asum3[cnt]=0.0;
    seisr->asum3p5[cnt]=0.0;
    seisr->asum4[cnt]=0.0;
    seisr->asum4p5[cnt]=0.0;
    seisr->asum5[cnt]=0.0;
*/
    seisr->asum_noSm[cnt]=0.0;
  }
// initialize neighbor numbers to zero
  for(cnt=0; cnt<(int)NEQ; cnt++) {
    for(cnt1=0; cnt1<(int)NNUM; cnt1++) {
      catn->dist_nbr[cnt][cnt1]=0.0;
    }
  }

}
