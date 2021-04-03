# Mantis
This program allows the user to run calorimetric NRF Cherenkov Detector cargo screening simulations. This simulation provides visualization tools in addition to root data. Visualization tools can be utilized in both batch and interactive modes. The final output provided can be manipulated by the user through the "mantis.in" input. 

Mantis Output
==

Seven(7) histograms are available upon user request: 

1. Weighted Interrogation Object Incident Energy Spectrum -> /output/myoutput IntObjData must be uncommented!
2. Weighted NRF Photons Incident Interrogation Object Energy Spectrum -> /output/myouput NRFData must be uncommented!
3. Weighted Interrogation Object Emission Energy Spectrum -> /output/myoutput IntObjData must be uncommented!
4. Weighted Emitted NRF Photons from Interrogation Object Energy Spectrum -> /output/myoutput IntObjData must be uncommented!
5. Weighted Incident Water Tank Energy Spectrum -> /output/myoutput WaterIncData must be uncommented!
6. Weighted NRF Photons Incident Water Tank Data -> /output/myouput NRFData must be uncommented!
7. Weighted Detected Energy Data
 - Weighted energy spectrum of photons "detected" on photocathode includes Quantum efficiency!

Additionally, five(5) ntuples(TTrees) are available upon user request:

1. Incident Chopper Data -> /output/myoutput ChopIncData must be uncommented!
 - Unweighted Incident Chopper Energies
 - Weights 
 - Event IDs

2. Emission Chopper Data -> /output/myoutput ChopOutData must be uncommented!
 - Unweighted Incident Chopper Energies
 - Weights 
 - Event IDs
 - isNRF 

3. NRF Data -> /output/myoutput/ NRFData must be uncommented!
 - Event IDs
 - Track IDs
 - Energy
 - Weight
 - Creation Material
 - Z position 
 
4. Cherenkov Data -> /output/myouput/ CherenkovData must be uncommented!
 - Energy
 - Weight
 - Event IDs
 - Track IDs
 - Number of Secondaries in Step (Cherenkov and Scintillation)
 - X Position 
 - Y Position
 - Z Position 
 - Time

5. Incident Photocathode Data -> /output/myoutput DetData must be uncommented!
 - Event IDs
 - Track IDs 
 - Energy
 - Weight
 - Creator Process
 - Detection Process
 - Time 
 

Mantis Input
==

The User can manipulate the simulation in various ways through the mantis.in macro input file. For example the following line of code:
`> ./mantis -m mantis.in -o test.root -s 1`
would run mantis with the inputs found in mantis.in located in the mantis executable directory. The ouput would be found in test.root and would have a seed of 1.

The command line input options and flags include:

`-m macro file`

`-n addNRF` -> IF set to false NRF Physics will be removed from physicsList! The default is set to true.

`-o output filename`

`-p print standalone.dat file` -> Calls G4NRF to print a file of NRF Energies (takes up to 15 min) not recommended for non-developers

`-s seed` 

`-t testBrem` -> For creating a bremsstrahlung beam to run Sampling.cc on for importance sampling input distribution

`-v NRF Verbose` 

`-e Event Check` -> Automatically Checks for Events where NRF Lead to Optical Photons that lead to detection see EventCheck.cc for details 

`-w Weight Histograms` -> Creates weighted histograms for TTree Outputs 

`-r Test Resonance` -> Tests Resonance energies for development of Sampling.cc 

__Mandatory Inputs for mantis.in__

mantis.in has the following MANDATORY inputs that the user must not comment:

1. Chopper State (On/Off)
2. Chopper Material (Uranium/Plutonium)
3. Chopper Material Isotopic Composition 
4. Interrogation Object Material
5. Interrogation Object Material Isotopic Composition if Interrogation Object is Uranium/Plutonium

The user also has several optional commands available in mantis.in:

1. Input Energy/Spectrum
2. Chopper Thickness
3. Chopper Distance from Source
4. Interrogation Object Location 
5. Interrogation Object Size (radius)
6. Detector Location
7. Attenuating Layers (state, material, thickness)
8. Photocathode (Size, number, material)
9. Output Desired 
10. Number of MC Particles to Simulate 

Author: Jacob E Bickus

Creation time: 8/2020 

Last Update: continous

To Install
==

Dependencies
  * Installed Geant4 Libraries 
  * CERN ROOT Installed 
  * Cmake 
  
__Version__:  Mantis and been built against and tested with Geant4 10.5 and 10.7, and ROOT 6.22. ROOT Version must be greater than 6.18. 

EASY SETUP WITH CONFIGURE FILE 
==

For Easy Set up run the following lines of code:
`> git clone https://github.com/jacobbickus/mantis.git && cd mantis`
`> ./configure.sh `


INSTALLATION WITHOUT CONFIGURE FILE (ONLY NECESSARY IF YOU DID NOT RUN ./configure.sh)
==

The Following Instructions should work once the Geant4 and CERN ROOT are installed:

Be sure to untar the NRF Database using 
`> tar xfz NRF_Database.tar.gz`

Export the path to the untarred NRF Database for example in the user's bash:

`export G4NRFGAMMADATA=/path/to/Database/Database1.1`

Lastly some path issues may occur without the following lines in the user's bash:

`source /path/to/root_build_directory/bin/thisroot.sh`

`source /path/to/geant4-install/bin/geant4.sh`

`source /path/to/geant4-install/share/Geant4-10.5.1/geant4make/geant4make.sh`


To Build
==

Now go to geant4 application
`> mkdir <application_build> && cd <application_build>`

`> cmake /path/to/application/source && make -j[N]`

To Run in Batch Mode
==
"vis_save.mac" will make a .wrl visualization file based on the commands provided in "vis_save.mac".

"mantis.in" will not create a visualization. 
`> ./mantis -m macro(mantis.in or vis_save.mac) -o <root output filename> -s <seed>`
  
To Run in Interactive Mode
==
The following will use OPENGL in interactive mode 
`> ./mantis`
  
Required Inputs
==

Mantis Requires one input:

* brems_distributions.root - This is the input spectrum file that is read if the user does not uncomment the /input/energy line in mantis.in. The bremstrahlung input and sampling distribution can be easily manipulated with Sampling.cc  

Manipulating the Input Spectrum "brems_distributions.root" with Sampling.cc
==

Sampling.cc can be run with the following terminal command:

`> root -b -q 'Sampling.cc("brem.root",maxEnergyOfBremBeam)' `

Where the "brem.root" file is the bremsstrahulung spectrum expected to impact the chopper wheel. The "brem.root" file can be obtained with the instructions under the section "Creating a Bremsstrahlung Input Beam" below. The "maxEnergyOfBremBeam" is the same maximum energy of the input bremsstrahulung beam from the "brem.root" file. 

Sampling.cc creates two output files.

1. brems_distributions.root
2. brems_distributions.png 

The root file consists of two histograms: hBrems and hSample. These histograms are used to determine the importance sampling in the mantis program. The .png file is a depiction of the hBrems and hSampled overlayed on the same canvas. 

Creating a Bremsstrahlung Input Beam
==

The bremsstrahlung beam can be determined via other Geant4 simulations or through mantis. To create the input bremsstrahlung beam expected to inpinge upon the chopper wheel run mantis the following way:

`> ./mantis -m mantis.in -o brem.root -s <seed> -t true `

Adding the -t flag tells mantis that it should only output the Chopper Data TTree. Be sure to edit the following line in "mantis.in": 

`/input/energy maxEnergyOfBremBeam`


Output
==

The Code will generate a .log and _error.log file in addition to a root file. All G4cout is sent to .log and all G4cerr is sent to _error.log. The user must select output in "mantis.in" for the histograms/TTree to be filled during the simulation. 

Analysis
==

Several helper scripts are included:
1. runBatch.sh
* The main call for running in batch mode on PSFC engaging cluster through slurm
* Takes input <start Seed> <Last Seed> <Input Macro FileName> <Output FileName Root> where the number of jobs submitted = <Last Seed> - <start Seed>
 
2. submit_geant4.slurm
* This is the file for the slurm scheduler. Adjusting the time limit on each run can be important for scheduling 

3. stitch.sh 
* This merges the output files.
* Takes inputs <"Output_FileName_Root*.root"> <merged_FileName.root> 


4. PrintResults.cc
* This prints the Chopper On/Off Comparison results and Z-Scores
* can be run in CERN ROOT with the command:

`> root -b -q 'PrintResults.cc("filenameOnBase", "filenameOffBase", checkWeighted_Histograms_File, check_Cherenkov, check_EventCheck)'`
