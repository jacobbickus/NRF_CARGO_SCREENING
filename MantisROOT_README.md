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

Check Emission Angles
==

void CheckAngles(const char* filename, int estimate=-1)

DESCRIPTION: 

Determines which Angles emitted from IntObjOut are Detected.
Estimate sets the limit on how many events to check. Set estimate to -1 for all events.
Usefull for determining emission angle cuts to place for developing efficient simulations 


***************************************************************************************************************************************************************

Check Detector Events 
==

void CheckDet(const char* filename, bool weighted=false, int estimate=-1)

DESCRIPTION: 
Checks DetInfo TTree for matches in IntObjOut TTree event IDs.
If there is not a matching ID the DetInfo is removed for that event.
If DetInfo is a weighted spectrum include the second input bool true.
For a subset of the data include the third input. Set estimate to -1 for all events 


***************************************************************************************************************************************************************

Check Events
==

void CheckEvents(const char* filename, bool Weighted, bool Corrected)

DESCRIPTION: 
Checks TTrees NRF, Cherenkov and DetInfo for EventIDs that match.
If the events match the event data in the detector is collected and written to w_events_(filename).
This function is called in Sig2Noise.
Corrected refers to whether or not the Corrected_DetInfo has been written to filename after running CheckDet.
Example: mantis->CheckEvents("test.root",true, true) would Check WEIGHTED CORRECTED events in file test.root.

***************************************************************************************************************************************************************

Check Interrogation Object
==

void CheckIntObj(const char* onFile, const char* offFile, double Er=1.73354, bool Weighted=false)

DESCRIPTION: 
Draws a 200eV Wide Region centered on Er with 2eV bin widths for both Chopper States (On/Off).
If the spectra contain weights be sure to add the boolean true to the fourth input.

***************************************************************************************************************************************************************

Chopper Weight and Cost 
==

void ChopperWeightandCost(string material_name, double chopper_thickness, double chopper_radius=7.5cm)

DESCRIPTION: 
Determines Chopper Weight and Cost for a given chopper material and thickness. Default chopper radius set to 7.5cm.

***************************************************************************************************************************************************************

Combine Files
==

void CombineFiles(std::vector<string> filenames, std::vector<string> ObjectsToCombine, const char* Outfilename)

DESCRIPTION: 
Combines files through TChain Method.
Takes a vector of filenames with the TTree object names in the second input vector to merge into the third input (filename).
Example: mantis->CombineFiles({"file1.root","file2.root"},{"IntObjIn","DetInfo"},"Merged.root")

***************************************************************************************************************************************************************

Copy Trees
==

void CopyTrees(const char* filename, std::vector<string> TTreeNamesToCopy, const char* outfilename)

DESCRIPTION:
Clones TTrees from input 1 (filename) to a file named by input 3 outfilename.
The TTrees to be cloned are inputed as input two as a string vector.
Example: mantis->CopyTrees("test.root",{"IntObjIn","DetInfo"})

***************************************************************************************************************************************************************

Create Photocathode Efficiency Curve
==

void CreateDetEfficiencyCurve(vector<double>, energies vector<double> efficiencies, string PCName)
  
DESCRIPTION: 
Creates TGraph of the users Photocathode Quantum Efficiency given vectors or energies and efficiencies
The PCName is written to the title of the output canvas.

void CreateDetEfficiencyCurve(string PCType)

DESCRIPTION: 
Creates TGraph of either "GaAsP" or "Bialkali" Photocathode Quantum Efficiency

***************************************************************************************************************************************************************

Create Scintillation Distribution
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

Open File
==

TFile* OpenFile(const char* filename)

DESCRIPTION: 
Opens a TFile with the input filename. Returns TFile*

***************************************************************************************************************************************************************

Predict Thickness
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

Prepare Analysis
==

void PrepareAnalysis(vector<string> filebases, bool weighted=false)
  
DESCRIPTION: 
Prepares files from users filebases vector. If the files have weighted information set the second input to true
Example: mantis->PrepareAnalysis({"test9","test10"},true)
Would run CheckDet, CheckEvent and CopyTrees on weighted spectra in files 
test9On-merged.root, test9Off-merged.root, test10On-merged.root and test10Off-merged.root

***************************************************************************************************************************************************************

Prepare Input Spectrum
==

void PrepInputSpectrum(const char* bremInputFilename, double deltaE=5.0e-6, string outfilename="brem.root")

DESCRIPTION: 
Prepares input spectrum file for Mantis Simulation without importance sampling.
deltaE is the bin width of the Histogram and if the user wishes a nondefault outfilename enter the third input.

void Sampling(const char* bremInputFilename, string sample_element=U, double deltaE=5.0e-6, bool checkZero=false, double non_nrf_energy_cut=1.5)

DESCRIPTION: 
Creates an importance sampling distribution and prepares mantis input file brems_distributions.root.
If the user would like a different bin width for hBrems than the user can supply the bin width with input 3.
Example: mantis->Sampling("brem.root","U", 5e-6,true,1.5)
would create brems_distributions.root with 5e-6 bin widths where if any bin content = 0 that bin would be set to the prior bins content
the importance sampling distribution energies below 1.5 MeV would have importances 1/1000 of the NRF resonance energies.

void SimpleSampling(const char* bremInputFilename, double deltaE=5.0e-6, double cut_energy=1.5, double weight=10000, bool checkZero=false)

DESCRIPTION: 
Creates an importance sampling distribution and prepares mantis input file brems_distributions.root.
If the User would like a different bin width for hBrems than the user can supply the bin width with input 2.
Example: mantis->SimpleSampling("brem.root", 5e-6,1.5, 10000, true)
would create brems_distributions.root with 5e-6 bin widths where if any bin content = 0 that bin would be set to the prior bins content
the importance sampling distribution energies below 1.5 MeV would have importances 1/10000 of all energies above 1.5 MeV.

***************************************************************************************************************************************************************

Rebin Histograms
==

void RebinHisto(std::vector<string> inFile, std::vector<string> ObjName, std::vector<string> OutObjName, int nbins, double Emin, double Emax)
  
DESCRIPTION: 
Rebins the TTree Objects from multiples files given histogram parameters.
Example: mantis->RebinHisto({"file1.root","file2.root"},{"IntObjIn","IntObjOut"},{"Rebinned_IntObjIn","Rebinned_IntObjOut"},100,1.7334,1.7336)
This would rebin the Incident and Emission Interrogation object spectra into TH1D objects Rebinned_IntObjIn and Rebinned_IntObjOut respectively.
The rebinned histograms would have 100 bins between 1.7334 and 1.7336 MeV.

void RebinHisto(std::vector<string> inFile, std::vector<string> ObjName,std::vector<string> OutObjName, int nbins, double Emin, double Emax, TCut cut1)
  
DESCRIPTION: 
Rebins the TTree Objects from multiples files given histogram parameters.
Example: mantis->RebinHisto({"file1.root","file2.root"},{"IntObjIn","IntObjOut"},{"Rebinned_IntObjIn","Rebinned_IntObjOut"},100,1.7334,1.7336, "CreatorProcess == \"Beam\"")
This would rebin the Incident and Emission Interrogation object spectra into TH1D objects Rebinned_IntObjIn and Rebinned_IntObjOut respectively.
The rebinned histograms would have 100 bins between 1.7334 and 1.7336 MeV and only contain particles created as primary particles in the beam.

void VarRebin(vector<string>, vector<string>, vector<string>, int, double, double, TCut, double, double)
  
DESCRIPTION: 
See RebinHisto. This Function allows variable binning.

***************************************************************************************************************************************************************

Signal to Noise
==

void Sig2Noise(std::vector<string> Filenames, string DataName, bool Weighted=false, bool Corrected=false, bool cut=false, TCut cut1="NA")
  
Computes the Signal to Noise Ratio in the files inputed in the string vector.
The signal to noise ratio can be computed for the Incident Interrogation Object spectrum
, the detected spectrum, or both with the second input options: IncObj, Det, Both.
IF the TTrees contain weights be sure to set the third input bool option to true.
IF the TTrees are from a Corrected File be sure to set the fourth input bool option to true.
IF the TTrees are to have a cut placed the fifth input should be set to true and 
 the sixth input should contain the TCut in parenthesis.
Example: mantis->Sig2Noise({"TestOn.root","TestOff.root"},"Both", true, true, true, "Energy<5e-6")

***************************************************************************************************************************************************************

ZScore Tests
==

void ZScore(const char* filename1, const char* filename2, std::vector<string> ObjectNames, bool weighted=false)

DESCRIPTION:
Computes weighted ZTest on input 1 filename and input 2 filename for the TTree object names in input 3 string vector.
Example: mantis->ZScore("TestOn.root", "TestOff.root", {"IntObjIn","DetInfo"}, true)

void ZScore(double c1, double c2)

DESCRIPTION:
Computes ZScore on values provided.

ZScore = abs(c1 - c2)/(sqrt(pow(sqrt(c1),2) + pow(sqrt(c2),2)))
  
