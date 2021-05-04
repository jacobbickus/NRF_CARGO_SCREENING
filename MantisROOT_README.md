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

void CheckAngles(const char* filename, const char* object1, const char* object2, int estimate=-1)

DESCRIPTION:

Determines which Angles emitted from object1 are found in object2.
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

void CheckIntObj(const char* onFilename, const char* offFilename, double Er=1.73354, bool Weighted=false)

DESCRIPTION:
Draws a 200eV Wide Region centered on Er with 2eV bin widths for both Chopper States (On/Off).
If the spectra contain weights be sure to add the boolean true to the fourth input.

void CheckIntObjRegion(const char* onFilename, const char* offFilename, double regionCutE, TCut regionCut)

DESCRIPTION:
Checks the given interrogation object region based on the regionCutE which is the region cut energy. Specify the TCut with regionCut TCut.

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

Create Detector Response Function
==

void CreateDetectorResponseFunction(const char* filename, const char*, outfilename, double maxE = 1.8, bool drawFigures = false)

DESCRIPTION:
Create Detector Response Function based on the input file filename which should be a simulation ran with the Detector test flag. The Output will be a file outfilename that can be inputed into a simulation ran with a detector response function. Be sure to specify the maximum energy of the response function and if the response function figures are to be drawn set the 4th input to true.

void CreateOptPerEnergy(const char* filename, double e_cut = 1.4)

DESCRIPTION:
Creates the number of optical photons emitted per incident energy. A cut can be placed on the Xaxis with e_cut.

***************************************************************************************************************************************************************

Create Scintillation Distribution
==

TGraph* CreateScintillationDistribution(vector<double> energies, vector<double> crossX)

DESCRIPTION:
Creates a TGraph of the users scintillation distribution given vectors of energies and probabilies

void CreateScintillationDistribution(string a, string b, string c, string d)

DESCRIPTION:
Creates a TGraph of a scintillation distribution based on exponential distributions with parameters "a", "b", "c", and "d".

Exponential Rise Equation-> a\*exp(b*x)
Exponential Decay Equation-> c*exp(d*x)

TGraph* CreateScintillationDistribution(double scale=1.)

DESCRIPTION:
Creates a TGraph of the default scintillation distribution. If the user wants to scale to simulated spectrum the max counts value of the simulated spectra should be set as the input.

void GetScintillationDistribution(const char* filename, bool Corrected=true)

DESCRIPTION:
Takes the simulated DetInfo (Corrected_DetInfo if Corrected=true) and Plots it against the default input scintillation distribution scaled appropriately. Useful for a comparison of simulated detected scintillation spectra vs inputed scintillation distribution.

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

void PredictThickness(std::vector<string> ObjectNames, bool write2file=false)

DESCRIPTION:
Predicts the results of changing the thickness on the objects passed in the string vector.

void PredictThickness(std::vector<string> ObjectNames, double ResonanceEnergy, bool write2file=false)

DESCRIPTION:
Predicts the results of changing the thickness on the objects passed in the string vector.
If the second input is passed the thickness calculations will focus on the given resonance energy. If the third input is set to true the scaled histograms will be written to a file. If a Resonance Energy is provided a vector of the scaled histograms is also returned at function call.
Example:
mantis->PredictThickness({\"IntObjIn\",\"IntObjOut\"},1.73354, true)"

This would predict thickness effects of IntObjIn and IntObjOut for the 1.73354 resonance energy and write the results to a file.

***************************************************************************************************************************************************************

Prepare Input Spectrum
==

void PrepInputSpectrum(const char*, const char* object = "ChopIn", string outfilename = "brem.root", bool Weighted = false, double deltaE = 0.001, double minimum_energy = 0.)

DESCRIPTION:
Prepares input spectrum file for Mantis Simulation without importance sampling.
deltaE is the bin width of the Histogram and if the user wishes a nondefault outfilename enter the third input.

void PrepIntObjInputSpectrum(const char* filename, const char* ObjName, const char* Outfilename, vector<double> energy_regions, vector<double> bin_widths, bool Weighted = false, bool normalize = true, bool drawWeights = false)

DESCRIPTION:
Prepares input spectrum file outfilename to input for a simulation ran without the chopper wheel by taking filename and creating a variable bin width histogram to sample from in accordance with energy_regions and bin_widths provided by user. If the data in filename is weighted set Weighted=true. To normalize the data set normalize=true and to draw the weighting distribution set drawWeights=true.


void Sampling(const char* filename, const char* object_to_sample, bool Weighted = false, string sample_element = "U", double deltaE = 5.0E-6, bool checkZero = false, double non_nrf_energy_cut = 1.5, double weighting_factor = 10000)

DESCRIPTION:
Creates an importance sampling distribution and prepares mantis input file brems_distributions.root.
If the user would like a different bin width for hBrems than the user can supply the bin width with input 3.
Example: mantis->Sampling("brem.root","U", 5e-6,true,1.5)
would create brems_distributions.root with 5e-6 bin widths where if any bin content = 0 that bin would be set to the prior bins content
the importance sampling distribution energies below 1.5 MeV would have importances 1/1000 of the NRF resonance energies.

void SimpleSampling(const char* filename, const char* object_to_sample, bool Weighted = false, double deltaE = 5.00E-6, double deltaE_short = 0.01, double cut_energy1 = 0.5, double cut_energy2 = 1., double weighting_factor = 1000, double weighting_factor2 = 10, bool checkZero = false, bool drawWeights = false)

DESCRIPTION:
Creates an importance sampling distribution and prepares mantis input file brems_distributions.root.
If the User would like a different bin width for hBrems than the user can supply the bin width with input 2.
Example: mantis->SimpleSampling("brem.root", 5e-6,1.5, 10000, true)
would create brems_distributions.root with 5e-6 bin widths where if any bin content = 0 that bin would be set to the prior bins content
the importance sampling distribution energies below 1.5 MeV would have importances 1/10000 of all energies above 1.5 MeV.

***************************************************************************************************************************************************************

Rebin Histograms
==

TGraph* VariableBinWidthRebin(const char* filename, const char* ObjName, const char* Outfilename, vector<double> energy_regions, vector<double> bin_widths, vector<double> samplev, bool weighted = false, bool normalize = true, bool for_weighted_spectrum = false)

DESCRIPTION:
Creates a variable bin width histogram of ObjName data from filename and writes the corresponding histogram and TGraph to Outfilename. The Energy regions and bin widths provided guide the variable width binning technique.

***************************************************************************************************************************************************************

Run Summary
==

void RunSummary(const char* onFile, const char* offFile, bool intObjIn = true, bool weighted = false, bool drawPlots = false, bool drawBeamEnergyPlots = false)

DESCRIPTION:
Prints the run summary of Chopper On(onFilename) and Chopper Off(offFilename) including
entries and means.

***************************************************************************************************************************************************************

Signal to Noise
==

void Sig2Noise(std::vector<string> Filenames, string DataName, bool Weighted=false, bool Corrected=false, bool cut=false, TCut cut1="NA")

DESCRIPTION:
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

void ZScore(const char* filenameOn, bool weightedOn, const char* filenameOff, bool weightedOff)

DESCRIPTION:
Computes weighted ZTest on input 1 filename and input 2 filename for detector information. If the data is weighted apply the appropriate boolean inputs.

void ZScore(double countsOn, double countsOff)

DESCRIPTION:
Computes ZScore on values provided.

ZScore = abs(c1 - c2)/(sqrt(pow(sqrt(c1),2) + pow(sqrt(c2),2)))

void ZScore(double countsOn, double errorOn, double countsOff, double errorOff)

DESCRIPTION:
Computs ZScore on values provided based on errors provided. 
