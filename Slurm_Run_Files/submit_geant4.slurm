#!/bin/bash
#SBATCH -N 1
#SBATCH -n 1
#SBATCH --time=6:00:00
#SBATCH -p newnodes
#SBATCH --job-name=GEANT4
#SBATCH --constraint=centos7
MACRO=$1
OUTFILENAME=$OUTFILE
ARG=$envvar_bsh
srun ./mantis --macro ${MACRO} --output_filename ${OUTFILENAME}-${ARG}.root --seed ${ARG} &
wait
exit
