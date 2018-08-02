INST_DIR = $(HOME)/bin
#BIN = agrid_cumul_compl_eq_c
BIN = calc_agrid_seis_rates

fflags = -ffixed-line-length-none -m64
FFLAGS = $(DBG) $(fflags)
CFLAGS = $(DBG) $(cflags) -Wall

FCC = gfortran
CC = gcc

DBG = 
FOBJS = print_param_format.o read_write_params.o init_dist_rates.o get_eqs_fm_catalog.o get_eq_neighbor_dists.o delaz.o inpolyint.o write_eq_neighbor_dists.o convert_eq_nos_rates.c smooth_seis_rates.c write_asum_to_file.c ck_args_wt_results.c calc_agrid_seis_rates.o initialize_params.o Parse_Text.o

all: $(BIN)

print_param_format.o: print_param_format.c
	$(CC) $(CFLAGS) -c print_param_format.c

read_write_params.o: read_write_params.c
	$(CC) $(CFLAGS) -c read_write_params.c

init_dist_rates.o: init_dist_rates.c
	$(CC) $(CFLAGS) -c init_dist_rates.c

get_eqs_fm_catalog.o: get_eqs_fm_catalog.c
	$(CC) $(CFLAGS) -c get_eqs_fm_catalog.c

delaz.o: delaz.f
	$(FCC) $(FFLAGS) -c delaz.f

inpolyint.o: inpolyint.f
	$(FCC) $(FFLAGS) -c inpolyint.f

get_eq_neighbor_dists.o: get_eq_neighbor_dists.c
	$(CC) $(CFLAGS) -c get_eq_neighbor_dists.c

write_eq_neighbor_dists.o: write_eq_neighbor_dists.c
	$(CC) $(CFLAGS) -c write_eq_neighbor_dists.c

convert_eq_nos_rates.o: convert_eq_nos_rates.c
	$(CC) $(CFLAGS) -c convert_eq_nos_rates.c 

smooth_seis_rates.o: smooth_seis_rates.c
	$(CC) $(CFLAGS) -c smooth_seis_rates.c 

write_asum_to_file.o: write_asum_to_file.c
	$(CC) $(CFLAGS) -c write_asum_to_file.c 

ck_args_wt_results.o: ck_args_wt_results.c
	$(CC) $(CFLAGS) -c ck_args_wt_results.c 

initialize_params.o: initialize_params.c
	$(CC) $(CFLAGS) -c initialize_params.c 

Parse_Text.o: Parse_Text.c
	$(CC) $(CFLAGS) -c Parse_Text.c 

calc_agrid_seis_rates.o: calc_agrid_seis_rates.c
	$(CC) $(CFLAGS) -c calc_agrid_seis_rates.c

$(BIN): $(FOBJS)
	$(CC) $(CFLAGS) $(FOBJS) -o $(BIN) -lm
#	$(CC) $(FFLAGS) $(CFLAGS) $(FOBJS) -o $(BIN)


install :: $(BIN)
	install -s $(BIN) $(INST_DIR)

clean :: 
	rm *.o 

cleanall :: 
	rm $(BIN) *.o 
