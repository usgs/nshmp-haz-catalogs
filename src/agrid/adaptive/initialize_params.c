#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agrid.h"


/*--------------------------------------------------------------------------*/
void initialize_params(struct compl_levels* cat_comp, struct boundaries* reg, 
  struct catalog* cat, struct smoothp* smp)
/*--------------------------------------------------------------------------*/
{
// boundaries structure
  reg->minlat=-9999.0;
  reg->maxlat=-9999.0;
  reg->minlon=-9999.0;
  reg->maxlon=-9999.0;
  reg->dlon=-9999.0;
  reg->dlat=-9999.0;
  reg->nsx=0;
  reg->nsy=0;
  reg->nsrc=0;

// compl_levels structure
  cat_comp->nlevels=0;
  cat_comp->end_yr=0;
  cat_comp->dMag=0.0;
  cat_comp->variableCompleteness=0;
  cat_comp->catalogGaps=0;
  cat_comp->nVarComp=0;

// catalog structure
  cat->end_yr=0;
  cat->minMag=0;
  cat->dMag=0.0;
  cat->bval=-9999.0;

// smoothp structure
  smp->cumul_rates=1;
  smp->adj_compl=0;
  smp->apply_sm=0;
  smp->adapt_sm=0;
  smp->nnum2_log=0;
  smp->use_neff=0;


}
