MANTISROOT
==

Analysis Package for Mantis Simulations providing the User with quick analysis on Mantis output files.

LOADING INTO ROOT 
==

root MantisROOT.cc

`> MantisROOT* m = new MantisROOT(debug=false)`

MEMBER FUNCTIONS 
==

***************************************************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************

CheckAngles
==

void CheckAngles(const char* filename, int estimate=-1)

DESCRIPTION: 

Determines which Angles emitted from IntObjOut are Detected.
Estimate sets the limit on how many events to check. Set estimate to -1 for all events.
Usefull for determining emission angle cuts to place for developing efficient simulations 


***************************************************************************************************************************************************************

CheckDet
==

void CheckDet(const char* filename, bool weighted=false, int estimate=-1)

DESCRIPTION: 
Checks DetInfo TTree for matches in IntObjOut TTree event IDs.
If there is not a matching ID the DetInfo is removed for that event.
If DetInfo is a weighted spectrum include the second input bool true.
For a subset of the data include the third input. Set estimate to -1 for all events 


***************************************************************************************************************************************************************

CheckEvents
==

void CheckEvents(const char* filename, bool Weighted, bool Corrected)

DESCRIPTION: 
Checks TTrees NRF, Cherenkov and DetInfo for EventIDs that match.
If the events match the event data in the detector is collected and written to w_events_(filename).
This function is called in Sig2Noise.
Corrected refers to whether or not the Corrected_DetInfo has been written to filename after running CheckDet.
Example: mantis->CheckEvents("test.root",true, true) would Check WEIGHTED CORRECTED events in file test.root.

***************************************************************************************************************************************************************

CheckIntObj
==

void CheckIntObj(const char* onFile, const char* offFile, double Er=1.73354, bool Weighted=false)

DESCRIPTION: 
Draws a 200eV Wide Region centered on Er with 2eV bin widths for both Chopper States (On/Off).
If the spectra contain weights be sure to add the boolean true to the fourth input.

***************************************************************************************************************************************************************

CombineFiles
==

void CombineFiles(std::vector<string> filenames, std::vector<string> ObjectsToCombine, const char* Outfilename)

DESCRIPTION: 
Combines files through TChain Method.
Takes a vector of filenames with the TTree object names in the second input vector to merge into the third input (filename).
Example: mantis->CombineFiles({"file1.root","file2.root"},{"IntObjIn","DetInfo"},"Merged.root")

***************************************************************************************************************************************************************

CopyTrees
==

void CopyTrees(const char* filename, std::vector<string> TTreeNamesToCopy, const char* outfilename)

DESCRIPTION:
Clones TTrees from input 1 (filename) to a file named by input 3 outfilename.
The TTrees to be cloned are inputed as input two as a string vector.
Example: mantis->CopyTrees("test.root",{"IntObjIn","DetInfo"})

***************************************************************************************************************************************************************

CreateDetEfficiencyCurve
==

void CreateDetEfficiencyCurve(vector<double>, energies vector<double> efficiencies, string PCName)
  
DESCRIPTION: 
Creates TGraph of the users Photocathode Quantum Efficiency given vectors or energies and efficiencies
The PCName is written to the title of the output canvas.

void CreateDetEfficiencyCurve(string PCType)

DESCRIPTION: 
Creates TGraph of either "GaAsP" or "Bialkali" Photocathode Quantum Efficiency

***************************************************************************************************************************************************************

CreateScintillationDistribution
==

void CreateScintillationDistribution(vector<double> energies, vector<double> crossX)
  
DESCRIPTION: 
Creates a TGraph of the users scintillation distribution given vectors of energies and probabilies 

void CreateScintillationDistribution(string a, string b, string c, string d)

DESCRIPTION: 
Creates a TGraph of a scintillation distribution based on exponential distributions with parameters "a", "b", "c", and "d".

Exponential Rise Equation-> a*exp(b*x)
Exponential Decay Equation-> c*exp(d*x)

void CreateScintillationDistribution()

DESCRIPTION: 
Creates a TGraph of the default scintillation distribution

***************************************************************************************************************************************************************

Energy/Wavelenth Conversion
==

double Energy2Wave(double energy, string unit="eV")

DESCRIPTION: 
Returns wavelength unit meters from energy unit eV.
Unit options eV keV MeV J

double Wave2Energy(double wavelength, string unit="m")

DESCRIPTION: 
Returns energy unit eV from wavelength unit meters.
Unit options nm mm m km

***************************************************************************************************************************************************************

Help
==

void Help()

DESCRIPTION: 
Help lists the available function calls and their descriptions.
To See just the Functions calls try Show().

***************************************************************************************************************************************************************

Integral
==

void Integral(TTree* Object)

DESCRIPTION: 
Computes the integral of the TTree by summing each entry of the TTree object.

void Integral(std::vector<TTree*> Objects)

DESCRIPTION: 
Computes the integral of the TTree by summing each entry of the TTree objects in the users vector of TTree object names

void Integral(std::vector<TTree*> Object, TCut IntegralCut)

DESCRIPTION:
Computes the integral of the TTree after the TCut has been applied.

***************************************************************************************************************************************************************

OpenFile
==

TFile* OpenFile(const char* filename)

DESCRIPTION: 
Opens a TFile with the input filename. Returns TFile*

***************************************************************************************************************************************************************

PredictThickness
==

void PredictThickness(std::vector<string> ObjectNames)
  
DESCRIPTION: 
Predicts the results of changing the thickness on the objects passed in the string vector.

void PredictThickness(std::vector<string> ObjectNames, double ResonanceEnergy)
  
DESCRIPTION: 
Predicts the results of changing the thickness on the objects passed in the string vector.
Thickness calculations will focus on the given resonance energy.
Example: mantis->PredictThickness({"IntObjIn","IntObjOut"},1.73354)

***************************************************************************************************************************************************************

PrepareAnalysis
==

void PrepareAnalysis(vector<string> filebases, bool weighted=false)
  
DESCRIPTION: 
Prepares files from users filebases vector. If the files have weighted information set the second input to true
Example: mantis->PrepareAnalysis({"test9","test10"},true)
Would run CheckDet, CheckEvent and CopyTrees on weighted spectra in files 
test9On-merged.root, test9Off-merged.root, test10On-merged.root and test10Off-merged.root
