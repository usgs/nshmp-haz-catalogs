#####################################################################
# Name: Makefile
# Author: Brandon Clayton
# Description: Makefile for adaptive/src, fixed/src, and util 
#####################################################################

.PHONY: all clean cleanall

# Directory for binary files to reside
BIN_DIR = bin
# Directory for object files to reside
OBJ_DIR = $(BIN_DIR)/out

# Fortran compiler and flags
FC = gfortran
FFLAGS = -ffixed-line-length-none -m64

# C compiler and flags
CC = gcc
CFLAGS = -Wall

# Adaptive source code location
adaptiveDir = adaptive/src
# Fixed source code location
fixedDir = fixed/src
# Utilities source code locaiton
utilDir = util

bin2xyzXBin = $(BIN_DIR)/bin2xyzX.v2
adaptiveBin = $(BIN_DIR)/adaptiveSmoothing
fixedBin = $(BIN_DIR)/fixedSmoothing

all: $(bin2xyzXBin) $(adaptiveBin) $(fixedBin) 

# Adaptive smoothing
adaptiveObjects = $(addprefix $(OBJ_DIR)/, print_param_format.o \
		read_write_params.o init_dist_rates.o get_eqs_fm_catalog.o \
		get_eq_neighbor_dists.o delaz.o inpolyint.o write_eq_neighbor_dists.o\
		convert_eq_nos_rates.o smooth_seis_rates.o write_asum_to_file.o\
		ck_args_wt_results.o calc_agrid_seis_rates.o initialize_params.o Parse_Text.o)

$(OBJ_DIR)/delaz.o: $(adaptiveDir)/delaz.f | $(OBJ_DIR) 
	$(FC) $(FFLAGS) -c $< -o $@ 

$(OBJ_DIR)/inpolyint.o: $(adaptiveDir)/inpolyint.f | $(OBJ_DIR) 
	$(FC) $(FFLAGS) -c $< -o $@ 

$(OBJ_DIR)/print_param_format.o: $(adaptiveDir)/print_param_format.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/read_write_params.o: $(adaptiveDir)/read_write_params.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/init_dist_rates.o: $(adaptiveDir)/init_dist_rates.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/get_eqs_fm_catalog.o: $(adaptiveDir)/get_eqs_fm_catalog.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/get_eq_neighbor_dists.o: $(adaptiveDir)/get_eq_neighbor_dists.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/write_eq_neighbor_dists.o: $(adaptiveDir)/write_eq_neighbor_dists.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/convert_eq_nos_rates.o: $(adaptiveDir)/convert_eq_nos_rates.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/smooth_seis_rates.o: $(adaptiveDir)/smooth_seis_rates.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/write_asum_to_file.o: $(adaptiveDir)/write_asum_to_file.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/ck_args_wt_results.o: $(adaptiveDir)/ck_args_wt_results.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/initialize_params.o: $(adaptiveDir)/initialize_params.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/Parse_Text.o: $(adaptiveDir)/Parse_Text.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR)/calc_agrid_seis_rates.o: $(adaptiveDir)/calc_agrid_seis_rates.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(adaptiveBin): $(adaptiveObjects) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Fixed smoothing
fixedObjects = $(addprefix $(OBJ_DIR)/,grda_ceus.o inetennz.o\
		innewmad.o lxyin.o z18_cmpl1.o z18_cmpl2.o z18_cmpl3.o z18_cmpl4.o \
    z18_cmpl5.o z18_cmpl6.o)

$(OBJ_DIR)/inetennz.o: $(fixedDir)/inetennz.f | $(OBJ_DIR)
	$(FC) $(FFLAGS) -c -o $@ $<

$(OBJ_DIR)/innewmad.o: $(fixedDir)/innewmad.f | $(OBJ_DIR)
	$(FC) $(FFLAGS) -c -o $@ $<

$(OBJ_DIR)/lxyin.o: $(fixedDir)/lxyin.f | $(OBJ_DIR)
	$(FC) $(FFLAGS) -c -o $@ $<

$(OBJ_DIR)/z18_cmpl%.o: $(fixedDir)/z18_cmpl%.f | $(OBJ_DIR)
	$(FC) $(FFLAGS) -c -o $@ $<

$(OBJ_DIR)/grda_ceus.o: $(fixedDir)/grda_ceus.f | $(OBJ_DIR)
	$(FC) $(FFLAGS) -c -o $@ $<

$(fixedBin): $(fixedObjects) | $(BIN_DIR)
	$(FC) $(FFLAGS) -o $@ $^

# Utilities
utilObjects = $(addprefix $(OBJ_DIR)/,iosubs.o bin2xyzX.v2.o)

$(OBJ_DIR)/iosubs.o: $(utilDir)/iosubs.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/bin2xyzX.v2.o: $(utilDir)/bin2xyzX.v2.f | $(OBJ_DIR)
	$(FC) $(FFLAGS) -c $< -o $@

$(bin2xyzXBin): $(utilObjects) | $(BIN_DIR)
	$(FC) $(FFLAGS) -o $@ $^

# Make bin directory
$(BIN_DIR):
	mkdir -p $@

# Make object directory
$(OBJ_DIR):
	mkdir -p $@

# Remove all object files
clean:
	rm -f $(utilObjects) $(adaptiveObjects) $(fixedObjects)

# Remove out and bin directory
cleanall:
	rm -fr $(OBJ_DIR) $(BIN_DIR)
