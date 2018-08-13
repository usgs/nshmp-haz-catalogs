# Adaptive Smoothed Seismicity Code

### Source Code:
* Source code is provided in adaptive/src
* Code can be compiled and installed in a `$(HOME)/bin` directory, if desired, using existing Makefile
* Compiling currently uses gcc/gfortran compilers

### Makefile:
* Location: `adapative/src`
* Compile: `make`
* Compile and install in `$(HOME)/bin`: `make install`
* Clean object files: `make clean`
* Clean executable and object files: `make cleanall`

### Running bash script to compute adaptive smoothed seismicity model:
* Example script and input files reproduce the adaptive-smoothed seismicity model for the central and eastern U.S. that was used in the development of the 2014 NSHM
* `nnum=4`
* `ck_params=n`
* `catalog_f=emm_mod.c4`
* `Mmin=3.2`
* `yMin=1870`
* `compl_file=dir_completeness_maps/compl_CEUS_M3p2.txt`
* `run_CEUS_final_inc_AdSm_2014NSHM.sh $based $nnum $ck_params $catalog_f $Mmin $yMin $compl_file`
* `bash run_CEUS_final_inc_AdSm_2014NSHM.sh y 4 n emm_mod.c4 3.2 1870 dir_completeness_maps/compl_CEUS_M3p2.txt`

### Output Files: 
* Output files are written to the directory, `dir_adN4_incr_Mmin3p2_1870_2012`
* Output values (10^a) are written to file, `agrd_adN4_incr_Mmin3p2_1870_2012_M0.out`
* this "agrid" file is formatted for input into the nshm-haz-fortran code
* ascii versions of the 10^a values may be computed with the bin2xyzX.v2 utility

### Note:
This version of the smoothed seismicity code uses an older format-style for the 
earthquake catalog than is currently formatted.
