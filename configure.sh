#!/bin/bash

# Default Arguments

GEANT4_DIR="None"
ROOT_DIRECTORY="None"
RUN_TEST="true"
DEBUGGING="false"
VERBOSE="false"

# Read Input Flags

for arg in "$@"
do
   case $arg in
        -h|--help)
        echo "Configure Script for Mantis"
        echo "options:"
        echo "h,--help              show brief help"
        echo "--geant4_dir          specify the Geant4 Install Directory to be sourced"
        echo "--root_dir            specify thisroot.sh Directory to be sourced"
        echo "--verbose             choose verbosity mode for cmake (optional Default=false)"
        shift
        exit 0
        ;;
        -g|--geant4_dir=*)
        GEANT4_DIR="${arg#*=}"
        shift
        ;;
        -r|--root_dir=*)
        ROOT_DIRECTORY="${arg#*=}"
        shift # Remove argument name from processing
        ;;
       -v|--verbose=*)
       VERBOSE="${arg#*=}"
       shift
       ;;
    esac
done

#echo "Geant4 Directory: $GEANT4_DIR"

echo Configuring mantis...

current_dir="$(pwd)"
cd ../
#cd ~
#bash_file=".bashrc"
#old_bash_file=".old_bashrc"

# Dealing with .bashrc
mkdir MANTIS_MAIN_DIR && mv $current_dir MANTIS_MAIN_DIR
cd MANTIS_MAIN_DIR/mantis
#if [ -f ~/.bashrc ]
#then
#   cp $bash_file MANTIS_MAIN_DIR/mantis && cd MANTIS_MAIN_DIR/mantis
#   cp $bash_file $old_bash_file
#else
#   cd MANTIS_MAIN_DIR/mantis
#fi

# Export NRF Database
tar xfz NRF_Database.tar.gz && mv Database1.1 ../ && cd ../Database1.1
database_working_dir="$(pwd)"
echo "Exporting the Database working directory path: $database_working_dir"
cd ../mantis
#echo "export G4NRFGAMMADATA=$database_working_dir " | tee -a $bash_file >/dev/null
export G4NRFGAMMADATA=$database_working_dir

# Source ROOT CERN
if [ $ROOT_DIRECTORY != "None" ]
then
    echo "ROOT DIRECTORY: $ROOT_DIRECTORY"
    VARROOTBIN=$ROOT_DIRECTORY
    echo "Sourcing $VARROOTBIN"
    source $VARROOTBIN
else
    echo Using the following ROOT CERN Directory...
    which "root"
    #VAR1=$(which "root")
    #VAR2=${VAR1:0:${#VAR1}-4}
    #VAR3="thisroot.sh"
    #VARROOTBIN=$VAR2$VAR3
fi

#echo "source $VARROOTBIN " | tee -a $bash_file >/dev/null

# Source Required Geant4 Make Files
if [ $GEANT4_DIR != "None" ]
then
   VAR4="/bin/geant4.sh"
   VARGEANTBIN=$GEANT4_DIR$VAR4
   echo "Sourcing $VARGEANTBIN"
   source $VARGEANTBIN
#echo "source $VARGEANTBIN " | tee -a $bash_file >/dev/null

   VAR5="/share/Geant4-10.5.1/geant4make/geant4make.sh"
   VARGEANTMAKE=$GEANT4_DIR$VAR5
   echo "Sourcing $VARGEANTMAKE"
   source $VARGEANTMAKE
fi
#echo "source $VARGEANTMAKE " | tee -a $bash_file >/dev/null

#cp $bash_file ~
#echo "Old bash file saved in: MANTIS_MAIN_DIR/mantis"

# Build Mantis
echo Building Mantis...

cd ../ && mkdir mantis_run && cd mantis_run
if [ $VERBOSE = "true" ]
then
   echo "Passing verbosity argument to cmake"
   cmake -DMYVERBOSE=ON ../mantis && make
else
   cmake -DMYVERBOSE=OFF ../mantis && make
fi

echo "Be sure to add the following to your bash profile: "
echo "export G4NRFGAMMADATA=$database_working_dir"
echo "Be sure to have thisroot.sh, geant4make.sh and geant4.sh all sourced prior to running"
echo "Mantis Configured. Good Luck and try the README.md!"
