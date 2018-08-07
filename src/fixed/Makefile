############################################################
# Name: Makefile
# Author: Brandon Clayton
# Description: Makefile for fixed agrid smoothing codes 
############################################################

.PHONY: all clean cleanall

# Source files directory
SRC_DIR = src
# Directory for binary file to reside
BIN_DIR = bin
# Directory for object files to reside
OBJ_DIR = $(BIN_DIR)/out

# Fortran compliler
FC = gfortran
# Fortran compliler flags
FFLAGS = -ffixed-line-length-none

# Binary file path and name
EXE = $(BIN_DIR)/grda_ceus

all: $(EXE)

objects = $(addprefix $(OBJ_DIR)/,grda_ceus.o inetennz.o innewmad.o lxyin.o \
    z18_cmpl1.o z18_cmpl2.o z18_cmpl3.o z18_cmpl4.o \
    z18_cmpl5.o z18_cmpl6.o )

$(OBJ_DIR)/inetennz.o: $(SRC_DIR)/inetennz.f | $(OBJ_DIR)
  $(FC) $(FFLAGS) -c -o $@ $<

$(OBJ_DIR)/innewmad.o: $(SRC_DIR)/innewmad.f | $(OBJ_DIR)
  $(FC) $(FFLAGS) -c -o $@ $<

$(OBJ_DIR)/lxyin.o: $(SRC_DIR)/lxyin.f | $(OBJ_DIR)
  $(FC) $(FFLAGS) -c -o $@ $<

$(OBJ_DIR)/z18_cmpl%.o: $(SRC_DIR)/z18_cmpl%.f | $(OBJ_DIR)
  $(FC) $(FFLAGS) -c -o $@ $<

$(OBJ_DIR)/grda_ceus.o: $(SRC_DIR)/grda_ceus.f | $(OBJ_DIR)
  $(FC) $(FFLAGS) -c -o $@ $<

$(EXE): $(objects) | $(BIN_DIR)
  $(FC) $(FFLAGS) -o $@ $^

$(BIN_DIR):
  mkdir -p $@

$(OBJ_DIR):
  mkdir -p $@

# Remove all object files
clean:
  rm -f $(objects)

# Remove out and bin directory
cleanall:
  rm -fr $(objects) $(OBJ_DIR) $(BIN_DIR)
