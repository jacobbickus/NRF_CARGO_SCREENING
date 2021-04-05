# Mantis
This program allows the user to run calorimetric NRF Cherenkov Detector cargo screening simulations. This simulation provides visualization tools in addition to root data. Visualization tools can be utilized in both batch and interactive modes. The final output provided can be manipulated by the user through the "mantis.in" input.

Mantis Output
==

Ten(10) ROOT ntuples(TTrees) are available upon user request:

1. Brem Emission Data -> Available only with the Option BremTest supplied by User
 - Energy
 - Theta Angle
 - Phi Angle

2. Incident Chopper Data -> /output/myoutput ChopIncData must be uncommented!
 - Event IDs
 - Energy
 - Weight

3. Emission Chopper Data -> /output/myoutput ChopOutData must be uncommented!
 - Event IDs
 - Energy
 - isNRF
 - Theta Angle
 - Phi Angle
 - Weight

4. NRF Data -> /output/myoutput/ NRFData must be uncommented!
 - Event IDs
 - Energy
 - Creation Material
 - Z position
 - Weight

5. Incident Interrogation Object Data -> /output/myoutput IntObjInData must be uncommented!
 - Event IDs
 - Track IDs
 - Energy
 - CreatorProcess
 - Theta
 - Phi
 - Time
 - Weight

6. Interrogation Object Emission Data -> /output/myoutput IntObjOutData must be uncommented!
 - Event IDs
 - Track IDs
 - Energy
 - CreatorProcess
 - Theta
 - Phi
 - Time
 - Weight

7. Water Data -> /output/myoutput WaterData must be uncommented!
 - Event IDs
 - Track IDs
 - Energy
 - CreatorProcess
 - Weight

8. Cherenkov Data -> /output/myouput/ CherenkovData must be uncommented!
 - Event IDs
 - Energy
 - NumSecondaries
 - Time
 - Weight

9. Detected Photocathode Data -> Automatic
 - Event IDs
 - Energy
 - X Position
 - Y Position
 - CreatorProcess
 - Time
 - Weight

10. Incident Photocathode Data -> /output/myoutput DetData must be uncommented!
 - Event ID
 - Energy
 - DetProcess
 - Weight

Mantis Output Post-Analysis
==

Included in Run_Analysis/MantisROOT.cc are various functions to aid in quick analysis of simulated data. See MantisROOT_ReadME.md for more information.
MantisROOT.cc can be loaded and compiled with CERN ROOT with the following terminal line of code:

`> root MantisROOT.cc `

`> MantisROOT* m = new MantisROOT()`

For help on the list of functions run:

`> m->Help()`



Mantis Input
==

The User can manipulate the simulation in various ways through the mantis.in macro input file. For example the following line of code:
`> ./mantis -m mantis.in -o test.root -s 1`
would run mantis with the inputs found in mantis.in located in the mantis executable directory. The output would be found in test.root and would have a seed of 1.

The command line input options and flags include:

`-a Energy` -> Sets the energy of the primary particle to the user's value in MeV

`-d Debug` -> Runtime Boolean option for developers to place program in debugging mode printing statements at various spots in the program

`-f PrintEvents` -> Runtime Boolean option to print event tracker to std::cout instead of G4cout to file

`-h help` -> Prints Usage Options

`-i Input Filename` -> Input File Containing hBrems bremsstrahlung input spectrum (ROOT Format TH1D*) to sample from. No importance sampling weighting will be done if the -i input is passed. The Default input file name is brems_distributions.root which should contain an importance sampling distribution

`-m macro file`-> Macro File to be read for user input options -> Required!

`-n addNRF` -> IF set to false NRF Physics will be removed from physicsList! The default is set to true.

`-o Output Filename` -> Data will be written to this file

`-p print standalone.dat file` -> Calls G4NRF to print a file of NRF Energies (takes up to 15 min) not recommended for non-developers

`-r Test Resonance` -> Tests Resonance energies as input

`-s seed` -> Seeds simulation Default = 1

`-t testBrem` -> For creating a bremsstrahlung beam for a secondary simulation input. Requires -a energy flag to be passed with max bremsstrahlung energy

`-u Uniform Sampling` -> Samples from a normal distribution centered on user's energy passed with flag -a

`-v NRF Verbose`   

`-w Uniform Width` -> Sets the uniform distribution width. Requires -u SampleEnergyRange Boolean to be passed as true and requires flag -a to be passed with energy greater than 0

__Mandatory Inputs for mantis.in__

mantis.in has the following MANDATORY inputs that the user must not comment:

1. Chopper State (On/Off)
2. Chopper Material (Uranium/Plutonium)
3. Chopper Material Isotopic Composition
4. Interrogation Object Material
5. Interrogation Object Material Isotopic Composition if Interrogation Object is Uranium/Plutonium

The user also has several optional commands available in mantis.in:

1. Beam
 - BeamSize
 - Energy Region Split Cut (For Importance Sampling Simulations)
2. Event
 - Event Info Frequency
3. Chopper Wheel
 - State
 - Material
 - Material Composition
 - Thickness
 - Distance From Source
4. Container
 - Remove
5. Interrogation Object
 - Radius
 - Material
 - Material Composition
6. Cargo
 - Add Spherical Object
 - Add Box Object
 - Sphere Radii
 - Sphere Positions
 - Sphere Materials
 - Box Sizes
 - Box Positions
 - Box Materials
7. Detector
 - Water Tank X,Y,Z Dimensions
 - Water Tank BackScattering Angle
 - Up to 2 Shielding Layer States, Thickness, and Materials
 - Photocathode Radius (PMT Radius Matches)
 - Photocathode Material (Automatically Sets Material Optical Properties)
 - Number of PMTs in Each Water Tank  
8. Output
 - Check Volume Overlaps (Recommended)
 - Output Data Options
9. Run
 - Number of MC Particles to Run

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


For Easy Set up run the following lines of code:

`> git clone https://github.com/jacobbickus/mantis.git && mkdir RunMantis && cd RunMantis && cmake ../mantis && make -jN`

The Following Instructions should work once the Geant4 and CERN ROOT are installed:

Be sure to untar the NRF Database using

`> tar xfz NRF_Database.tar.gz`

Export the path to the untarred NRF Database for example in the user's bash:

`export G4NRFGAMMADATA=/path/to/Database/Database1.1`

Lastly some path issues may occur without the following lines in the user's bash:

`source /path/to/root_build_directory/bin/thisroot.sh`

`source /path/to/geant4-install/bin/geant4.sh`

`source /path/to/geant4-install/share/Geant4-10.5.1/geant4make/geant4make.sh`

To Run in Batch Mode
==
"vis_save.mac" will make a .wrl visualization file based on the commands provided in "vis_save.mac".

"mantis.in" will not create a visualization.

`> ./mantis -m macro(mantis.in or vis_save.mac) -o <root output filename> -s <seed>`

Creating a Bremsstrahlung Input Beam
==

The bremsstrahlung beam can be determined via other Geant4 simulations or through mantis. To create the input bremsstrahlung beam expected to inpinge upon the chopper wheel run mantis the following way:

`> ./mantis -m mantis.in -o bremsstrahlung.root -s <seed> -t true `

Importance Sampling Simulations
==

The Output from the simulation can be prepared for an importance sampling simulation by running either:

MantisROOT::Sampling("bremsstrahlung.root")

MantisROOT::SimpleSampling("bremsstrahlung.root")

Non-Importance Sampling Simulations
==

The output from the simulation should be prepared with MantisROOT::PrepInputSpectrum("bremsstrahlung.root",5e-6,"brem.root")


Output Notes
==

The Code will generate a .log and _error.log file in addition to a root file. All G4cout is sent to .log and all G4cerr is sent to _error.log. The user must select output in "mantis.in" for the histograms/TTree to be filled during the simulation.
