#!/bin/bash

START=$1
LIMIT=$2
INFILE=$3
export OUTFILE=$4
for ((a=START; a<LIMIT;a++))
do
  export envvar_bsh=$a
  sbatch submit_geant4.slurm $3 &
  sleep 0.2
done 
wait 
