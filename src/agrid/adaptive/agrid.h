#define BUFFLEN 300
#define CLEVELS 10
#define NV 170000
#define NEQ 15000
#define NNUM 10
#define MAXCHAR 128
#define BUFFLEN 300
#define LINESIZE 512
#define STREQ(s1,s2) (strcmp(s1,s2) == 0)
#define NPPOLY 20
#define NPP 20
#define PI_VAL 3.14159265
#define DEG2RAD PI_VAL/180

/*
#define _GNU_SOURCE
*/

// structure for single polygon
  struct poly {
    int npts;
    int nyears;
    float lon[NPP], lat[NPP];
  };

// structure for all polygons
  struct all_poly {
    int npoly;
    struct poly elem[NPPOLY];
  }
  ;

struct compl_levels {
  int variableCompleteness;
  int catalogGaps;
  int correctAreaForLat;
  int nlevels;
  int end_yr;
  int yr_comp[CLEVELS];
  float mag_comp[CLEVELS];
  float dMag;
  char completenessFile[200];
  char catalogGapsFile[200];
  int nVarComp; 
  int yrCompl_arr[NV];
  float magCompl_arr[NV];
  float lon_arr[NV], lat_arr[NV];
  struct all_poly polygons;
} ;

struct boundaries {
  int nsx, nsy, nsrc;
  float minlon, maxlon, minlat, maxlat;
  float dlon, dlat;
} ;

struct catalog {
  int end_yr;
  float minMag, dMag, bval;
  char catnm[200];
} ;

struct smoothp {
  int cumul_rates, adj_compl, apply_sm, adapt_sm, nnum2_log, use_neff;
  int nnum, nnum2;
  int gauss_kern;
  float sigmin, sigmax, sigfix;
} ;

struct rates {
  int asum_cnt_int[NV];
  float asum_cnt[NV];
  float asum[NV];
//  float asum3[NV], asum4[NV], asum5[NV];
//  float asum3p5[NV], asum4p5[NV];
  float asum_noSm[NV];
  int nv;
} ;

struct eqcat {
  int nv;
  int year[NEQ], month[NEQ], day[NEQ], year_compl[NEQ];
  int lineno[NEQ], agrid_id[NEQ];
  float mag[NEQ], lon[NEQ], lat[NEQ], depth[NEQ], neff[NEQ], mag_sigma[NEQ]; 
  float mag_Mc[NEQ];
  float dist_nbr[NEQ][NNUM];
} ;

struct header
{
   char name[MAXCHAR][6];
   float period;
   int nlev;
   float xlev[20];
   float extra[10];
};

