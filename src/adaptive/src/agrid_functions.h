// all function declarations here

// c functions
void print_param_format(void);
//int check_args(char *parf, char *parf_format, int *newp);
int check_args(int argc, char **argv, char *paramf, char *summf, char *paramf_format, int *stop_run, int *newp);
int read_params_new_old(char *parf, struct compl_levels* cat_comp, struct boundaries* reg, struct catalog* cat, struct smoothp* smp, char *outfnm, int new_style_formatting);
int read_params(char *parf, struct compl_levels* cat_comp, struct boundaries* reg, struct catalog* cat, struct smoothp* smp, char *outfnm);
int read_params_new(char *parf, struct compl_levels* cat_comp, struct boundaries* reg, struct catalog* cat, struct smoothp* smp, char *outfnm);
void Parse_Text(FILE *fp, const char *querystring, const char type, void* result);
void write_params(struct compl_levels* cat_comp, struct boundaries* reg, struct catalog* cat, struct smoothp* smp1, char *outfnm) ;
void write_params_to_file(char *fnm, struct compl_levels* cat_comp, struct boundaries* reg, struct catalog* cat, struct smoothp* smp1, char *outfnm);
void init_dist_rates(struct eqcat* catn, float atot[], struct rates* seisr);
int get_eqs_fm_catalog(char *fnm, struct eqcat* catn, struct compl_levels* compl, struct boundaries* reg, struct rates* seisr);
void get_eq_neighbor_dists(struct eqcat* catn);
void write_eq_neighbor_dists(struct eqcat* catn, struct smoothp* smp);
void smooth_seis_rates(struct rates* seisr, struct smoothp* smp, struct eqcat* catn, struct boundaries* reg, struct compl_levels* cat_comp, struct catalog* cat);
float calc_seis_rate(struct smoothp* smpp, struct eqcat* eqcatp, struct compl_levels* cat_compp, struct catalog* cat_parp, int eqno);
void convert_eq_nos_rates(struct rates* seisr, struct compl_levels* compl, struct smoothp* smp, struct catalog* cat, struct boundaries* reg);
float get_sigma_value(struct smoothp* smp, struct eqcat* catn, int eqno);
void write_asum_to_file(struct rates* seisr, char *fbasenm, struct catalog * catv, float mag);
void write_unsmoothed_asum_to_file(struct rates* seisr, char *fbasenm);
void write_results_to_file(struct smoothp* smp, struct rates* seisr, struct catalog * cat, char *outf);
void initialize_params(struct compl_levels* cat_comp, struct boundaries* reg, struct catalog* cat, struct smoothp* smp);
void read_completenessVals(struct compl_levels* cat_comp);
void assign_variable_completeness_eqs(struct eqcat* cat, struct compl_levels* compl_vals);
