#####################################################################
# Name: Makefile
# Author: Brandon Clayton
# Description: Makefile for adaptive/src, fixed/src, and util 
#####################################################################

.PHONY: all clean cleanall

# Fortran compiler and flags
FC = gfortran
FFLAGS = -ffixed-line-length-none -m64

# C compiler and flags
CC = gcc
CFLAGS = -Wall

# Directory for binary files to reside
binDir = build/exe
# Directory for object files to reside
objDir = build/objects

# Source directory
srcDir = src/agrid
# Adaptive source code location
adaptiveDir = $(srcDir)/adaptive
# Fixed source code location
fixedDir = $(srcDir)/fixed

adaptiveBin = $(binDir)/adaptiveAgrid
fixedBin = $(binDir)/fixedAgrid

all: $(bin2xyzXBin) $(adaptiveBin) $(fixedBin) 

# Adaptive smoothing
adaptiveObjects = $(addprefix $(objDir)/, print_param_format.o \
		read_write_params.o init_dist_rates.o get_eqs_fm_catalog.o \
		get_eq_neighbor_dists.o delaz.o inpolyint.o write_eq_neighbor_dists.o\
		convert_eq_nos_rates.o smooth_seis_rates.o write_asum_to_file.o\
		ck_args_wt_results.o calc_agrid_seis_rates.o initialize_params.o Parse_Text.o)

$(objDir)/delaz.o: $(adaptiveDir)/delaz.f | $(objDir) 
	$(FC) $(FFLAGS) -c $< -o $@ 

$(objDir)/inpolyint.o: $(adaptiveDir)/inpolyint.f | $(objDir) 
	$(FC) $(FFLAGS) -c $< -o $@ 

$(objDir)/print_param_format.o: $(adaptiveDir)/print_param_format.c | $(objDir) 
	$(CC) $(CFLAGS) -c $< -o $@

$(objDir)/read_write_params.o: $(adaptiveDir)/read_write_params.c | $(objDir) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(objDir)/init_dist_rates.o: $(adaptiveDir)/init_dist_rates.c | $(objDir) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(objDir)/get_eqs_fm_catalog.o: $(adaptiveDir)/get_eqs_fm_catalog.c | $(objDir) 
	$(CC) $(CFLAGS) -c $< -o $@

$(objDir)/get_eq_neighbor_dists.o: $(adaptiveDir)/get_eq_neighbor_dists.c | $(objDir)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(objDir)/write_eq_neighbor_dists.o: $(adaptiveDir)/write_eq_neighbor_dists.c | $(objDir) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(objDir)/convert_eq_nos_rates.o: $(adaptiveDir)/convert_eq_nos_rates.c | $(objDir) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(objDir)/smooth_seis_rates.o: $(adaptiveDir)/smooth_seis_rates.c | $(objDir) 
	$(CC) $(CFLAGS) -c $< -o $@

$(objDir)/write_asum_to_file.o: $(adaptiveDir)/write_asum_to_file.c | $(objDir) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(objDir)/ck_args_wt_results.o: $(adaptiveDir)/ck_args_wt_results.c | $(objDir)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(objDir)/initialize_params.o: $(adaptiveDir)/initialize_params.c | $(objDir) 
	$(CC) $(CFLAGS) -c $< -o $@ 

$(objDir)/Parse_Text.o: $(adaptiveDir)/Parse_Text.c | $(objDir)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(objDir)/calc_agrid_seis_rates.o: $(adaptiveDir)/calc_agrid_seis_rates.c | $(objDir)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(adaptiveBin): $(adaptiveObjects) | $(binDir)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Fixed smoothing
fixedObjects = $(addprefix $(objDir)/,grda_ceus.o inetennz.o\
		innewmad.o lxyin.o z18_cmpl1.o z18_cmpl2.o z18_cmpl3.o z18_cmpl4.o \
    z18_cmpl5.o z18_cmpl6.o)

$(objDir)/inetennz.o: $(fixedDir)/inetennz.f | $(objDir)
	$(FC) $(FFLAGS) -c -o $@ $<

$(objDir)/innewmad.o: $(fixedDir)/innewmad.f | $(objDir)
	$(FC) $(FFLAGS) -c -o $@ $<

$(objDir)/lxyin.o: $(fixedDir)/lxyin.f | $(objDir)
	$(FC) $(FFLAGS) -c -o $@ $<

$(objDir)/z18_cmpl%.o: $(fixedDir)/z18_cmpl%.f | $(objDir)
	$(FC) $(FFLAGS) -c -o $@ $<

$(objDir)/grda_ceus.o: $(fixedDir)/grda_ceus.f | $(objDir)
	$(FC) $(FFLAGS) -c -o $@ $<

$(fixedBin): $(fixedObjects) | $(binDir)
	$(FC) $(FFLAGS) -o $@ $^

# Make bin directory
$(binDir):
	mkdir -p $@

# Make object directory
$(objDir):
	mkdir -p $@

# Remove all object files
clean:
	rm -f $(adaptiveObjects) $(fixedObjects)

# Remove out and bin directory
cleanall:
	rm -fr $(objDir) $(binDir)
