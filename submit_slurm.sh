#!/bin/bash

#SBATCH -J pseudo_dagger
#SBATCH -p debug
#SBATCH -o /N/project/ceem_ucn/pseudo_dagger-out/%j.o
#SBATCH -e /N/project/ceem_ucn/pseudo_dagger-out/%j.e
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --time=00:40:00
#SBATCH -A r00344

#Call "date" at beginning and end just to get timestamps
date

OUTPUT_DIR=/N/project/ceem_ucn/pseudo_dagger-out/${SLURM_JOB_ID}

cd ~/projects/pseudo_dagger/ #go to cenns10geant4 directory
module load root
make
mkdir -p ${OUTPUT_DIR}       #this is where all output will go
( git log ; \
  git status ; \
  git diff ) \
  > ${OUTPUT_DIR}/gitstatus.txt #document the code and any uncommited changes

#off we go
srun bash run_slurm.sh ${OUTPUT_DIR}

date
