//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Author:
// Jacob E Bickus, 2021
// MIT, NSE
// jbickus@mit.edu
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////
bool debug;

class MantisROOT
{
public:
    MantisROOT(bool debug_in=false);
    ~MantisROOT();

  public:
    static MantisROOT *GetInstance();
    static void Destroy();
    void Help();
    void Show(string cmd="All", bool=false);
    TFile* OpenFile(const char*);
    void CombineFiles(std::vector<string>, std::vector<string>, const char*);
    void CopyTrees(const char*, std::vector<string>, const char*);
    void Sig2Noise(std::vector<string>, string, bool Weighted=false, bool Corrected=false, bool cut=false,TCut cut1="NA");
    void ZScore(const char*, const char*, std::vector<string>, bool weighted=false);
    void ZScore(double,double);
    void Integral(TTree*);
    void Integral(std::vector<TTree*>);
    void Integral(std::vector<TTree*>,TCut);
    void PredictThickness(std::vector<string>);
    void PredictThickness(std::vector<string>, double);
    void RebinHisto(vector<string>, vector<string>, vector<string>, int, double, double);
    void RebinHisto(vector<string>, vector<string>, vector<string>, int, double, double, TCut);
    void VarRebin(vector<string>, vector<string>, vector<string>, int, double, double, TCut, double, double);
    void CheckEvents(const char*, bool, bool);
    void Sampling(const char*, string sample_element="U", double deltaE=5.0e-6, bool checkZero=false, double non_nrf_energy_cut=1.5);
    void SimpleSampling(const char*, double deltaE=5.0e-6, double cut_energy=1.5, double weight=10000, bool checkZero=false);
    void CheckIntObj(const char*, const char*, double Er=1.73354, bool Weighted=false);
    void CheckAngles(const char*, int estimate=-1);
    TGraph* CreateTKDE(const char*, int nentries=10000);
    void CheckDet(const char*, bool weighted=false, int estimate=-1);
    void CreateScintillationDistribution(std::vector<double>, std::vector<double>);
    void CreateScintillationDistribution(string, string, string, string);
    void CreateScintillationDistribution();
    void CreateDetEfficiencyCurve(std::vector<double>, std::vector<double>, string);
    void CreateDetEfficiencyCurve(string);
    double Energy2Wave(double, string unit="eV");
    double Wave2Energy(double, string unit="m");
    void PrepareAnalysis(std::vector<string>, bool weighted=false, int estimate=-1);
    void PrepInputSpectrum(const char*, double deltaE=5.0e-6, string outfile="brem.root");
    //void CreateBremFit(const char*, double bin_width=5e-6);

private:

    // Zip Tuple 5
    template<typename A, typename B, typename C, typename D, typename E>
    void zip(const std::vector<A> &a, const std::vector<B> &b, const std::vector<C> &c,
      const std::vector<D> &d, const std::vector<E> &e,
              std::vector<std::tuple<A,B,C,D,E>> &zipped)
    {
      for(size_t i=0;i<a.size();++i)
      {
        zipped.push_back(std::make_tuple(a[i],b[i],c[i],d[i],e[i]));
      }
    }

    template<typename A, typename B, typename C, typename D, typename E>
    void unzip(const std::vector<std::tuple<A,B,C,D,E>> &zipped,
                std::vector<A> &a, std::vector<B> &b, std::vector<C> &c, std::vector<D> &d, std::vector<E> &e)
    {
      for(size_t i=0;i<a.size();++i)
      {
        a[i] = std::get<0>(zipped[i]);
        b[i] = std::get<1>(zipped[i]);
        c[i] = std::get<2>(zipped[i]);
        d[i] = std::get<3>(zipped[i]);
        e[i] = std::get<4>(zipped[i]);
      }
    }

    // Zip tuple 4

    template<typename A, typename B, typename C, typename D>
    void zip(const std::vector<A> &a, const std::vector<B> &b, const std::vector<C> &c, const std::vector<D> &d,
              std::vector<std::tuple<A,B,C,D>> &zipped)
    {
      for(size_t i=0;i<a.size();++i)
      {
        zipped.push_back(std::make_tuple(a[i],b[i],c[i],d[i]));
      }
    }

    template<typename A, typename B, typename C, typename D>
    void unzip(const std::vector<std::tuple<A,B,C,D>> &zipped,
                std::vector<A> &a, std::vector<B> &b, std::vector<C> &c, std::vector<D> &d)
    {
      for(size_t i=0;i<a.size();++i)
      {
        a[i] = std::get<0>(zipped[i]);
        b[i] = std::get<1>(zipped[i]);
        c[i] = std::get<2>(zipped[i]);
        d[i] = std::get<3>(zipped[i]);
      }
    }
    // Zip tuple 3

    template<typename A, typename B, typename C>
    void zip(const std::vector<A> &a, const std::vector<B> &b, const std::vector<C> &c,
              std::vector<std::tuple<A,B,C>> &zipped)
    {
      for(size_t i=0;i<a.size();++i)
      {
        zipped.push_back(std::make_tuple(a[i],b[i],c[i]));
      }
    }

    template<typename A, typename B, typename C>
    void unzip(const std::vector<std::tuple<A,B,C>> &zipped,
                std::vector<A> &a, std::vector<B> &b, std::vector<C> &c)
    {
      for(size_t i=0;i<a.size();++i)
      {
        a[i] = std::get<0>(zipped[i]);
        b[i] = std::get<1>(zipped[i]);
        c[i] = std::get<2>(zipped[i]);
      }
    }


    // Zip Pair

    template<typename A, typename B>
    void zip(const std::vector<A> &a, const std::vector<B> &b,
              std::vector<std::pair<A,B>> &zipped)
    {
      for(size_t i=0;i<a.size();++i)
      {
        zipped.push_back(std::make_pair(a[i],b[i]));
      }
    }

    template<typename A, typename B>
    void unzip(const std::vector<std::pair<A,B>> &zipped,
                std::vector<A> &a, std::vector<B> &b)
    {
      for(size_t i=0;i<a.size();++i)
      {
        a[i] = zipped[i].first;
        b[i] = zipped[i].second;
      }
    }

  public:
    static MantisROOT *instance;

  private:
    void CheckFile(const char*);
    void Compute(const char*, time_t, bool, bool);
    void CopyATree(const char*, const char*, const char*);
    void CopyATreeNoWeight(const char*, const char*, const char*);
    void SNR_IntObj(const char*, bool);
    void SNR_Det(const char*, bool, bool, bool, TCut cut1="NA");
    void hIntegral(TH1*);
    double hIntegral(TH1*, int);
    double hIntegral(TTree*, int, TCut, double);
    double hIntegral(TTree*, int);
    double hIntegralReturn(TTree*, bool, TCut cut1="NA");
    double hIntegralReturnWeightedCounts(TTree*, double cut_energy=5e-6);
    double hIntegralReturnWeightedEnergy(TTree*, double cut_energy=5e-6);
    void hIntegral(TTree*);
    void hIntegral(TTree*, TCut);
    void hIntegral(const char*, const char*);
    void hIntegral(const char*, const char*, TCut);
    void hIntegral(const char*);
    double ZTest(double, double);
    void ZTest(const char*, const char*, const char*, bool weighted=false);
    void Rebin(bool,const char*,const char*,const char*,int,double Emin=0.0,double Emax=2.1,TCut cut1="NA",bool VarArray=false,double nrf_bin_width=-1.,double non_nrf_bin_width=-1.);
    void Rebin(const char*, const char*, const char*);
    void Rescale(const char*, double);
    void Rescale(const char*);

    double ReturnBremMax(const char*);
    TH1D* BuildBrem(const char*, double, bool);
    TH1D* BuildSimpleSample(const char*, double, double, double);
    void WriteSampling(TH1D*, TH1D*, TGraph*, TGraph*, double);

    string EraseSubStr(string&, const string&);

    void Show_Help();
    void Show_Help_Description();
    void Show_Show();
    void Show_Show_Description();
    void Show_OpenFile();
    void Show_OpenFile_Description();
    void Show_CheckDet();
    void Show_CheckDet_Description();
    void Show_CombineFiles();
    void Show_CombineFiles_Description();
    void Show_CopyTrees();
    void Show_CopyTrees_Description();
    void Show_Sig2Noise();
    void Show_Sig2Noise_Description();
    void Show_ZScore();
    void Show_ZScore_Description();
    void Show_Integral();
    void Show_Integral_Description();
    void Show_PredictThickness();
    void Show_PredictThickness_Description();
    void Show_RebinHisto();
    void Show_RebinHisto_Description();
    void Show_VarRebin();
    void Show_VarRebin_Description();
    void Show_CheckEvents();
    void Show_CheckEvents_Description();
    void Show_Sampling();
    void Show_Sampling_Description();
    void Show_GetInstance();
    void Show_SimpleSampling();
    void Show_SimpleSampling_Description();
    void Show_CheckIntObj();
    void Show_CheckIntObj_Description();
    void Show_CheckAngles();
    void Show_CheckAngles_Description();
    void Show_CreateTKDE();
    void Show_CreateTKDE_Description();
    void Show_CreateScintillationDistribution();
    void Show_CreateScintillationDistribution_Description();
    void Show_CreateDetEfficiencyCurve();
    void Show_CreateDetEfficiencyCurve_Description();
    void Show_Energy2Wave();
    void Show_Energy2Wave_Description();
    void Show_Wave2Energy();
    void Show_Wave2Energy_Description();
    void Show_PrepareAnalysis();
    void Show_PrepareAnalysis_Description();
    void Show_PrepInputSpectrum();
    void Show_PrepInputSpectrum_Description();

    double hc = 6.62607004e-34*299792458;

};

MantisROOT *MantisROOT::instance = 0;


MantisROOT *MantisROOT::GetInstance()
{
  if(instance == 0)
  {
    instance = new MantisROOT();
  }
  std::cout << "Mantis ROOT Analysis Package for Mantis Output Analysis" << std::endl;
  std::cout <<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << std::endl;
  std::cout << "Author:" <<std::endl;
  std::cout << "Jacob E Bickus, 2021" << std::endl;
  std::cout << "MIT, NSE" << std::endl;
  std::cout << "jbickus@mit.edu" << std::endl;
  std::cout <<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << std::endl;
  return instance;
}

MantisROOT::MantisROOT(bool debug_in=false)
{
  debug = debug_in;
  if(debug)
    std::cout << "Set to Debugging Mode!" << std::endl;
  std::cout << "Run: Help() to see the list of available functions." << std::endl;
}

MantisROOT::~MantisROOT()
{}

void MantisROOT::Destroy()
{
  if(instance != 0)
  {
   delete instance;
   instance = 0;
  }
}

void MantisROOT::Help()
{
  std::cout << "Calls and Descriptions" << std::endl << std::endl;
  Show_CheckAngles();
  Show_CheckAngles_Description();
  std::cout << std::endl;

  Show_CheckDet();
  Show_CheckDet_Description();
  std::cout << std::endl;

  Show_CheckEvents();
  Show_CheckEvents_Description();
  std::cout << std::endl;

  Show_CheckIntObj();
  Show_CheckIntObj_Description();
  std::cout << std::endl;

  Show_CombineFiles();
  Show_CombineFiles_Description();
  std::cout << std::endl;

  Show_CopyTrees();
  Show_CopyTrees_Description();
  std::cout << std::endl;

  Show_CreateDetEfficiencyCurve();
  Show_CreateDetEfficiencyCurve_Description();
  std::cout << std::endl;

  Show_CreateScintillationDistribution();
  Show_CreateScintillationDistribution_Description();
  std::cout << std::endl;

  Show_CreateTKDE();
  Show_CreateTKDE_Description();
  std::cout << std::endl;

  Show_Energy2Wave();
  Show_Energy2Wave_Description();
  std::cout << std::endl;

  Show_GetInstance();
  std::cout << std::endl;

  Show_Help();
  Show_Help_Description();
  std::cout << std::endl;

  Show_Integral();
  Show_Integral_Description();
  std::cout << std::endl;

  Show_OpenFile();
  Show_OpenFile_Description();
  std::cout << std::endl;

  Show_PredictThickness();
  Show_PredictThickness_Description();
  std::cout << std::endl;

  Show_PrepareAnalysis();
  Show_PrepareAnalysis_Description();
  std::cout << std::endl;

  Show_PrepInputSpectrum();
  Show_PrepInputSpectrum_Description();
  std::cout << std::endl;

  Show_RebinHisto();
  Show_RebinHisto_Description();
  std::cout << std::endl;

  Show_Sampling();
  Show_Sampling_Description();
  std::cout << std::endl;

  Show_SimpleSampling();
  Show_SimpleSampling_Description();
  std::cout << std::endl;

  Show_Sig2Noise();
  Show_Sig2Noise_Description();
  std::cout << std::endl;

  Show_VarRebin();
  Show_VarRebin_Description();
  std::cout << std::endl;

  Show_Wave2Energy();
  Show_Wave2Energy_Description();
  std::cout << std::endl;

  Show_ZScore();
  Show_ZScore_Description();
  std::cout << std::endl;
}

void MantisROOT::Rebin(bool verbose, const char* inFile, const char* ObjName, const char* OutObjName,
  int nbins, double Emin = 0.0, double Emax=2.1, TCut cut1="NA",
  bool VarArray=false, double nrf_bin_width = -1., double non_nrf_bin_width =-1.)
{

  // Check to make sure file exists
  CheckFile(inFile);

  // Complete User Input Checks
  if(VarArray && (nrf_bin_width < 0. || non_nrf_bin_width < 0.))
  {
    std::cerr << "USER ERROR. Variable bin widths selected without user selecting "
                << "the magnitude of the bin widths. Exiting..." << std::endl;
    exit(1);
  }

  TFile *f = new TFile(inFile);
  f->cd();
  TTree *inObj;
  // Grab the Users TTree
  inObj = (TTree*) f->Get(ObjName);
  inObj->SetEstimate(-1);
  // Grab TTree Values
  Int_t nentries=0;
  if(cut1=="NA")
    nentries = inObj->Draw("Energy:Weight","","goff");
  else
    nentries = inObj->Draw("Energy:Weight",cut1,"goff");

  if(nentries <= 0)
    exit(1);

  if(verbose)
    inObj->Print();

  Double_t *energies = inObj->GetVal(0);
  Double_t *weights = inObj->GetVal(1);
  TH1D *hObj;
  // If User wants fixed bin widths
  if(!VarArray)
  {
    if(verbose)
      std::cout << "User did not select variable bin widths." << std::endl;
    hObj = new TH1D(OutObjName, "Weighted Energy Spectrum", nbins, Emin, Emax);
  }
  // User wants variable bin widths
  else
  {
    if(verbose)
      std::cout << "User selected variable bin widths." <<std::endl;
    // Find total number of bins
    double edge_counter = 0.;
    int nbins1 = Emin/(non_nrf_bin_width);
    int nbins2 = (Emax - Emin)/nrf_bin_width;
    Int_t tbins = nbins1 + nbins2;
    // create edges (dynamically sized array)
    Double_t* edges = new Double_t[tbins+1];
    // fill the edges for the first region
    if(verbose)
      std::cout << "Creating Edges for the first region..." << std::endl;

    for(int i=0;i<nbins1+1;++i)
    {
      edges[i] = edge_counter;
      edge_counter += non_nrf_bin_width;
    }
    // fill the edges for the second region
    edge_counter = edge_counter - non_nrf_bin_width;
    if(verbose)
      std::cout << "Creating Edges for the second region..." << std::endl;

    for(int i=nbins1;i<tbins+2;++i)
    {
      edges[i] = edge_counter;
      edge_counter += nrf_bin_width;
    }

    // edges complete now create new histogram
    hObj = new TH1D(OutObjName,"Weighted Energy Spectrum w/ Variable Bins",tbins,edges);

  }// end !VarArray

  // Prior to filling histogram set structure for storing bin errors
  hObj->Sumw2();

  if(verbose)
    std::cout << "Filling Histogram..." << std::endl;
  // Fill the new Histogram
  for(unsigned int i=0;i<nentries;++i)
  {
    hObj->Fill(energies[i],weights[i]);
  }

  // Write to OutFile
  std::string OutFileName;
  if(!VarArray)
    OutFileName = "rebinned_" + to_string(nbins) + "_" + (std::string)inFile;
  else
    OutFileName = "rebinned_Variable_binWidth_" + to_string(nrf_bin_width) + "_" + (std::string)inFile;

  TFile *fout;
  if(!gSystem->AccessPathName(OutFileName.c_str()))
    fout = new TFile(OutFileName.c_str(),"update");
  else
    fout = new TFile(OutFileName.c_str(),"recreate");

  fout->cd();
  hObj->Write();
  std::cout << "Rebinned Histogram written to: " << OutFileName << std::endl;
  fout->Close();

}

// *************************************************************************//
// *************************************************************************//
// *************************************************************************//

void MantisROOT::Rebin(const char* inFile,const char* ObjName,const char* OutObjName)
{
  // Check to make sure file exists
  CheckFile(inFile);

  std::string OutFileName = "converted_" + (std::string)inFile;
  if(!gSystem->AccessPathName(OutFileName.c_str()))
  {
    std::cout << "Rebinned File Already exists. Continue to rebin?" << std::endl;
    string response;
    std::cin >> response;

    if(!response.compare(0,1,"N"))
      exit(1);
    else
      std::cout << "Continuing rebinning..." << std::endl;

  }

  TFile *f = new TFile(inFile);
  f->cd();
  TTree *inObj;
  // Grab the Users TTree
  inObj = (TTree*) f->Get(ObjName);
  inObj->SetEstimate(-1);
  double Emax = inObj->GetMaximum("Energy");
  // Write TTree to histogram
  TH1D *hObj = new TH1D(OutObjName, "Weighted Energy Spectrum",100,0.,Emax);
  std::string tCommand = "Energy>>" + (std::string)OutObjName;
  inObj->Draw(tCommand.c_str(), "Weight","goff");

  TFile *fout;

  if(!gSystem->AccessPathName(OutFileName.c_str()))
    fout = new TFile(OutFileName.c_str(),"update");
  else
    fout = new TFile(OutFileName.c_str(),"recreate");

  fout->cd();
  hObj->Write();
  fout->Close();
} // end of Rebin Functions

void MantisROOT::hIntegral(TH1 *h)
{
  int nentries = h->GetEntries();
  TAxis *xaxis = h->GetXaxis();
  double hMean = h->GetMean();
  int nbins = h->GetNbinsX();

  double intSum = 0;
  for(unsigned int i=0;i<nbins;++i)
  {
    double xVal = xaxis->GetBinCenter(i);
    double binVal = h->GetBinContent(i);
    double integralVal = xVal*binVal;
    intSum += integralVal;
  }
  std::cout << h->GetTitle() << " BinCenter Method Sum: " << intSum << std::endl;
  std::cout << h->GetTitle() << " Mean Method Sum: " << nentries*hMean << std::endl;
  std::cout << h->GetTitle() << " Integral Method: " << h->Integral() << std::endl << std::endl;
}
double MantisROOT::hIntegral(TH1* h, int returnValue)
{
  int nentries = h->GetEntries();
  TAxis *xaxis = h->GetXaxis();
  double hMean = h->GetMean();
  int nbins = h->GetNbinsX();

  double intSum = 0;
  for(unsigned int i=0;i<nbins;++i)
  {
    double xVal = xaxis->GetBinCenter(i);
    double binVal = h->GetBinContent(i);
    double integralVal = xVal*binVal;
    intSum += integralVal;
  }

  if(returnValue == 1)
    return intSum;
  else if(returnValue == 2)
    return nentries*hMean;
  else if(returnValue == 3)
    return h->Integral();
  else
    return 0;

}

double MantisROOT::hIntegral(TTree *inObj, int returnValue)
{
  inObj->SetEstimate(-1);
  double Emax = inObj->GetMaximum("Energy");
  TH1D *e1 = new TH1D("e1",inObj->GetName(),100,0.,Emax);

  inObj->Draw("Energy>>e1","","goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }

  double theSum = hIntegral(e1, returnValue);
  delete e1;

  if(returnValue == 0)
    return intSum;
  else
    return theSum;

}
double MantisROOT::hIntegral(TTree *inObj, int returnValue, TCut cut1, double Emax)
{
  //std::cout << "MantisROOT::hIntegral -> Setting TCut to: " << cut1 << std::endl;
  inObj->SetEstimate(-1);
  TH1D *e1 = new TH1D("e1",inObj->GetName(),100,0.,Emax);

  if(cut1 == "NA")
    inObj->Draw("Energy>>e1","","goff");
  else
    inObj->Draw("Energy>>e1",cut1,"goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }

  double theSum = hIntegral(e1, returnValue);
  delete e1;

  if(returnValue == 0)
    return intSum;
  else
    return theSum;

}

void MantisROOT::hIntegral(TTree* inObj)
{
  inObj->SetEstimate(-1);
  double Emax = inObj->GetMaximum("Energy");
  TH1D *e1 = new TH1D("e1",inObj->GetName(),100,0.,Emax);

  inObj->Draw("Energy>>e1","","goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }

  std::cout << inObj->GetName() << " TTree Sum: " << intSum << std::endl;

  hIntegral(e1);
  if(debug)
    std::cout << "MantisROOT::hIntegral -> Deallocating Memory." << std::endl;
  delete e1; // avoids potential memory leak
}

double MantisROOT::hIntegralReturn(TTree* inObj, bool cut, TCut cut1="NA")
{
  inObj->SetEstimate(-1);

  if(cut)
  {
    if(debug)
      std::cout << "Cut " << cut1 << " placed!" << std::endl;
    inObj->Draw("Energy",cut1,"goff");
  }
  else
    inObj->Draw("Energy","","goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }
  return intSum;
}

double MantisROOT::hIntegralReturnWeightedCounts(TTree* inObj, double cut_energy=5e-6)
{
  inObj->SetEstimate(-1);
  TH1D* h = new TH1D("h","Det Integral",100,0.,cut_energy);

  if(debug)
    std::cout << "Cut " << cut_energy << " MeV placed!" << std::endl;
  inObj->Draw("Energy>>h","Weight","goff");
  double intSum = h->Integral();

  std::cout << "MantisROOT::hIntegralReturnWeightedCounts -> Deallocating Memory..." << std::endl;

  delete h;
  return intSum;

}

double MantisROOT::hIntegralReturnWeightedEnergy(TTree* inObj, double cut_energy=5e-6)
{
  inObj->SetEstimate(-1);
  if(debug)
    std::cout << "Cut " << cut_energy << " MeV placed!" << std::endl;
  int nentries = inObj->Draw("Energy:Weight","","goff");
  double *Energies = inObj->GetVal(0);
  double *Weights = inObj->GetVal(1);

  double intSum = 0;
  for(int i=0;i<nentries;++i)
  {
    intSum += Energies[i]*Weights[i];
  }
  if(debug)
    std::cout << "MantisROOT::hIntegralReturnWeightedEnergy -> Deallocating Memory..." << std::endl;

  return intSum;
}

void MantisROOT::hIntegral(TTree *inObj,TCut cut1)
{
  inObj->SetEstimate(-1);
  double Emax = inObj->GetMaximum("Energy");
  TH1D *e1 = new TH1D("e1",inObj->GetName(),100,0.,Emax);

  if(cut1 == "NA")
    inObj->Draw("Energy>>e1","","goff");
  else
    inObj->Draw("Energy>>e1",cut1,"goff");

  Double_t *energies = inObj->GetVal(0);
  int nentries = inObj->GetEntries();
  double intSum = 0;

  for(unsigned int i=0;i<nentries;++i)
  {
    intSum +=energies[i];
  }

  std::cout << inObj->GetName() << " TTree Sum: " << intSum << std::endl;

  hIntegral(e1);
  if(debug)
    std::cout << "MantisROOT::hIntegral -> Deallocating Memory..." << std::endl;
  delete e1; // avoids potential memory leak
}

void MantisROOT::hIntegral(const char* filename, const char* objName)
{
  CheckFile(filename);
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  bool confirm = f->cd();
  if(!confirm)
    exit(0);

  TTree *inObj;
  f->GetObject(objName,inObj);
  if(inObj == 0)
    exit(0);

  hIntegral(inObj);
  if(debug)
    std::cout << "MantisROOT::hIntegral -> Deallocating Memory..." << std::endl;
  delete inObj; // avoids potential memory leak
}

void MantisROOT::hIntegral(const char* filename, const char* objName, TCut cut1)
{
  CheckFile(filename);
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  bool confirm = f->cd();
  if(!confirm)
    exit(0);

  TTree *inObj;
  f->GetObject(objName,inObj);
  if(inObj == 0)
    exit(0);

  hIntegral(inObj, cut1);
  delete inObj; // avoids potential memory leak
}

void MantisROOT::hIntegral(const char* filename)
{
  // Doing the integral for chopOut, IntObjIn and DetInfo
  CheckFile(filename);
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  TTree *inChopOut, *inIntObjIn, *inDetInfo;
  f->GetObject("ChopOut",inChopOut);
  f->GetObject("IntObjIn",inIntObjIn);
  f->GetObject("DetInfo",inDetInfo);
  hIntegral(inChopOut);
  hIntegral(inIntObjIn);
  inDetInfo->SetEstimate(-1);
  TH1D *e1 = new TH1D("e1","DetInfo Histogram",100,0.,10e-6);
  inDetInfo->Draw("Energy>>e1","Energy<10e-6","goff");
  std::cout << "DetInfo Integral: " << e1->Integral() << std::endl;
  if(debug)
    std::cout << "MantisROOT::hIntegral -> Deallocating Memory..." << std::endl;
  delete e1;
  f->Close();
} // end of hIntegral Functions

void MantisROOT::CombineFiles(std::vector<string> filenames, std::vector<string> objects, const char* outfilename)
{
  for(int i=0;i<objects.size();++i)
  {
    TFile *f = new TFile(outfilename, "update");
    f->cd();
    TChain ch(objects[i].c_str());
    for(int i=0;i<filenames.size();++i)
      ch.Add(filenames[i].c_str());

    ch.Merge(f,1000);

    std::cout << objects[i] << " merged to " << outfilename << std::endl;
  }

}

void MantisROOT::CopyTrees(const char* filename, std::vector<string> noObjv, const char* outfilename)
{
  std::vector<bool> dataList = {0,0,0,0,0,0,0,0,0,0,0,0,0};
  std::vector<string> optList = {"Weight","Brem","ChopIn","ChopOut","NRF","AirIn",
                                  "IntObjIn","IntObjOut","Water","Cherenkov",
                                  "DetInfo","IncDetInfo", "Corrected_DetInfo", "Erroneous_DetInfo",
                                  "event_tree"};
  // check which trees to copy
  for(int i=0;i<noObjv.size();++i)
  {
    for(int j=0;j<optList.size();++j)
    {
      int counter = 0;
      if(!noObjv[i].compare(optList[j]))
      {
        dataList[j] = true;
        continue;
      }
      else
        counter++;

      if(counter == optList.size())
        std::cout << noObjv[i] << " option not found!" << std::endl;

    }
  }

  std::cout << "MantisROOT::CopyTrees -> Copying Tree Status: " << std::endl;
  for(int i=0;i<optList.size();++i)
    std::cout << "\t\t " << optList[i] << " = " << dataList[i] << std::endl;

  for(int i=1;i<optList.size();++i)
  {
    if(dataList[0])
    {
      if(dataList[i])
        CopyATree(filename, optList[i].c_str(), outfilename);
    }
    else
    {
      if(dataList[i])
        CopyATreeNoWeight(filename, optList[i].c_str(), outfilename);
    }
  }

  std::cout << "MantisROOT::CopyTrees -> Complete." << std::endl;
}

void MantisROOT::Sig2Noise(std::vector<string> filenames, string object, bool Weighted=false, bool Corrected=false, bool cut=false, TCut cut1="NA")
{
  if(!object.compare("IntObj"))
  {
    for(int i=0;i<filenames.size();++i)
    {
      std::cout << std::endl << "Signal to Noise Calculation for " << filenames[i] << std::endl;
      SNR_IntObj(filenames[i].c_str(), Weighted);
    }
  }
  else if(!object.compare("Det"))
  {
    for(int i=0;i<filenames.size();++i)
    {
      std::cout << std::endl << "Signal to Noise Calculation for " << filenames[i] << std::endl;
      SNR_Det(filenames[i].c_str(), Weighted, Corrected, cut, cut1);
    }
  }
  else if(!object.compare("Both"))
    for(int i=0;i<filenames.size();++i)
    {
      std::cout << std::endl << "Signal to Noise Calculation for " << filenames[i] << std::endl;
      std::cout << filenames[i] << " Interrogation Object Signal to Noise Calculating..." << std::endl;
      SNR_IntObj(filenames[i].c_str(), Weighted);
      std::cout << filenames[i] << " Detected Signal to Noise Calculating..." << std::endl;
      SNR_Det(filenames[i].c_str(), Weighted, Corrected, cut, cut1);
    }
  else
  {
    std::cerr << "Error: Object Not Found Input Options: " << std::endl
    << "IntObj" << std::endl << "Det" << std::endl << "Both" << std::endl;
    return;
  }

  std::cout << "MantisROOT::Sig2Noise -> Signal to Noise Ratio Analysis Complete." << std::endl;
}

void MantisROOT::ZScore(const char* file1, const char* file2, std::vector<string> objects, bool weighted=false)
{
  for(int i=0;i<objects.size();++i)
  {
    std::cout << std::endl << objects[i] << " ZScore Results: " << std::endl;
    ZTest(file1, file2, objects[i].c_str(), weighted);
  }

  std::cout << std::endl << "MantisROOT::ZScore -> Complete." << std::endl;
}

void MantisROOT::ZScore(double c1, double c2)
{
  double zscore = ZTest(c1,c2);
  std::cout << "ZScore Result: " << zscore << std::endl;
  std::cout << std::endl << "MantisROOT::ZScore -> Complete." << std::endl;
}

void MantisROOT::Integral(TTree* tree)
{
  hIntegral(tree);
  std::cout << "Integral Analysis Complete." << std::endl;
}

void MantisROOT::Integral(std::vector<TTree*> trees)
{
  for(int i=0;i<trees.size();++i)
    hIntegral(trees[i]);

  std::cout << "Integral Analysis Complete." << std::endl;
}

void MantisROOT::Integral(std::vector<TTree*> trees, TCut cut1)
{
  for(int i=0;i<trees.size();++i)
    hIntegral(trees[i], cut1);

  std::cout << "Integral Analysis Complete." << std::endl;
}

void MantisROOT::PredictThickness(std::vector<string> obj)
{
  for(int i=0;i<obj.size();++i)
    Rescale(obj[i].c_str());

  std::cout << "Thickness Prediction Analysis Complete." << std::endl;
}

void MantisROOT::PredictThickness(std::vector<string> obj, double Er)
{
  for(int i=0;i<obj.size();++i)
    Rescale(obj[i].c_str(),Er);

  std::cout << "Thickness Prediction Analysis Complete." << std::endl;
}

void MantisROOT::RebinHisto(std::vector<string> inFile, std::vector<string> ObjName,
                 std::vector<string> OutObjName, int nbins, double Emin,
                 double Emax)
{
  if(OutObjName.size() != ObjName.size())
  {
    std::cout << "Error ObjName and OutObjName vector inputs must be same length! Check your inputs!" << std::endl;
    exit(1);
  }
  for(int i=0;i<inFile.size();++i)
  {
    for(int j=0;j<ObjName.size();++j)
    {
      Rebin(false, inFile[i].c_str(), ObjName[j].c_str(), OutObjName[j].c_str(),
            nbins, Emin, Emax);
    }
  }
}

void MantisROOT::RebinHisto(std::vector<string> inFile, std::vector<string> ObjName,
                 std::vector<string> OutObjName, int nbins, double Emin,
                 double Emax, TCut cut1)
{
  if(OutObjName.size() != ObjName.size())
  {
    std::cout << "Error ObjName and OutObjName vector inputs must be same length! Check your inputs!" << std::endl;
    exit(1);
  }
  for(int i=0;i<inFile.size();++i)
  {
    for(int j=0;j<ObjName.size();++j)
    {
      Rebin(false, inFile[i].c_str(), ObjName[j].c_str(), OutObjName[j].c_str(),
            nbins, Emin, Emax, cut1);
    }
  }
}

void MantisROOT::VarRebin(std::vector<string> inFile, std::vector<string> ObjName,
                           std::vector<string> OutObjName, int nbins,
                           double Ecut1, double Ecut2,
                           TCut cut1, double binwidth1, double binwidth2)
{
  if(OutObjName.size() != ObjName.size())
  {
    std::cout << "Error ObjName and OutObjName vector inputs must be same length! Check your inputs!" << std::endl;
    exit(1);
  }
  for(int i=0;i<inFile.size();++i)
  {
    for(int j=0;j<ObjName.size();++j)
    {
      Rebin(false, inFile[i].c_str(), ObjName[j].c_str(), OutObjName[j].c_str(),
            nbins, Ecut1, Ecut2, cut1, true, binwidth1, binwidth2);
    }
  }
}

void MantisROOT::Show(string name="All", bool description=false)
{
  if(!name.compare("All") && description)
  {
    Help();
  }
  else if(!name.compare("All") && !description)
  {
    Show_CheckAngles();
    Show_CheckDet();
    Show_CheckEvents();
    Show_CheckIntObj();
    Show_CombineFiles();
    Show_CopyTrees();
    Show_CreateDetEfficiencyCurve();
    Show_CreateScintillationDistribution();
    Show_CreateTKDE();
    Show_Energy2Wave();
    Show_Help();
    Show_Integral();
    Show_OpenFile();
    Show_PredictThickness();
    Show_PrepareAnalysis();
    Show_PrepInputSpectrum();
    Show_RebinHisto();
    Show_Sampling();
    Show_Show();
    Show_Sig2Noise();
    Show_SimpleSampling();
    Show_VarRebin();
    Show_Wave2Energy();
    Show_ZScore();
  }
  else if(!name.compare("Help"))
  {
    Show_Help();
    if(description)
      Show_Help_Description();
  }
  else if(!name.compare("Show"))
  {
    Show_Show();
    if(description)
      Show_Show_Description();
  }
  else if(!name.compare("OpenFile"))
  {
    Show_OpenFile();
    if(description)
      Show_OpenFile_Description();
  }
  else if(!name.compare("CheckDet"))
  {
    Show_CheckDet();
    if(description)
      Show_CheckDet_Description();
  }
  else if(!name.compare("CopyTrees"))
  {
    Show_CopyTrees();
    if(description)
      Show_CopyTrees_Description();
  }
  else if(!name.compare("CombineFiles"))
  {
    Show_CombineFiles();
    if(description)
      Show_CombineFiles_Description();
  }
  else if(!name.compare("CreateDetEfficiencyCurve"))
  {
    Show_CreateDetEfficiencyCurve();
    if(description)
      Show_CreateDetEfficiencyCurve_Description();
  }
  else if(!name.compare("CreateTKDE"))
  {
    Show_CreateTKDE();
    if(description)
      Show_CreateTKDE_Description();
  }
  else if(!name.compare("CreateScintillationDistribution"))
  {
    Show_CreateScintillationDistribution();
    if(description)
      Show_CreateScintillationDistribution_Description();
  }
  else if(!name.compare("Energy2Wave"))
  {
    Show_Energy2Wave();
    if(description)
      Show_Energy2Wave_Description();
  }
  else if(!name.compare("Wave2Energy"))
  {
    Show_Wave2Energy();
    if(description)
      Show_Wave2Energy_Description();
  }
  else if(!name.compare("Sig2Noise"))
  {
    Show_Sig2Noise();
    if(description)
      Show_Sig2Noise_Description();
  }
  else if(!name.compare("ZScore"))
  {
    Show_ZScore();
    if(description)
      Show_ZScore_Description();
  }
  else if(!name.compare("Integral"))
  {
    Show_Integral();
    if(description)
      Show_Integral_Description();
  }
  else if(!name.compare("PredictThickness"))
  {
    Show_PredictThickness();
    if(description)
      Show_PredictThickness_Description();
  }
  else if(!name.compare("PrepareAnalysis"))
  {
    Show_PrepareAnalysis();
    if(description)
      Show_PrepareAnalysis_Description();
  }
  else if(!name.compare("PrepInputSpectrum"))
  {
    Show_PrepInputSpectrum();
    if(description)
      Show_PrepInputSpectrum_Description();
  }
  else if(!name.compare("RebinHisto"))
  {
    Show_RebinHisto();
    if(description)
      Show_RebinHisto_Description();
  }
  else if(!name.compare("VarRebin"))
  {
    Show_VarRebin();
    if(description)
      Show_VarRebin_Description();
  }
  else if(!name.compare("CheckEvents"))
  {
    Show_CheckEvents();
    if(description)
      Show_CheckEvents_Description();
  }
  else if(!name.compare("Sampling"))
  {
    Show_Sampling();
    if(description)
      Show_Sampling_Description();
  }
  else if(!name.compare("SimpleSampling"))
  {
    Show_SimpleSampling();
    if(description)
      Show_SimpleSampling_Description();
  }
  else if(!name.compare("CheckIntObj"))
  {
    Show_CheckIntObj();
    if(description)
      Show_CheckIntObj_Description();
  }
  else if(!name.compare("CheckAngles"))
  {
    Show_CheckAngles();
    if(description)
      Show_CheckAngles_Description();
  }
  else
    std::cout << "Error Function Not Found." << std::endl;
}

TFile* MantisROOT::OpenFile(const char* filename)
{
  return (new TFile(filename));
} // end of OpenFile function

void MantisROOT::CheckEvents(const char* filename, bool Weighted=false, bool Corrected=false)
{
  CheckFile(filename);
  time_t timer;
  time_t time_start = std::time(&timer);
  Compute(filename, time_start, Weighted, Corrected);
}

void MantisROOT::Compute(const char* fname, time_t time_start, bool Weighted, bool Corrected)
{
  int x =0;
  TFile *fMaincompute = new TFile(fname);
  fMaincompute->cd();
  string filename = fMaincompute->GetName();
  TTree *Cherenkov_in, *NRF_in, *DetInfo_in;
  fMaincompute->GetObject("Cherenkov",Cherenkov_in);
  fMaincompute->GetObject("NRF",NRF_in);

  if(Corrected)
    fMaincompute->GetObject("Corrected_DetInfo", DetInfo_in);
  else
    fMaincompute->GetObject("DetInfo",DetInfo_in);

  Cherenkov_in->SetEstimate(-1);
  NRF_in->SetEstimate(-1);
  DetInfo_in->SetEstimate(-1);

  if(debug)
    std::cout << "CheckEvents::Compute -> Objects Grabbed!" << std::endl;

  int nrf_entries = NRF_in->Draw("EventID","","goff");

  std::cout << "CheckEvents::Compute -> NRF Entries: " << nrf_entries << std::endl;

  double *nrfEvent = NRF_in->GetVal(0);
  std::vector<int> nrfEventv;
  // Write NRF Events to Vector
  for(int i=0; i<nrf_entries; ++i)
  {
    nrfEventv.push_back((int)nrfEvent[i]);
  }

  // Grab Cherenkov Events
  int cher_entries = Cherenkov_in->Draw("EventID","","goff");

  std::cout << "CheckEvents::Compute -> Cherenkov Entries: " << cher_entries << std::endl;

  double *cherEvent = Cherenkov_in->GetVal(0);

  std::vector<int> cherEventv;
  // Write Cherenkov Events to vector
  for(int i=0; i<cher_entries; ++i)
  {
    cherEventv.push_back((int)cherEvent[i]);
  }

  // Grab DetInfo Events
  int det_entries =0;

  if(Weighted)
    det_entries = DetInfo_in->Draw("EventID:Energy:CreatorProcess:Time:Weight", "CreatorProcess == \"Scintillation\" || CreatorProcess == \"Cerenkov\"","goff");
  else
    det_entries = DetInfo_in->Draw("EventID:Energy:CreatorProcess:Time","CreatorProcess == \"Scintillation\" || CreatorProcess == \"Cerenkov\"","goff");

  std::cout << "CheckEvents::Compute -> Total Number of Detected Optical Photon entries: "
            << det_entries << std::endl << std::endl;

  double *detEvent = DetInfo_in->GetVal(0);
  double *detEnergy = DetInfo_in->GetVal(1);

  std::cout << "MantisROOT::Compute -> Setting CreatorProcess Branch Address..." << std::endl;

  string *creators;
  DetInfo_in->SetBranchAddress("CreatorProcess",&creators);

  double *detTime = DetInfo_in->GetVal(3);
  double *detWeight = new double[(int)det_entries];

  if(Weighted)
    detWeight = DetInfo_in->GetVal(4);

  std::vector<int> detEventv;
  std::vector<double> detEnergyv, detWeightv, detTimev;
  std::vector<string> detCreatorv;

  for(int i=0; i<det_entries; ++i)
  {
    detEventv.push_back((int)detEvent[i]);
    DetInfo_in->GetEntry(i);
    detCreatorv.push_back(*creators);
    detEnergyv.push_back(detEnergy[i]);
    detTimev.push_back(detTime[i]);

    if(Weighted)
      detWeightv.push_back(detWeight[i]);
  }

  std::vector<int> nrf_to_cherEvents, nrf_to_cher_to_det_event;

  if(nrf_entries != 0 && cher_entries != 0 && det_entries != 0)
  {
    std::cout << "CheckEvents::Compute -> Finding NRF Events that Caused Optical Photons..." << std::endl;

    for(int i=0; i<nrf_entries; ++i)
    {
      // Grab nrf Event ID
      x = nrfEventv[i];
      // Check if NRF EventID is found in Cherenkov Vector
      auto exists = std::find(cherEventv.begin(),cherEventv.end(), x);

      if(exists != cherEventv.end())
        // if the eventID is found in cherenkov write to new vector
        nrf_to_cherEvents.push_back(x);
    }
    std::cout << "CheckEvents::Compute -> NRF to Optical Photon Number of Events Found: "
            << nrf_to_cherEvents.size() << std::endl;

    for(unsigned int i=0; i<nrf_to_cherEvents.size(); ++i)
    {
      // Check if nrf->cherenkov event ID is in detected
      x = nrf_to_cherEvents[i];
      auto exists = std::find(detEventv.begin(), detEventv.end(), x);
      if(exists != detEventv.end())
      {
        // if the eventID is found write to new vector
        nrf_to_cher_to_det_event.push_back(x);
      }
    }
    std::cout << "CheckEvents::Compute -> NRF to Optical Photons Detected Number of Events: "
            << nrf_to_cher_to_det_event.size() << std::endl;
  } // end if nrf_entries != 0 && cher_entries != 0

  std::cout << "CheckEvents::Compute -> Finding Total Number of NRF to Optical Photons Detected..." << std::endl;

  if(debug)
    std::cout << "CheckEvents::Compute -> Sorting DetInfo..." << std::endl;

  // Zipping Process for sorting
  std::vector<std::tuple<int, double, string, double>> zipped4; // No Weights
  std::vector<std::tuple<int, double, string, double, double>> zipped5; // weighted

  if(Weighted)
  {
    zip(detEventv, detEnergyv, detCreatorv, detTimev, detWeightv, zipped5);
    std::sort(zipped5.begin(),zipped5.end());
    unzip(zipped5, detEventv, detEnergyv, detCreatorv, detTimev, detWeightv);
  }
  else
  {
    zip(detEventv, detEnergyv, detCreatorv, detTimev, zipped4);
    std::sort(zipped4.begin(),zipped4.end());
    unzip(zipped4, detEventv, detEnergyv, detCreatorv, detTimev);
  }

  if(debug)
    std::cout << "MantisROOT::Compute -> DetInfo Sorted." << std::endl;

  int index = 0;
  int tmp_index = 0;
  std::vector<int> final_nrf_det_eventsv;
  std::vector<double> final_nrf_det_energiesv, final_nrf_det_timesv, final_nrf_det_weightsv;
  std::vector<string> final_nrf_det_creatorsv;

  std::cout << "MantisROOT::Compute-> NRF to Det EventID Vector Size: " << nrf_to_cher_to_det_event.size() << std::endl;
  std::cout << "MantisROOT::Compute -> DetInfo EventID Vector Size: " << detEventv.size() << std::endl;

  if(nrf_to_cher_to_det_event.size() > 0)
  {
    while(index < detEventv.size())
    {
      if(!debug)
        std::cout << "\r** Checking Entry: " << index << std::flush;
      else
        std::cout << "Checking Entry: " << index << std::endl;

      // Grab Det EventID
      x = detEventv[index];

      if(debug)
        std::cout << "X: " << x << std::endl;
      // Check if DetInfo EventID matches nrf_to_det_EventID
      auto exists = std::find(nrf_to_cher_to_det_event.begin(),nrf_to_cher_to_det_event.end(), x);

      // if found write event info to new vectors
      if(exists != nrf_to_cher_to_det_event.end())
      {
        final_nrf_det_eventsv.push_back(x);
        final_nrf_det_energiesv.push_back(detEnergyv[index]);
        final_nrf_det_creatorsv.push_back(detCreatorv[index]);
        final_nrf_det_timesv.push_back(detTimev[index]);

        if(Weighted)
          final_nrf_det_weightsv.push_back(detWeightv[index]);

        int counter = 0;
        while(counter >= 0)
        {
          counter++;
          tmp_index = index + counter;
          if(x == detEventv[index+counter])
          {
            final_nrf_det_eventsv.push_back(x);
            final_nrf_det_energiesv.push_back(detEnergyv[index+counter]);
            final_nrf_det_creatorsv.push_back(detCreatorv[index+counter]);
            final_nrf_det_timesv.push_back(detTimev[index+counter]);

            if(Weighted)
              final_nrf_det_weightsv.push_back(detWeightv[index+counter]);

          } // end of if(x == det_eventsv[index+counter])
          else
          {
            index = tmp_index;
            counter = -1;

            if(debug)
              std::cout << "While Else Index: " << index << std::endl;
          } // end of else
        } // end of while counter > 0
      } // end of if exists !=
      else
        index++;
    } // end of While index < detEvent.size()
  } // end of if nrf_to_cher_event > 0

  std::cout << std::endl << "MantisROOT::Compute -> Total Number of NRF Optical Photons Found: "
            << final_nrf_det_eventsv.size() << std::endl;

  fMaincompute->Close();

  if(debug)
    std::cout << "CheckEvents::Compute -> File Closed." << std::endl;

  // Open New TFile to write to
  string event_output_name = "w_events_" + filename;
  TFile *fin = new TFile(event_output_name.c_str(),"recreate");
  fin->cd();

  // Set up NRF to Cher to Det Tree
  int event;
  double energy, weight, thetimes;
  string theCreator;

  TTree *nrf_to_cher_to_det_tree = new TTree("event_tree","NRF Events that Lead to Cherenkov that were Detected");
  nrf_to_cher_to_det_tree->Branch("EventID",&event);
  nrf_to_cher_to_det_tree->Branch("Energy",&energy);
  nrf_to_cher_to_det_tree->Branch("CreatorProcess",&theCreator);
  nrf_to_cher_to_det_tree->Branch("Time",&thetimes);

  if(Weighted)
    nrf_to_cher_to_det_tree->Branch("Weight",&weight);

  // Fill nrf_to_cher_to_det Tree
  if(nrf_to_cher_to_det_event.size() > 0)
  {
    // Fill tree
    for(unsigned int i=0; i<final_nrf_det_eventsv.size(); ++i)
    {
      event = final_nrf_det_eventsv[i];
      energy = final_nrf_det_energiesv[i];
      theCreator = final_nrf_det_creatorsv[i];
      thetimes = final_nrf_det_timesv[i];

      if(Weighted)
        weight = final_nrf_det_weightsv[i];

      nrf_to_cher_to_det_tree->Fill();
    }
  }

  nrf_to_cher_to_det_tree->Write();

  std::cout << "CheckEvents::Compute -> TTrees Written to File: "
              << event_output_name << std::endl;

  if(debug)
    std::cout << "CheckEvents::Compute -> Closing Event File..." << std::endl;

  fin->Close();

  if(debug)
    std::cout << "CheckEvents::Compute -> Event File Closed." << std::endl;

  time_t timer2;
  time_t time_end = std::time(&timer2);
  std::cout << "CheckEvents::Compute -> Event Check took: " << std::difftime(time_end, time_start)
          << " seconds!" << std::endl << std::endl;

  std::cout << "CheckEvents::Compute -> Deallocating memory..." << std::endl;

} // end of Compute function

void MantisROOT::CopyATree(const char* filename, const char* tObj, const char* outfilename)
{
  CheckFile(filename);
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  TTree *oldTree;
  f->GetObject(tObj, oldTree);
  oldTree->SetBranchStatus("*",1);

  TFile *newfile;
  if(gSystem->AccessPathName(outfilename))
    newfile = new TFile(outfilename, "recreate");
  else
    newfile = new TFile(outfilename,"update");

  auto newtree = oldTree->CloneTree();

  newtree->Print();
  // write to new file
  newfile->Write();
  newfile->Close();
}// end of function CopyATree

void MantisROOT::CopyATreeNoWeight(const char* filename, const char* tObj, const char* outfilename)
{
  CheckFile(filename);
  TFile *f = new TFile(filename);
  if(f == 0)
    exit(0);

  TTree *oldTree;
  f->GetObject(tObj, oldTree);

  oldTree->SetBranchStatus("*",0);

  if(!string(tObj).compare("ChopIn"))
  {
    for(auto activeBranchName : {"Energy","EventID"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("ChopOut"))
  {
    for(auto activeBranchName : {"Energy","EventID","isNRF","Theta","Phi"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("NRF"))
  {
    for(auto activeBranchName : {"EventID","Energy", "Material", "ZPos"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("AirIn") || !string(tObj).compare(0,6,"IntObj"))
  {
    for(auto activeBranchName : {"Energy", "CreatorProcess", "Theta", "Phi", "Time", "EventID"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("Water"))
  {
    for(auto activeBranchName : {"Energy","CreatorProcess","EventID", "TrackID"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("Cherenkov"))
  {
    for(auto activeBranchName : {"Energy","EventID", "NumSecondaries", "Time"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("DetInfo"))
  {
    for(auto activeBranchName : {"EventID", "Energy", "CreatorProcess", "Time", "X", "Y"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("IncDetInfo"))
  {
    for(auto activeBranchName : {"EventID", "Energy", "DetProcess"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("Corrected_DetInfo"))
  {
    for(auto activeBranchName :{"EventID", "Energy", "CreatorProcess", "Time"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("Erroneous_DetInfo"))
  {
    for(auto activeBranchName :{"EventID"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else if(!string(tObj).compare("event_tree"))
  {
    for(auto activeBranchName :{"EventID","Energy","CreatorProcess","Time"})
    {
      oldTree->SetBranchStatus(activeBranchName,1);
    }
  }
  else
  {
    std::cerr << "ERROR: Object Name not found." << std::endl;
    exit(1);
  }

  TFile *newfile;
  if(gSystem->AccessPathName(outfilename))
    newfile = new TFile(outfilename, "recreate");
  else
    newfile = new TFile(outfilename,"update");

  auto newtree = oldTree->CloneTree();

  newtree->Print();
  // write to new file
  newfile->Write();
  newfile->Close();
} // end of CopyTreeNoWeight

void MantisROOT::SNR_IntObj(const char* inFile, bool Weighted)
{
  // Check to make sure file exists
  CheckFile(inFile);

  TFile *f = new TFile(inFile);
  f->cd();
  TTree *aIntObjIn;
  f->GetObject("IntObjIn", aIntObjIn);

  if(aIntObjIn !=0)
  {
    aIntObjIn->Print();
    double inMax = aIntObjIn->GetMaximum("Energy");
    aIntObjIn->SetEstimate(-1);

    TH1D* e11 = new TH1D("e11","IntObjIn NRF Histogram",100,1.73350, 1.73360);
    TH1D* e10 = new TH1D("e10","IntObjIn NRF Histogram",100,1.65620,1.65630);
    TH1D* e12 = new TH1D("e12","IntObjIn NRF Histogram",100,1.81520,1.81530);
    TH1D* e13 = new TH1D("e13","IntObjIn NRF Histogram",100,1.86230,1.86240);
    TH1D* e14 = new TH1D("e14","IntObjIn NRF Histogram",100,2.00615,2.00625);

    TH1D* eT = new TH1D("eT","IntObjIn Histogram",100,0.0,inMax);

    std::cout << "MantisROOT::SNR_IntObj -> Drawing IntObjIn NRF Histograms..." << std::endl;
    if(Weighted)
    {
      aIntObjIn->Draw("Energy>>e10","Weight","goff");
      aIntObjIn->Draw("Energy>>e11","Weight","goff");
      aIntObjIn->Draw("Energy>>e12","Weight","goff");
      aIntObjIn->Draw("Energy>>e13","Weight","goff");
      aIntObjIn->Draw("Energy>>e14","Weight","goff");
    }
    else
    {
      aIntObjIn->Draw("Energy>>e10","","goff");
      aIntObjIn->Draw("Energy>>e11","","goff");
      aIntObjIn->Draw("Energy>>e12","","goff");
      aIntObjIn->Draw("Energy>>e13","","goff");
      aIntObjIn->Draw("Energy>>e14","","goff");
    }

    std::cout << "Drawing IntObjIn and IntObjOut Histograms..." << std::endl;

    if(Weighted)
      aIntObjIn->Draw("Energy>>eT","Weight","goff");
    else
      aIntObjIn->Draw("Energy>>eT","","goff");

    // start integral 5eV prior to energy level peak
    double eStart[] = {1.6562312,1.7335419,1.8152525,1.8623129,2.0061941};
    double eEnd[5];

    for(int i=0;i<5;++i)
      eEnd[i] = eStart[i] + 0.00001; // add 10 eV or 5eV above energy level peak

    int binStart = e10->GetXaxis()->FindBin(eStart[0]);
    int binEnd = e10->GetXaxis()->FindBin(eEnd[0]);
    double inSignal[5];
    inSignal[0] = e10->Integral(binStart, binEnd);
    binStart = e11->GetXaxis()->FindBin(eStart[1]);
    binEnd = e11->GetXaxis()->FindBin(eEnd[1]);
    inSignal[1] = e11->Integral(binStart,binEnd);
    binStart = e12->GetXaxis()->FindBin(eStart[2]);
    binEnd = e12->GetXaxis()->FindBin(eEnd[2]);
    inSignal[2] = e12->Integral(binStart,binEnd);
    binStart = e13->GetXaxis()->FindBin(eStart[3]);
    binEnd = e13->GetXaxis()->FindBin(eEnd[3]);
    inSignal[3] = e13->Integral(binStart,binEnd);
    binStart = e14->GetXaxis()->FindBin(eStart[4]);
    binEnd = e14->GetXaxis()->FindBin(eEnd[4]);
    inSignal[4] = e14->Integral(binStart,binEnd);

    double tSignalin = 0;

    for(int i=0;i<5;++i)
    {
      std::cout << "MantisROOT::SNR_IntObj -> IntObjIn " << eStart[i] << " MeV Signals: \t" << inSignal[i] << std::endl;
      tSignalin += inSignal[i];
    }

    std::cout << "MantisROOT::SNR_IntObj -> Total IntObjIn Signal: \t" << tSignalin << std::endl;

    double inNoise = eT->Integral();
    std::cout << "MantisROOT::SNR_IntObj -> IntObjIn Noise: \t" << inNoise << std::endl;

    std::cout << "MantisROOT::SNR_IntObj -> The NRF photons are removed from the beam in the chopper wheel stage."
              << std::endl
              << "A lower SNR is better because you want the signal from NRF to be minimized."
              << std::endl;
    std::cout << "MantisROOT::SNR_IntObj -> IntObjIn SNR: \t" << tSignalin/sqrt(inNoise) << std::endl;
  }
  else
    std::cerr << "MantisROOT::SNR_IntObj -> ERROR IntObjIn Not Found in " << inFile << std::endl;
}// end of SNR function

void MantisROOT::SNR_Det(const char* inFile, bool Weighted, bool Corrected, bool cut, TCut cut1="NA")
{
  // Check to make sure file exists
  CheckFile(inFile);

  // Open the file
  TFile *mainf = new TFile(inFile);
  mainf->cd();

  TTree *eventT;
  mainf->GetObject("event_tree",eventT);
  double eventCounts, eventEnergy;
  if(!Weighted)
  {
    if(debug)
      std::cout << "MantisROOT::SNR_Det -> !Weighted hIntegralReturn" << std::endl;
    eventCounts = eventT->GetEntries();
    eventEnergy = hIntegralReturn(eventT,false);
  }
  else
  {
    if(debug)
      std::cout << "MantisROOT::SNR_Det-> Weighted hIntegralReturnWeighted" << std::endl;

    eventCounts = hIntegralReturnWeightedCounts(eventT);
    eventEnergy = hIntegralReturnWeightedEnergy(eventT);
  }

  std::cout << "MantisROOT::SNR_Det -> Detected Counts from NRF: " << eventCounts << std::endl;
  std::cout << "MantisROOT::SNR_Det -> Detected Energy from NRF: " << eventEnergy << " MeV" << std::endl;

  TTree* detT;

  if(Corrected)
    mainf->GetObject("Corrected_DetInfo",detT);
  else
    mainf->GetObject("DetInfo",detT);

  double counts, energy;
  if(!Weighted)
  {
    counts = detT->GetEntries();
    energy = hIntegralReturn(detT,cut,cut1);
  }
  else
  {
    counts = hIntegralReturnWeightedCounts(detT);
    energy = hIntegralReturnWeightedEnergy(detT);
  }

  std::cout << "MantisROOT::SNR_Det -> Total Detected Counts: " << counts << std::endl;
  std::cout << "MantisROOT::SNR_Det -> Total Detected Energy: " << energy << " MeV" << std::endl;

  std::cout << "MantisROOT::SNR_Det -> Counts SNR: " << eventCounts/sqrt(counts) << std::endl;

  if(debug)
    std::cout << "MantisROOT::SNR_Det -> Closing Main File..." << std::endl;

  mainf->Close();

  if(debug)
    std::cout << "MantisROOT::SNR_Det -> Main File Closed." << std::endl;

}

double MantisROOT::ZTest(double c1, double c2)
{
  double zscore = abs(c1 - c2)/(sqrt(pow(sqrt(c1),2) + pow(sqrt(c2),2)));
  //std::cout << "The Z-Score: " << zscore << std::endl;
  return zscore;
}

void MantisROOT::ZTest(const char* file1, const char* file2, const char* inObj, bool weighted=false)
{
  CheckFile(file1);
  CheckFile(file2);

  TFile *f = new TFile(file1);
  if(f != 0)
    f->cd();
  else
    return;

  TTree *inTree;
  f->GetObject(inObj, inTree);
  double c1,c11,c12,c13;
  if(!string(inObj).compare("DetInfo") || !string(inObj).compare("Corrected_DetInfo"))
  {
    TCut detCut;

    if(weighted)
      detCut = "Weight";
    else
      detCut = "Energy < 5e-6";

    c1  = hIntegral(inTree,0,detCut,5e-6);
    c11 = hIntegral(inTree,1,detCut,5e-6);
    c12 = hIntegral(inTree,2,detCut,5e-6);
    c13 = hIntegral(inTree,3,detCut,5e-6);
  }
  else if(!string(inObj).compare("IntObjIn"))
  {
    TCut intCut = "CreatorProcess == \"beam\"";

    if(debug)
      std::cout << "MantisROOT::ZTest -> Setting TCut for IntObjIn: " << intCut << std::endl;

    c1  = hIntegral(inTree,0,intCut,2.1);
    c11 = hIntegral(inTree,1,intCut,2.1);
    c12 = hIntegral(inTree,2,intCut,2.1);
    c13 = hIntegral(inTree,3,intCut,2.1);
  }
  else
  {
    if(weighted)
    {
      c1  = hIntegral(inTree, 0, "Weight", 2.1);
      c11 = hIntegral(inTree, 1, "Weight", 2.1);
      c12 = hIntegral(inTree, 2, "Weight", 2.1);
      c13 = hIntegral(inTree, 3, "Weight", 2.1);
    }
    else
    {
      c1  = hIntegral(inTree,0);
      c11 = hIntegral(inTree,1);
      c12 = hIntegral(inTree,2);
      c13 = hIntegral(inTree,3);
    }
  }

  delete inTree;
  f->Close();

  TFile *f1 = new TFile(file2);
  if(f1 != 0)
    f1->cd();
  else
    return;

  TTree *inTree2;
  f1->GetObject(inObj,inTree2);
  double c2,c21,c22,c23;
  if(!string(inObj).compare("DetInfo"))
  {
    TCut detCut = "Energy<5e-6";

    if(debug)
      std::cout << "MantisROOT::ZTest -> Setting TCut for DetInfo: " << detCut << std::endl;

    c2  = hIntegral(inTree2,0,detCut,5e-6);
    c21 = hIntegral(inTree2,1,detCut,5e-6);
    c22 = hIntegral(inTree2,2,detCut,5e-6);
    c23 = hIntegral(inTree2,3,detCut,5e-6);
  }
  else if(!string(inObj).compare("IntObjIn"))
  {
    TCut intCut = "CreatorProcess == \"beam\"";

    if(debug)
      std::cout << "MantisROOT::ZTest -> Setting TCut for IntObjIn: " << intCut << std::endl;

    c2  = hIntegral(inTree2,0,intCut,2.1);
    c21 = hIntegral(inTree2,1,intCut,2.1);
    c22 = hIntegral(inTree2,2,intCut,2.1);
    c23 = hIntegral(inTree2,3,intCut,2.1);
  }
  else
  {
    c2  = hIntegral(inTree2,0);
    c21 = hIntegral(inTree2,1);
    c22 = hIntegral(inTree2,2);
    c23 = hIntegral(inTree2,3);
  }


  std::cout << "TTree Method Z-Score: " << ZTest(c1,c2) << std::endl;
  std::cout << "Bin Center Method Z-Score: " << ZTest(c11,c21) << std::endl;
  std::cout << "Mean Method Z-Score: " << ZTest(c12,c22) << std::endl;
  std::cout << "Integral Method Z-Score: " << ZTest(c13,c23) << std::endl;

  delete inTree2;
  f1->Close();


} // end of ZTest functions

void MantisROOT::Rescale(const char* inObj, double Er)
{
  std::cout << "Input the Scale Base Filename:" << std::endl;
  string filename;
  std::cin >> filename;
  std::vector<std::string> user_files;
  std::vector<float> user_thick;
  std::vector<TFile*> tfilesv;

  // check to make sure file exists
  CheckFile(filename.c_str());

  // Handle the Base File
  user_files.push_back(filename);
  TFile *base = new TFile(filename.c_str());
  tfilesv.push_back(base);

  std::cout << "Input the Scale Base Thickness:" << std::endl;
  float b_thick;
  std::cin >> b_thick;
  user_thick.push_back(b_thick);

  string response = "Y";
  float thick;

  // Handle Files to be scaled
  while(!strcmp(response.c_str(),"Y"))
  {
    std::cout << "Input Filename to Scale: " << std::endl;
    std::cin >> filename;
    user_files.push_back(filename);

    CheckFile(filename.c_str());

    TFile *data_file = TFile::Open(filename.c_str());
    tfilesv.push_back(data_file);
    std::cout << "Input Thickness: " << std::endl;
    std::cin >> thick;
    user_thick.push_back(thick);
    std::cout << "Would you like to load another file?" << std::endl;
    std::cin >> response;
  }

  base->cd();
  double lowE = Er - 0.0001;
  double highE = Er + 0.0001;

  TH1D *hBase = new TH1D("hBase","Scale Base Histogram",60,lowE,highE);

  TTree *base_tInObj;
  base->GetObject(inObj,base_tInObj);
  base_tInObj->SetEstimate(-1);
  double Emax = base_tInObj->GetMaximum("Energy");
  TH1D *oghBase = new TH1D("theBase","Full Base Histogram",100,0.,Emax);
  base_tInObj->Draw("Energy>>theBase","Weight","goff");
  std::vector<TH1D*> ogOut;
  ogOut.push_back(oghBase);
  base_tInObj->Draw("Energy>>hBase","Weight","goff");
  std::vector<TH1D*> hOut;
  hOut.push_back(hBase);

  // based on 60 bins in histograms normalize to background
  std::vector<double> binValues;
  for(int i=0;i<60;++i)
  {
    binValues.push_back(hBase->GetBinContent(i));
  }

  std::cout << "Base set." << std::endl;

  std::cout << "Creating Histograms..." << std::endl;
  TTree *scale_tInObj;

  TH1D *hIn = new TH1D("hIn","Scaled Histogram",60,lowE,highE);
  for(int i=1;i<tfilesv.size();++i)
  {
    TFile *scaleFile = tfilesv[i];
    scaleFile->cd();
    //scaleFile->ls();
    scaleFile->GetObject(inObj,scale_tInObj);
    if(scale_tInObj !=0)
      std::cout << scaleFile->GetName() << " " << inObj << " grabbed."
                  << std::endl;
    else
    {
      std::cerr << "Error Object " << inObj << " not found in file: "
                  << scaleFile->GetName() << std::endl;
      exit(1);
    }

    TH1D *hIn = new TH1D("hIn","Scaled Histogram",60,lowE,highE);
    TH1D *oghIn = new TH1D("theH","Full Histogram to be Scaled",100,0.,Emax);
    scale_tInObj->SetEstimate(-1);
    scale_tInObj->Draw("Energy>>theH","Weight","goff");
    string fullhname = "full_" + std::to_string(i);
    ogOut.push_back(oghIn);
    ogOut[i]->SetName(fullhname.c_str());

    int nentries = scale_tInObj->Draw("Energy:Weight","","goff");
    Double_t* energies = scale_tInObj->GetVal(0);
    Double_t* weights = scale_tInObj->GetVal(1);

    for(unsigned int i=0;i<nentries;++i)
    {
      hIn->Fill(energies[i],weights[i]);
    }

    hOut.push_back(hIn);
    string hname = "Scaled_" + std::to_string(i);
    string htitle = "Scaled_" + user_files[i];
    hOut[i]->SetName(hname.c_str());
    hOut[i]->SetTitle(htitle.c_str());
    //scaleFile->Close();
  }

  std::cout << "Histograms created." << std::endl;
  for(int i=0;i<hOut.size();++i)
    hOut[i]->Print();

  std::cout << "Scaling Histograms." << std::endl;
  double newbinValue = 0;
  std::vector<TH1D*> outputHisto;
  for(int j=1;j<hOut.size();++j)
  {
    for(int i=0;i<60;++i)
    {
      if(binValues[i] == 0)
        newbinValue = 0;
      else
        newbinValue = binValues[i]*pow(hOut[j]->GetBinContent(i)/binValues[i],1./user_thick[j]);

      hOut[j]->SetBinContent(i, newbinValue);

    }
    std::cout << "Histogram Scaled: " <<std::endl;
    hOut[j]->Print();
  }

  // Draw Histograms to canvas
  TCanvas *c0 = new TCanvas();
  c0->cd();
  gPad->SetTicks(1,1);
  hOut[0]->SetLineColor(kBlack);
  std::cout << "Histogram from " << user_files[0]
              << " line color set to: Black" << std::endl;
  hOut[0]->Draw("h");

  std::vector<double> last_colors;
  last_colors.push_back(0);
  int histo_color = 0;
  string histo_color_label;

  for(int i=1;i<hOut.size();++i)
  {
    bool check = false;
    while(!check)
    {
        histo_color = rand() % 9 + 2;
        if(count(last_colors.begin(),last_colors.end(),histo_color) !=0)
        {
            check = false;
        }
        else
        {
            last_colors.push_back(histo_color);
            check = true;
        }
    }
    hOut[i]->SetLineColor(histo_color);
    switch(histo_color)
    {
        case 2:
            histo_color_label = "RED";
            break;
        case 3:
            histo_color_label = "LIGHT GREEN";
            break;
        case 4:
            histo_color_label = "DARK BLUE";
            break;
        case 5:
            histo_color_label = "YELLOW";
            break;
        case 6:
            histo_color_label = "PINK";
            break;
        case 7:
            histo_color_label = "LIGHT BLUE/CYAN";
            break;
        case 8:
            histo_color_label = "MEDIUM GREEN";
            break;
        case 9:
            histo_color_label = "PURPLE";
            break;
        default:
            histo_color_label = "BLACK";
    }
    cout << "Histogram from " << user_files[i] << " line color set to: " << histo_color_label << endl;

    hOut[i]->Draw("h,SAME");
  }

  std::cout << "All Histograms drawn." << std::endl;
  c0->SaveAs("Rescaled_Output.png");
  // Save to file
  TFile *fout = new TFile("thick_rescale.root","recreate");
  fout->cd();

  for(int i=0;i<hOut.size();++i)
  {
    ogOut[i]->Write();
    hOut[i]->Write();
  }

  std::cout << "Histograms saved." << std::endl;

} // end of function


// *************************************************************************** //
// *************************************************************************** //
// *************************************************************************** //

void MantisROOT::Rescale(const char* inObj)
{
  std::cout << "Input the Scale Base Filename:" << std::endl;
  string filename;
  std::cin >> filename;
  std::vector<std::string> user_files;
  std::vector<float> user_thick;
  std::vector<TFile*> tfilesv;

  // check to make sure file exists
  CheckFile(filename.c_str());

  // Handle the Base File
  user_files.push_back(filename);
  TFile *base = new TFile(filename.c_str());
  tfilesv.push_back(base);

  std::cout << "Input the Scale Base Thickness:" << std::endl;
  float b_thick;
  std::cin >> b_thick;
  user_thick.push_back(b_thick);

  string response = "Y";
  float thick;

  // Handle Files to be scaled
  while(!strcmp(response.c_str(),"Y"))
  {
    std::cout << "Input Filename to Scale: " << std::endl;
    std::cin >> filename;
    user_files.push_back(filename);

    CheckFile(filename.c_str());

    TFile *data_file = TFile::Open(filename.c_str());
    tfilesv.push_back(data_file);
    std::cout << "Input Thickness: " << std::endl;
    std::cin >> thick;
    user_thick.push_back(thick);
    std::cout << "Would you like to load another file?" << std::endl;
    std::cin >> response;
  }

  bool same_rescale = false;

  if(!user_files[0].compare(user_files[1]))
    same_rescale = true;

  if(same_rescale)
    std::cout << "Rescaling the same histogram." << std::endl;

  base->cd();

  // Create the base histogram to scale everything else to
  TTree *base_tInObj;
  base->GetObject(inObj,base_tInObj);
  base_tInObj->SetEstimate(-1);
  double Emax = base_tInObj->GetMaximum("Energy");
  TH1D *hBase = new TH1D("hBase","Scale Base Histogram",100,0.,Emax);
  base_tInObj->Draw("Energy>>hBase","Weight","goff");
  std::vector<TH1D*> hOut;
  hOut.push_back(hBase);

  // based on 100 bins in histograms normalize to background
  std::vector<double> binValues;

  for(int i=0;i<100;++i)
  {
    binValues.push_back(hBase->GetBinContent(i));
  }

  std::cout << "Base set." << std::endl;

  std::cout << "Creating Histograms..." << std::endl;
  TTree *scale_tInObj;

  TH1D *hIn = new TH1D("hIn","Scaled Histogram",100,0.,Emax);
  for(int i=1;i<tfilesv.size();++i)
  {
    TFile *scaleFile = tfilesv[i];
    scaleFile->cd();
    //scaleFile->ls();
    scaleFile->GetObject(inObj,scale_tInObj);
    if(scale_tInObj !=0)
      std::cout << scaleFile->GetName() << " " << inObj << " grabbed."
                  << std::endl;
    else
    {
      std::cerr << "Error Object " << inObj << " not found in file: "
                  << scaleFile->GetName() << std::endl;
      exit(1);
    }

    TH1D *hIn = new TH1D("theH","Full Histogram to be Scaled",100,0.,Emax);
    scale_tInObj->SetEstimate(-1);

    int nentries = scale_tInObj->Draw("Energy:Weight","","goff");
    Double_t* energies = scale_tInObj->GetVal(0);
    Double_t* weights = scale_tInObj->GetVal(1);

    for(unsigned int i=0;i<nentries;++i)
    {
      hIn->Fill(energies[i],weights[i]);
    }

    hOut.push_back(hIn);
    string hname = "Scaled_" + std::to_string(i);
    string htitle = "Scaled_" + user_files[i];
    hOut[i]->SetName(hname.c_str());
    hOut[i]->SetTitle(htitle.c_str());
  }

  std::cout << "Histograms created." << std::endl;
  for(int i=0;i<hOut.size();++i)
    hOut[i]->Print();

  std::cout << "Scaling Histograms." << std::endl;
  double newbinValue = 0;
  std::vector<TH1D*> outputHisto;
  for(int j=1;j<hOut.size();++j)
  {
    for(int i=0;i<100;++i)
    {
      if(binValues[i] == 0)
        newbinValue = 0;
      else
      {
        if(same_rescale)
          newbinValue = binValues[i]*user_thick[0]/user_thick[j];
        else
          newbinValue = binValues[i]*pow(hOut[j]->GetBinContent(i)/binValues[i],1./user_thick[j]);
      }
      hOut[j]->SetBinContent(i, newbinValue);

    }
    std::cout << "Histogram Scaled: " <<std::endl;
    hOut[j]->Print();
  }

  // Draw Histograms to canvas
  TCanvas *c0 = new TCanvas();
  c0->cd();
  gPad->SetTicks(1,1);
  hOut[0]->SetLineColor(kBlack);
  std::cout << "Histogram from " << user_files[0]
              << " line color set to: Black" << std::endl;
  hOut[0]->Draw("h");

  std::vector<double> last_colors;
  last_colors.push_back(0);
  int histo_color = 0;
  string histo_color_label;

  for(int i=1;i<hOut.size();++i)
  {
    bool check = false;
    while(!check)
    {
        histo_color = rand() % 9 + 2;
        if(count(last_colors.begin(),last_colors.end(),histo_color) !=0)
        {
            check = false;
        }
        else
        {
            last_colors.push_back(histo_color);
            check = true;
        }
    }
    hOut[i]->SetLineColor(histo_color);
    switch(histo_color)
    {
        case 2:
            histo_color_label = "RED";
            break;
        case 3:
            histo_color_label = "LIGHT GREEN";
            break;
        case 4:
            histo_color_label = "DARK BLUE";
            break;
        case 5:
            histo_color_label = "YELLOW";
            break;
        case 6:
            histo_color_label = "PINK";
            break;
        case 7:
            histo_color_label = "LIGHT BLUE/CYAN";
            break;
        case 8:
            histo_color_label = "MEDIUM GREEN";
            break;
        case 9:
            histo_color_label = "PURPLE";
            break;
        default:
            histo_color_label = "BLACK";
    }
    cout << "Histogram from " << user_files[i] << " line color set to: " << histo_color_label << endl;

    hOut[i]->Draw("h,SAME");
  }

  std::cout << "All Histograms drawn." << std::endl;
  c0->SaveAs("Rescaled_Output.png");
  // Save to file
  TFile *fout = new TFile("thick_rescale.root","recreate");
  fout->cd();

  for(int i=0;i<hOut.size();++i)
    hOut[i]->Write();

  std::cout << "Histograms saved." << std::endl;

}// end of function
TH1D* MantisROOT::BuildBrem(const char* bremInputFilename, double deltaE, bool checkZero)
{
  std::cout << "MantisROOT::BuildBrem -> Building Brem Distribution..." << std::endl;
  TFile *f = TFile::Open(bremInputFilename);
  bool confirm = f->cd();
  if(!confirm)
    exit(10);
  TTree *ChopperData;
  f->GetObject("Brem", ChopperData);
  //ChopperData->Print();

  double Emax = ChopperData->GetMaximum("Energy");

  Int_t nbins = Emax/deltaE;

  TH1D *tmp_hBrems = new TH1D("tmp_hBrems","Temporary Bremsstrahlung Data",250, 0.,Emax);
  std::cout << "MantisROOT::BuildBrem -> Grabbing Brem Data..." << std::endl;
  ChopperData->Draw("Energy>>tmp_hBrems","","goff");
  //tmp_Brems->Smooth(4);
  std::cout << "MantisROOT::BuildBrem -> Data Grabbed." << std::endl;
  //std::cout << "MantisROOT::BuildBrem -> Rebinning Brem Data..." << std::endl;
  TH1D* hBrems = new TH1D("hBrems","Final Bremsstrahlung Data",nbins, 0.,Emax);

  for(int i=1;i<=nbins;++i)
  {
    hBrems->SetBinContent(i,tmp_hBrems->GetBinContent(tmp_hBrems->GetNbinsX()*(i-1)/nbins+1));
  }

  hBrems->Scale(1.0/hBrems->Integral());
  //std::cout << "MantisROOT::BuildBrem -> Rebin Complete." << std::endl;
  if(checkZero)
  {
    std::cout << "MantisROOT::BuildBrem -> Checking for zeros..." << std::endl;
    bool zero_present = false;
    for(unsigned int i=0;i<nbins;++i)
    {
      int count = 0;
      if(hBrems->GetBinContent(i) == 0)
      {
        zero_present = true;
        while(hBrems->GetBinContent(i-1-count) == 0)
          count++;
        hBrems->SetBinContent(i,hBrems->GetBinContent(i-1-count));
      }
    }
    if(zero_present)
      std::cout << "MantisROOT::BuildBrem -> Zeros Found and Corrected." << std::endl;
    else
      std::cout << "MantisROOT::BuildBrem -> No Zeros Found Correction not needed." << std::endl;
  }
  std::cout << "MantisROOT::BuildBrem -> Brem Distribution Created." << std::endl;
  return hBrems;
}

double MantisROOT::ReturnBremMax(const char* bremInputFilename)
{
  std::cout << "MantisROOT::ReturnBremMax -> Searching Max Energy..." << std::endl;
  TFile *f = TFile::Open(bremInputFilename);
  bool confirm = f->cd();
  if(!confirm)
    exit(10);
  TTree *ChopperData;
  f->GetObject("Brem", ChopperData);
  //ChopperData->Print();

  double Emax = ChopperData->GetMaximum("Energy");
  std::cout << "MantisROOT::ReturnBremMax -> Max Found: " << Emax << std::endl;
  return Emax;
}

TH1D* MantisROOT::BuildSimpleSample(const char* bremInputFilename, double deltaE, double cut_energy, double weight)
{
  double Emax = ReturnBremMax(bremInputFilename);

  int nbins = Emax/deltaE;
  TH1D *hSample = new TH1D("hSample", "hSample", nbins, 0., Emax);
  double theWeight = 1./weight;

  if(debug)
  {
    std::cout << "MantisROOT::BuildSimpleSample -> Weights below: " << cut_energy << " set to: "
    << theWeight << std::endl;
  }
	// create the sampling distribution
	// user can adjust relative scales in SetBinContent
	for (int i = 1; i <= nbins; ++i)
  {
		double e = hSample->GetBinCenter(i);

		if (e < cut_energy)
			hSample->SetBinContent(i, theWeight);
		else
			hSample->SetBinContent(i, 1);
	}


	// normalize hSample so that its integral is 1
	hSample->Scale(1.0/(hSample->Integral()));
  std::cout << "MantisROOT::BuildSimpleSample -> Sampling Distribution Created." << std::endl;
  return hSample;
}

void MantisROOT::WriteSampling(TH1D* hBrems, TH1D* hSample, TGraph* gBrems, TGraph* gSample, double bin_width)
{
  std::cout << "MantisROOT::WriteSampling -> Writing to file..." << std::endl;
  hSample->SetTitle("NRF importance sampling distribution");
  gSample->SetTitle("NRF importance sampling distribution");
  hSample->GetXaxis()->SetTitle("energy #it{E} [MeV]");
  string titleProb = "probability per " + std::to_string(bin_width*1e6) + " eV";
  hSample->GetYaxis()->SetTitle(titleProb.c_str());
  gSample->GetXaxis()->SetTitle("Energy #it{E} [MeV]");
  gSample->GetYaxis()->SetTitle(titleProb.c_str());

  // save everything to file
  TFile *fout = new TFile("brems_distributions.root","recreate");
  fout->cd();
  gBrems->Write();
  hBrems->Write();
  gSample->Write();
  hSample->Write();
  std::cout << "MantisROOT::WriteSampling -> File Complete. Saved to brems_distributions.root" << std::endl;
  fout->Close();
}

void MantisROOT::PrepInputSpectrum(const char* bremInputFilename, double deltaE=5.0e-6, string outfile="brem.root")
{
  CheckFile(bremInputFilename);
  TFile *f = new TFile(bremInputFilename);
  f->cd();
  TTree* tBrem;
  f->GetObject("Brem", tBrem);
  double maxE = tBrem->GetMaximum("Energy");
  double minE = tBrem->GetMinimum("Energy");

  int nbins = (maxE - minE)/deltaE;

  TH1D* hBrems = new TH1D("hBrems","Bremsstrahlung Input Spectrum", nbins, minE, maxE);
  tBrem->Draw("Energy>>hBrems","","goff");
  hBrems->Scale(1./hBrems->Integral());
  hBrems->GetXaxis()->SetTitle("Energy [MeV]");
  int titleEValue = deltaE*1e6;
  string yTitle = "Probability per " + std::to_string(titleEValue) + " eV";
  hBrems->GetYaxis()->SetTitle(yTitle.c_str());
  TFile *fout = new TFile(outfile.c_str(),"RECREATE");
  fout->cd();
  hBrems->Write();
  std::cout << "MantisROOT::PrepInputSpectrum -> hBrems written to " << outfile << std::endl;
  fout->Close();
  f->cd();
  f->Close();
}

void MantisROOT::SimpleSampling(const char* bremInputFilename, double deltaE=5.0e-6, double cut_energy=1.5, double weight=10000, bool checkZero=false)
{
  CheckFile(bremInputFilename);

  TH1D* hBrems = BuildBrem(bremInputFilename, deltaE, checkZero);
  hBrems->Print();
	TGraph *gBrems = new TGraph(hBrems);
  TH1D* hSample = BuildSimpleSample(bremInputFilename, deltaE, cut_energy, weight);
  TGraph *gSample = new TGraph(hSample);

  WriteSampling(hBrems,hSample,gBrems,gSample,deltaE);

  std::cout << "MantisROOT::SimpleSampling -> Complete." << std::endl;

}

void MantisROOT::Sampling(const char *bremInputFilename, string sample_element="U",
							double deltaE=5.0e-6, bool checkZero=false, double non_nrf_energy_cut=1.5)
{
	// Convert Input Bremsstrahlung Spectrum Histogram to TGraph
	CheckFile(bremInputFilename);

  TH1D* hBrems = BuildBrem(bremInputFilename, deltaE, checkZero);
  TGraph* gBrems = new TGraph(hBrems);
  double Emax = ReturnBremMax(bremInputFilename);
  int nbins = Emax/deltaE;
	// resonance energies in MeV as calculated by G4NRF
	vector<double> Evec;
	vector<double> Evec_above_threshold;

	if(sample_element == "U")
	{
		std::cout << "Sampling for Uranium!" << std::endl;
		// U-238
		Evec.push_back(1.78200716196); // Vavrek
		Evec.push_back(1.84600768563); // Vavrek
		Evec.push_back(2.17601067909); // Vavrek
		Evec.push_back(2.20901100546); // Vavrek
		Evec.push_back(2.24501136709); // Vavrek

		// U-235
		Evec.push_back(1.65623622215); // Me
		Evec.push_back(1.73354686425); // Vavrek
		Evec.push_back(1.81525753275); // Vavrek
		Evec.push_back(1.86231786689); // Me
		//Evec.push_back(2.00336916735); // Vavrek
		Evec.push_back(2.00619912943); // Me
	}
	else if(sample_element == "Pu")
	{
		std::cout << "Sampling for Plutonium!" << std::endl;
		// Pu-239
		Evec.push_back(2.13501023737);
		Evec.push_back(2.14357031961);
		Evec.push_back(2.15101039142);
		Evec.push_back(2.43171328004);

		// Pu-240
		Evec.push_back(2.43321324158);
		Evec.push_back(2.57751485869);
		Evec.push_back(2.56641473101);
	}
	else
	{
		std::cout << "Sample element not found. Exiting..." << std::endl;
		exit(100);
	}

	for(int i=0;i<Evec.size();i++)
	{
	  if(Evec[i] <= Emax)
	  {
	    Evec_above_threshold.push_back(Evec[i]);
	  }
	}

	TH1D *hSample = new TH1D("hSample", "hSample", nbins, 0., Emax);

	// create the sampling distribution
	// user can adjust relative scales in SetBinContent
	for (int i = 1; i <= nbins; ++i) {
		double e = hSample->GetBinCenter(i);

		for (int j = 0; j < Evec_above_threshold.size(); ++j)
		{
			if (e < non_nrf_energy_cut)
			{
				hSample->SetBinContent(i, 0.0001);
			}
  		else if (e > Evec_above_threshold[j] - deltaE && e < Evec_above_threshold[j] + deltaE)
  		{
				hSample->SetBinContent(i, 1);
				break;
  		}
  		else
  		{
				hSample->SetBinContent(i, 0.1);
			}
		}
	}

	// normalize hSample so that its integral is 1
	hSample->Scale(1.0/(hSample->Integral()));
	TGraph *gSample = new TGraph(hSample);
	WriteSampling(hBrems,hSample,gBrems,gSample,deltaE);

}// End of sampling

void MantisROOT::CheckIntObj(const char* onFile, const char* offFile, double Er=1.73354, bool Weighted=false)
{
  CheckFile(onFile);
  CheckFile(offFile);

  // On Analysis
  TFile *f = new TFile(onFile);
  if(f ==0)
  {
    std::cout << "MantisROOT::CheckIntObj -> File " << onFile << " NULL!" << std::endl;
    exit(1);
  }
  bool confirmf = f->cd();
  if(!confirmf)
  {
    std::cout << "MantisROOT::CheckIntObj -> Cound Not Change into " << onFile << " directory." << std::endl;
    exit(1);
  }
  TTree* inTree;
  f->GetObject("IntObjIn", inTree);
  double teMax = inTree->GetMaximum("Energy");
  std::cout << "MantisROOT::CheckIntObj -> " << onFile << " IntObjIn grabbed." << std::endl;
  double emin = Er - 100e-6;
  double emax = Er + 100e-6;
  TH1D *e1 = new TH1D("e1","Incident Interrogation Object 2eV Binning", 100, emin, emax);
  TH1D *e3 = new TH1D("e3","Incident Interrogation Object Full Spectrum", 500, 0., teMax);
  if(Weighted)
    inTree->Draw("Energy>>e1","Weight","goff");
  else
    inTree->Draw("Energy>>e1","","goff");

  e1->SetStats(0);
  e1->Sumw2();
  e1->Print();

  if(Weighted)
    inTree->Draw("Energy>>e3","Weight","goff");
  else
    inTree->Draw("Energy>>e3","","goff");

  e3->SetStats(0);
  e3->Sumw2();
  e3->Print();

  // Off Analysis
  TFile *f2 = new TFile(offFile);
  if(f2 ==0)
  {
    std::cout << "MantisROOT::CheckIntObj -> ERROR File " << offFile << " NULL!" << std::endl;
    exit(1);
  }
  bool confirmf2 = f2->cd();
  if(!confirmf2)
  {
    std::cout << "MantisROOT::CheckIntObj -> ERROR Could NOT Change into " << offFile << " directory." << std::endl;
    exit(1);
  }

  TTree *inTree2;
  f2->GetObject("IntObjIn", inTree2);
  std::cout << "MantisROOT::CheckIntObj -> " << offFile << " IntObjIn grabbed." << std::endl;

  TH1D *e2 = new TH1D("e2","Incident Interrogation Object 2eV Binning", 100, emin, emax);
  TH1D *e4 = new TH1D("e4", "Incident Interrogation Object Full Spectrum", 500, 0., teMax);

  if(Weighted)
    inTree2->Draw("Energy>>e2","Weight","goff");
  else
    inTree2->Draw("Energy>>e2","","goff");

  e2->SetStats(0);
  e2->Sumw2();
  e2->SetLineColor(kRed);
  e2->Print();

  if(Weighted)
    inTree2->Draw("Energy>>e4","Weight","goff");
  else
    inTree2->Draw("Energy>>e4","","goff");

  e4->SetStats(0);
  e4->Sumw2();
  e4->SetLineColor(kRed);
  e4->Print();

  TCanvas *c1 = new TCanvas("c1","Incident Interrogation Object 2eV Binning",600,400);
  c1->cd();
  e2->Draw("h");
  f->cd();

  e1->Draw("h,SAME");

  e2->GetXaxis()->SetTitle("Incident Energy [MeV]");
  e2->GetYaxis()->SetTitle("Counts/2eV");

  auto legend = new TLegend();
  legend->SetHeader("Chopper State", "C");
  legend->AddEntry(e1, "Chopper On");
  legend->AddEntry(e2, "Chopper Off");
  legend->Draw();

  TCanvas *c2 = new TCanvas("c2","Incident Interrogation Object Full Spetrum",600,400);
  c2->cd();
  e3->Draw("h");
  f2->cd();

  e4->Draw("h,SAME");

  e3->GetXaxis()->SetTitle("Incident Energy [MeV]");
  e3->GetYaxis()->SetTitle("Counts");

  auto legend2 = new TLegend();
  legend2->SetHeader("Chopper State","C");
  legend2->AddEntry(e3, "Chopper On");
  legend2->AddEntry(e4, "Chopper Off");
  legend2->Draw();

  ZScore(e3->Integral(), e4->Integral());
  std::cout << "MantisROOT::CheckIntObj -> Complete." << std::endl;

} // end of CheckIntObj

void MantisROOT::CheckAngles(const char* filename, int estimate=-1)
{
  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Checking Angles..." << std::endl;

  CheckFile(filename);

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Opening File." << std::endl;

  TFile *f = new TFile(filename);
  f->cd();

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> File opened." << std::endl;

  TTree *t1, *t2;
  f->GetObject("IntObjOut",t1);
  f->GetObject("DetInfo",t2);
  t1->SetEstimate(estimate);
  t2->SetEstimate(estimate);
  std::cout << "MantisROOT::CheckAngles -> Grabbing IntObjOut Angles and Events." << std::endl;
  int entries = t1->Draw("Theta:Phi:EventID","","goff");

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> IntObjOut Entries: " << entries << std::endl;

  Double_t* thetas = t1->GetVal(0);
  Double_t* phis = t1->GetVal(1);
  Double_t* eventID = t1->GetVal(2);

  std::vector<double> thetasv;
  std::vector<double> phisv;
  std::vector<int> eventsv;

  if(estimate == -1)
  {
    for(int i=0;i<entries;++i)
    {
      thetasv.push_back(thetas[i]);
      phisv.push_back(phis[i]);
      eventsv.push_back((int)eventID[i]);
    }
  }
  else
  {
    for(int i=0;i<estimate;++i)
    {
      thetasv.push_back(thetas[i]);
      phisv.push_back(phis[i]);
      eventsv.push_back((int)eventID[i]);
    }
  }

  std::cout << "MantisROOT::CheckAngles -> IntObjOut Angles and Events Grabbed." << std::endl;
  std::cout << "MantisROOT::CheckAngles -> Grabbing Detinfo Events..." << std::endl;

  int entries2 = t2->Draw("EventID","","goff");

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> DetInfo Entries: " << entries2 << std::endl;

  Double_t* eventID2 = t2->GetVal(0);
  std::vector<int> events2v;

  if(estimate == -1)
  {
    for(int i=0;i<entries2;++i)
      events2v.push_back((int)eventID2[i]);
  }
  else
  {
    for(int i=0;i<estimate;++i)
      events2v.push_back((int)eventID2[i]);
  }

  std::cout << "MantisROOT::CheckAngles -> DetInfo Events Grabbed." << std::endl;

  std::cout << "MantisROOT::CheckAngles -> Removing Duplicates and Sorting DetInfo..." << std::endl;
  std::sort(events2v.begin(),events2v.end());
  events2v.erase(unique(events2v.begin(),events2v.end()),events2v.end());
  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Duplicates Removed. New Vector Size: " << events2v.size() << std::endl;

  // Compare Event IDs
  std::vector<double> det_thetasv;
  std::vector<double> det_phisv;
  std::vector<int> det_eventsv;
  int x;

  if(estimate == -1)
  {
    for(int i=0;i<eventsv.size();++i)
    {
      if(i % 100 == 0)
        std::cout << "\r** Checking Entry: " << i << std::flush;
      // Grab IntObjOut EventID
      x = eventsv[i];
      // Check if DetInfo EventID matches IntObjOut EventID
      auto exists = std::find(events2v.begin(),events2v.end(), x);

      if(exists != events2v.end())
      {
        det_eventsv.push_back(x);
        det_thetasv.push_back(thetasv[i]);
        det_phisv.push_back(phisv[i]);
      }

    }
  }
  else
  {
    for(int i=0;i<estimate;++i)
    {
      if(i % 100 == 0)
        std::cout << "\r** Checking Entry: " << i << std::flush;
      // Grab IntObjOut EventID
      x = eventsv[i];
      // Check if DetInfo EventID matches IntObjOut EventID
      auto exists = std::find(events2v.begin(),events2v.end(), x);

      if(exists != events2v.end())
      {
        det_eventsv.push_back(x);
        det_thetasv.push_back(thetasv[i]);
        det_phisv.push_back(phisv[i]);
      }

    }
  }

  std::cout << std::endl << "MantisROOT::CheckAngles -> Search complete." << std::endl;
  string filename_string = string(filename);
  string filenamebase = EraseSubStr(filename_string, ".root");
  string outfile = "Check_Angles_" + filenamebase + ".root";
  TFile *fout = new TFile(outfile.c_str(),"RECREATE");
  fout->cd();
  TTree *tAngle = new TTree("tAngle","IntObjOut Emission Angles Leading to Detection");
  int theEvent;
  double theTheta, thePhi;

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Assigning Branches..." << std::endl;

  tAngle->Branch("EventID",&theEvent);
  tAngle->Branch("Theta", &theTheta);
  tAngle->Branch("Phi", &thePhi);

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Filling TTree..." << std::endl;

  for(int i=0;i<det_eventsv.size();++i)
  {
    theEvent = det_eventsv[i];
    theTheta = det_thetasv[i];
    thePhi = det_phisv[i];
    tAngle->Fill();
  }

  if(debug)
    std::cout << "MantisROOT::CheckAngles -> Writing..." << std::endl;

  tAngle->Write();
  std::cout << "MantisROOT::CheckAngles -> Events and Angles written to Check_Angles.root" << std::endl;
  fout->Close();
  f->cd();
  f->Close();


} // end of CheckAngles

TGraph* MantisROOT::CreateTKDE(const char* filename, int nentries=10000)
{
  CheckFile(filename);
  TFile *f = new TFile(filename);
  f->cd();
  TTree* tBrem;

  f->GetObject("Brem",tBrem);
  tBrem->SetEstimate(nentries);

  double xmin = 0.;
  double xmax = tBrem->GetMaximum("Energy");
  double rho = 1.0;
  int nbins = xmax/5e-6;

  TH1D* h1 = new TH1D("h1","h1",nbins,xmin, xmax);

  tBrem->Draw("Energy","","goff");

  std::vector<double> energyv(nentries);
  Double_t* energies = tBrem->GetVal(0);
  std::cout << "MantisROOT::CreateTKDE -> Energies Grabbed." << std::endl;

  for(int i=0;i<nentries;++i)
  {
    energyv[i] = energies[i];
    h1->Fill(energyv[i]);
  }

  // Scale histogram
  h1->Scale(1./h1->Integral(),"width");
  h1->SetStats(0);
  h1->SetTitle("Bremsstrahlung Data");
  h1->Draw();

  std::cout << "MantisROOT::CreateTKDE -> Creating TKDE..." << std::endl;
  TKDE* kde = new TKDE(nentries, &energyv[0], xmin, xmax, "", rho);

  // Normalize kde
  //kde->ComputeKernelL2Norm();
  std::cout << "MantisROOT::CreateTKDE -> TKDE Created." << std::endl;

  kde->Draw("SAME");
  std::cout << "MantisROOT::CreateTKDE -> TKDE Drawn." << std::endl;

  TLegend* legend = new TLegend();
  legend->SetHeader("Bremsstrahlung Data","C");
  legend->AddEntry(h1,"Bremsstrahlung Histogram");
  legend->AddEntry(kde, "Bremsstrahlung kde");
  legend->Draw();
  std::cout << "MantisROOT::CreateTKDE -> Complete." << std::endl;

  TCanvas* c2 = new TCanvas();
  c2->cd();
  kde->Draw();

  TCanvas* c3 = new TCanvas();
  c3->cd();
  TF1* const hk = kde->GetFunction(nentries);
  TGraph* gBrems = new TGraph(hk);
  gBrems->Draw();

  return gBrems;

} // End of CreateTKDE Function

void MantisROOT::CheckDet(const char* filename, bool weighted=false, int estimate=-1)
{
  CheckFile(filename);
  TFile *f = new TFile(filename);
  f->cd();
  TTree *tDet, *tIntObjOut;
  f->GetObject("IntObjOut",tIntObjOut);
  f->GetObject("DetInfo",tDet);
  tIntObjOut->SetEstimate(estimate);
  tDet->SetEstimate(estimate);

  int intObj_entries = tIntObjOut->Draw("EventID","","goff");
  Double_t* intObj_ids = tIntObjOut->GetVal(0);

  int det_entries = 0;

  if(weighted)
    det_entries = tDet->Draw("EventID:Energy:Time:Weight","","goff");
  else
    det_entries = tDet->Draw("EventID:Energy:Time","","goff");

  Double_t* det_ids = tDet->GetVal(0);
  Double_t* det_energies = tDet->GetVal(1);
  Double_t* det_times = tDet->GetVal(2);

  int n_intObj = 0;
  int n_det = 0;
  if(estimate == -1)
  {
    n_intObj = intObj_entries;
    n_det = det_entries;
  }
  else
  {
    n_intObj = estimate;
    n_det = estimate;
  }

  Double_t *det_weights = new Double_t[n_det];

  if(weighted)
    det_weights = tDet->GetVal(3);

  if(debug)
    std::cout << "MantisROOT::CheckDet -> All Objects Grabbed!" << std::endl;

  std::vector<int> intObj_eventsv, det_eventsv;
  std::vector<Double_t> det_energiesv, det_weightsv, det_timesv;
  std::vector<string> det_creationsv;

  for(int i=0;i<n_intObj;++i)
    intObj_eventsv.push_back((int)intObj_ids[i]);

  Char_t creations[16];
  tDet->SetBranchAddress("CreatorProcess", &creations);

  for(int i=0;i<n_det;++i)
  {
    det_eventsv.push_back((int)det_ids[i]);
    det_energiesv.push_back(det_energies[i]);
    tDet->GetEntry(i);
    string creator_string = string(creations);
    det_creationsv.push_back(creator_string);
    det_timesv.push_back(det_times[i]);
    if(weighted)
      det_weightsv.push_back(det_weights[i]);
  }

  std::sort(intObj_eventsv.begin(), intObj_eventsv.end());
  intObj_eventsv.erase(unique(intObj_eventsv.begin(),intObj_eventsv.end()),intObj_eventsv.end());
  std::cout << "MantisROOT::CheckDet -> IntObj Event Vector Size: " << intObj_eventsv.size() << std::endl;
  std::cout << "MantisROOT::CheckDet -> DetInfo Event Vector Size: " << det_eventsv.size() << std::endl;

  // Zipping Method sorting
  std::vector<std::tuple<int, double, string, double>> zipped4; // no weights
  std::vector<std::tuple<int, double, string, double, double>> zipped5; // weights

  if(weighted)
  {
    zip(det_eventsv, det_energiesv, det_creationsv, det_timesv, det_weightsv, zipped5);
    std::sort(zipped5.begin(),zipped5.end());
    unzip(zipped5, det_eventsv, det_energiesv, det_creationsv, det_timesv, det_weightsv);
  }
  else
  {
    zip(det_eventsv, det_energiesv, det_creationsv, det_timesv, zipped4);
    std::sort(zipped4.begin(),zipped4.end());
    unzip(zipped4, det_eventsv, det_energiesv, det_creationsv, det_timesv);
  }

  std::vector<int> final_det_eventsv;
  std::vector<double> final_det_energiesv, final_det_weightsv, final_det_timesv;
  std::vector<string> final_det_creationsv;

  std::vector<int> errorneous_det_eventsv;
  std::vector<double> errorneous_det_weightsv;
  int x=0;
  int index = 0;
  int tmp_index;
  // Now Complete check
  while(index < det_eventsv.size())
  {
    if(!debug)
      std::cout << "\r** Checking Entry: " << index << std::flush;
    else
      std::cout << "Checking Entry: " << index << std::endl;

    // Grab Det EventID
    x = det_eventsv[index];

    if(debug)
      std::cout << "X: " << x << std::endl;

    // Check if DetInfo EventID matches IntObjOut EventID
    auto exists = std::find(intObj_eventsv.begin(),intObj_eventsv.end(), x);

    // if found write event info to new vectors
    if(exists != intObj_eventsv.end())
    {
      final_det_eventsv.push_back(x);
      final_det_energiesv.push_back(det_energiesv[index]);
      final_det_creationsv.push_back(det_creationsv[index]);
      final_det_timesv.push_back(det_timesv[index]);

      if(weighted)
        final_det_weightsv.push_back(det_weightsv[index]);

      int counter = 0;
      while(counter >= 0)
      {
        counter++;
        tmp_index = index + counter;
        if(x == det_eventsv[index+counter])
        {
          final_det_eventsv.push_back(x);
          final_det_energiesv.push_back(det_energiesv[index+counter]);
          final_det_creationsv.push_back(det_creationsv[index+counter]);
          final_det_timesv.push_back(det_timesv[index+counter]);

          if(weighted)
            final_det_weightsv.push_back(det_weightsv[index+counter]);

        } // end of if(x == det_eventsv[index+counter])
        else
        {
          index = tmp_index;
          counter = -1;
          if(debug)
          {
            //std::cout << "While Else Counter: " << counter << std::endl;
            std::cout << "While Else Index: " << index << std::endl;
          }
        } // end of else
      } // end of while counter > 0
    } // end of if exists !=
    else
    {
      errorneous_det_eventsv.push_back(x);

      if(weighted)
        errorneous_det_weightsv.push_back(det_weightsv[index]);

      index++;

      if(debug)
        std::cout << "Else Index: " << index << std::endl;
    } // end of else

  } // end of while index < det_eventsv.size()

  // Write vectors to new TTree
  string outfile = "Corrected_DetInfo_" + string(filename);

  if(debug)
  {
    std::cout << "MantisROOT::CheckDet -> Writing Corrected DetInfo to file: "
              << outfile << std::endl;
  }

  TFile *fout = new TFile(outfile.c_str(),"RECREATE");
  fout->cd();

  int a;
  double b,d,e;
  string c;
  TTree* Corrected_DetInfo = new TTree("Corrected_DetInfo","Corrected Detector Information");
  Corrected_DetInfo->Branch("EventID",&a);
  Corrected_DetInfo->Branch("Energy",&b);
  Corrected_DetInfo->Branch("CreatorProcess",&c);
  Corrected_DetInfo->Branch("Time",&d);

  if(weighted)
    Corrected_DetInfo->Branch("Weight",&e);

  if(debug)
    std::cout << "MantisROOT::CheckDet -> Corrected DetInfo Filling..." << std::endl;

  for(int i=0;i<final_det_eventsv.size();++i)
  {
    a = final_det_eventsv[i];
    b = final_det_energiesv[i];
    c = final_det_creationsv[i];
    d = final_det_timesv[i];

    if(weighted)
      e = final_det_weightsv[i];

    Corrected_DetInfo->Fill();
  }

  int h;
  double g;
  TTree* Errorneous_DetInfo = new TTree("Erroneous_DetInfo","Erroneous Detector Information");
  Errorneous_DetInfo->Branch("EventID",&h);

  if(weighted)
    Errorneous_DetInfo->Branch("Weight",&g);

  for(int i=0;i<errorneous_det_eventsv.size();++i)
  {
    h = errorneous_det_eventsv[i];

    if(weighted)
      g = errorneous_det_weightsv[i];

    Errorneous_DetInfo->Fill();
  }

  if(debug)
    std::cout << std::endl << "MantisROOT::CheckDet -> Corrected DetInfo Filled." << std::endl;

  Corrected_DetInfo->Write();
  Errorneous_DetInfo->Write();
  std::cout << std::endl << "MantisROOT::CheckDet -> Corrected DetInfo Written to file: " << outfile << std::endl;

  fout->Close();

  std::cout <<"MantisROOT::CheckDet -> Complete." << std::endl;

} // end of CheckDet Function

void MantisROOT::CreateScintillationDistribution(std::vector<double> x, std::vector<double> y)
{
  std::vector<double> energies, crossX, wavelengths;
  int n = x.size();
  for(int i=0;i<n;++i)
  {
    energies.push_back(x[i]);
    crossX.push_back(y[i]);
    wavelengths.push_back(Energy2Wave(x[i],"eV")*1e9); // units nm
  }

  TGraph* gScint = new TGraph(n, &energies[0], &crossX[0]);
  gScint->GetXaxis()->SetTitle("Energy [eV]");
  gScint->GetYaxis()->SetTitle("Cross Section");
  gScint->SetTitle("Scintillation in Water Emission Distribution");

  TCanvas *c1 = new TCanvas();
  c1->cd();

  double xmin = energies[0];
  double xmax = energies[n-1];
  double ypos = c1->GetFrame()->GetY2();

  TF1* f1 = new TF1("f1","(6.62607004e-34*299792458/(x/1e9))/1.60218e-19",wavelengths[0],wavelengths[n-1]);
  TGaxis* axis1 = new TGaxis(xmin, ypos + 0.03, xmax, ypos + 0.03, "f1", 510, "-L");

  axis1->SetName("axis1");
  axis1->SetTitle("Wavelength [nm]");
  axis1->SetTitleOffset(-1.);
  //axis1->CenterTitle(kTRUE);
  axis1->SetLabelFont(42);
  axis1->SetLabelSize(0.03);
  axis1->SetTitleSize(0.03);
  //axis1->SetLabelOffset(0.03);
  axis1->SetTitleColor(kRed);
  axis1->SetLineColor(kRed);
  axis1->SetLabelColor(kRed);
  axis1->Print();

  gScint->Draw("AC");
  axis1->Draw("same");
  std::cout << "MantisROOT::CreateScintillationDistribution -> Scintillation Distribution Drawn." << std::endl;
}

void MantisROOT::CreateScintillationDistribution(string a, string b, string c, string d)
{
  std::vector<double> energies = {1.90744, 1.9837, 2.0664, 2.156, 2.25425, 2.3615,
                                  2.4796, 2.6101, 2.75520, 2.9173, 3.0996, 3.306,
                                  3.44400, 3.542405, 3.64659, 3.8149, 4.132806, 4.95936};

  string cmd = a + "*exp(" + b + "*x)";
  string cmd2 = c + "*exp(" + d + "*x)";
  string cmd3 = "(x<3.44400)*" + cmd + " + " + "(x>3.44400)*" + cmd2;

  const char* cmd_cstr = cmd.c_str();
  const char* cmd2_cstr = cmd2.c_str();
  const char* cmd3_cstr = cmd3.c_str();

  TF1* f0 = new TF1("f0","0.0004731*exp(2.215*x)",energies[0], energies[12]);
  TF1* f01 = new TF1("f01","4.687e8*exp(-5.812*x)",energies[12], energies[energies.size() - 1]);
  TF1* f02 = new TF1("f02","(x<3.44400)*0.0004731*exp(2.215*x) + (x>3.44400)*4.687e8*exp(-5.812*x)", energies[0], energies[energies.size() - 1]);

  TF1* f1 = new TF1("f1",cmd_cstr,energies[0],energies[12]);
  TF1* f2 = new TF1("f2",cmd2_cstr, energies[12], energies[energies.size()-1]);
  TF1* f3 = new TF1("f3",cmd3_cstr, energies[0], energies[energies.size() - 1]);

  TCanvas* c2 = new TCanvas("c2","Exponential Rise",600,400);
  TCanvas* c3 = new TCanvas("c3","Exponential Decay",600,400);
  TCanvas* c4 = new TCanvas("c4","Combined Distro",600,400);

  c2->cd();
  f0->SetLineColor(kBlue);
  f1->SetLineColor(kRed);
  f0->Draw();
  f1->Draw("SAME");

  c3->cd();
  f01->SetLineColor(kBlue);
  f2->SetLineColor(kRed);
  f01->Draw();
  f2->Draw("SAME");
  c4->cd();
  f02->SetLineColor(kBlue);
  f3->SetLineColor(kRed);
  f02->Draw();
  f3->Draw("SAME");

  for(int i=0;i<12;++i)
    std::cout << "MantisROOT::CreateScintillationDistribution -> Energy: " << energies[i] << " \t Eval: " << f1->Eval(energies[i]) << std::endl;

  for(int i=12;i<energies.size();++i)
    std::cout << "MantisROOT::CreateScintillationDistribution -> Energy: " << energies[i] << " \t Eval: " << f2->Eval(energies[i]) << std::endl;

  std::cout << std::endl << "MantisROOT::CreateScintillationDistribution -> Complete."
  << std::endl;
}

void MantisROOT::CreateScintillationDistribution()
{
  std::cout << "MantisROOT::CreateScintillationDistribution -> Using Default values..." << std::endl;
  std::vector<double> energies = {1.90744, 1.9837, 2.0664, 2.156, 2.25425, 2.3615,
                                  2.4796, 2.6101, 2.75520, 2.9173, 3.0996, 3.306,
                                  3.44400, 3.542405, 3.64659, 3.8149, 4.132806, 4.95936};

  std::vector<double> crossX = {0.134451, 0.148294, 0.164921, 0.185046, 0.209947, 0.240969,
                                0.280459, 0.331664, 0.399644, 0.492194, 0.622117, 0.811069,
                                0.941421, 0.23927, 0.0561113, 0.00538955, 6.45194e-05, 6.49917e-10};

  CreateScintillationDistribution(energies, crossX);
} // end of CreateScintillationDistribution Function

/*
void MantisROOT::CreateBremFit(const char* filename, double bin_width=5e-6)
{
  CheckFile(filename);
  TFile* f = new TFile(filename);
  f->cd();

  TTree* tBrems;
  f->GetObject("Brem",tBrems);

  if(debug)
    std::cout << "MantisROOT::CreateBremFit -> Brem Object Grabbed." << std::endl;

  double maxE = tBrems->GetMaximum("Energy");
  double maxWave = Energy2Wave(maxE,"MeV")*1e9; // units nm
  Int_t nbins = maxE/bin_width;
  TH1D* hBrems = new TH1D("hBrems","Brem Histo", nbins, 0., maxE);
  int n = tBrems->Draw("Energy>>hBrems","","goff");

  if(debug)
    std::cout << "MantisROOT::CreateBremFit -> hBrems Created. Smoothing..." << std::endl;

  hBrems->Smooth(2024);
  hBrems->Scale(1./hBrems->Integral());

  std::vector<double> energies, counts;

  if(debug)
    std::cout << "MantisROOT::CreateBremFit -> Writing to hBrems..." << std::endl;

  for(int i=0;i<nbins;++i)
  {
    energies.push_back(hBrems->GetXaxis()->GetBinCenter(i));
    counts.push_back(hBrems->GetBinContent(i));
  }

  TGraph* gBrems = new TGraph(nbins, &energies[0], &counts[0]);

  string cmd = "([0]/pow(x,2))*((x/" + std::to_string(maxE) + ") - 1)";

  if(debug)
    std::cout << "MantisROOT::CreateBremFit -> Fitting Equation: " << cmd << std::endl;

  const char* cmd_c_str = cmd.c_str();
  TF1* f1 = new TF1("f1",cmd_c_str, 0.001, maxE);

  gBrems->SetTitle("Bremsstrahlung Distribution with Fitting");
  gBrems->GetXaxis()->SetTitle("Energy [MeV]");
  string bin_width_string = std::to_string((int)bin_width*1e6);
  string yaxis_title = "Probability per " + bin_width_string + "eV";
  gBrems->GetYaxis()->SetTitle(yaxis_title.c_str());
  TCanvas* c1 = new TCanvas("c1","Brem TGraph",900,600);
  c1->cd();

  if(debug)
    std::cout << "MantisROOT::CreateBremFit -> Drawing Brem TGraph..." << std::endl;

  gBrems->Draw("AC");

  if(debug)
    std::cout << "MantisROOT::CreateBremFit -> Creating Best Fit..." << std::endl;

  gBrems->Fit("f1","R");

  std::cout << "MantisROOT::CreateBremFit -> Complete." << std::endl;


}

*/

void MantisROOT::CreateDetEfficiencyCurve(std::vector<double> x, std::vector<double> y, string DetType)
{
  std::vector<double> energies, wavelengths, eff;
  int n = x.size();
  for(int i=0;i<n;++i)
  {
    energies.push_back(x[i]);
    wavelengths.push_back(Energy2Wave(x[i],"eV")*1e9); // units nm
    eff.push_back(y[i]);
  }

  std::cout << "Energy Range: " << energies[0] << " - " << energies[n-1] << std::endl;

  TF1* f1 = new TF1("f1","(6.62607004e-34*299792458/(x/1e9))/1.60218e-19",wavelengths[0],wavelengths[n-1]);
  TCanvas* c2 = new TCanvas("c2","Input",900,700);
  c2->cd();
  f1->Draw();
  string title = DetType + " Quantum Efficiency";

  TGraph* gDete = new TGraph(n, &energies[0], &eff[0]);
  gDete->GetXaxis()->SetTitle("Energy [eV]");
  gDete->GetYaxis()->SetTitle("Efficiency [%]");
  gDete->SetTitle(title.c_str());

  TCanvas* c1 = new TCanvas("c1","Efficiencies",900,700);
  c1->cd();
  gDete->Draw("AC");
  c1->Update();
  double xmin = energies[0];
  double xmax = c1->GetFrame()->GetX2();
  double ypos = c1->GetFrame()->GetY2();
  TGaxis* axis1 = new TGaxis(xmin, ypos, xmax, ypos, "f1", 510, "-L");
  axis1->SetName("axis1");
  axis1->SetTitle("Wavelength [nm]");
  axis1->SetTitleOffset(-1.1);
  axis1->CenterTitle(kTRUE);
  axis1->SetLabelFont(42);
  axis1->SetLabelSize(0.03);
  axis1->SetTitleSize(0.03);
  //axis1->SetLabelOffset(0.03);
  axis1->SetTitleColor(kRed);
  axis1->SetLineColor(kRed);
  axis1->SetLabelColor(kRed);
  axis1->Draw("same");
  axis1->Print();

  std::cout << "MantisROOT::CreateDetEfficiencyCurve -> Efficiencies Drawn." << std::endl;
}

void MantisROOT::CreateDetEfficiencyCurve(string DetType)
{
  std::vector<double> energies = {1.7711, 1.9074, 2.0663, 2.2542, 2.4796,
                                  2.7551, 3.0995, 3.5423, 4.133, 4.428, 4.959, 6.199};

  std::vector<double> GaAsP_eff = {0.25,36.246,39.8,40.0,36.0,30.0,
                                  24.0, 15.0,4.8, 0.6, 0.25, 0.10};

  std::vector<double> Bi_eff = {0.015, 0.2, 3.0, 7.0, 18.5, 25.0,
                                28.0, 26.0, 6.0, 0.19, 0.1, 0.01};

  if(!DetType.compare("GaAsP"))
    CreateDetEfficiencyCurve(energies, GaAsP_eff, DetType);
  else if(!DetType.compare("Bialkali"))
    CreateDetEfficiencyCurve(energies, Bi_eff, DetType);
  else
  {
    std::cout << "MantisROOT::CreateDetEfficiencyCurve -> ERROR Det Type must either be GaAsP or Bialkali" << std::endl;
    return;
  }

  std::cout << "MantisROOT::CreateDetEfficiencyCurve -> Complete." << std::endl;
}

double MantisROOT::Energy2Wave(double energy, string unit="eV")
{
  if(!unit.compare("MeV"))
    energy = energy*1e6;
  else if(!unit.compare("keV"))
    energy = energy*1e3;
  else if(!unit.compare("eV"))
    energy = energy;
  else if(!unit.compare("J"))
    energy = energy/1.60218e-19;
  else
  {
    std::cout << "MantisROOT::Energy2Wave -> ERROR Unit not found." << std::endl;
    return -1;
  }

  energy = energy*1.60218e-19; // conversion to joule
  return hc/energy; // units m
}

double MantisROOT::Wave2Energy(double wavelength, string unit="m")
{
  if(!unit.compare("nm"))
    wavelength = wavelength/1e9;
  else if(!unit.compare("mm"))
    wavelength = wavelength/1e3;
  else if(!unit.compare("m"))
    wavelength = wavelength;
  else if(!unit.compare("km"))
    wavelength = wavelength*1e3;
  else
  {
    std::cout << "MantisROOT::Wave2Energy -> ERROR Unit not found." << std::endl;
    return -1;
  }
  double energy = hc/wavelength;
  return energy/1.60218e-19; // unit eV
}

void MantisROOT::PrepareAnalysis(std::vector<string> filebases, bool weighted=false, int estimate=-1)
{
  for(int i=0;i<filebases.size();++i)
  {
    string fileOn = filebases[i] + "On-merged.root";
    string fileOff = filebases[i] + "Off-merged.root";
    string fileOnc = "Corrected_DetInfo_" + fileOn;
    string fileOffc = "Corrected_DetInfo_" + fileOff;

    std::cout << "MantisROOT::PrepareAnalysis -> Checking Detected Events..." << std::endl;

    CheckDet(fileOn.c_str(),weighted, estimate);
    CheckDet(fileOff.c_str(),weighted, estimate);

    std::cout << "MantisROOT::PrepareAnalysis -> Copying Corrected DetInfo..." << std::endl;

    if(weighted)
    {
      CopyTrees(fileOnc.c_str(), {"Weight","Corrected_DetInfo","Erroneous_DetInfo"}, fileOn.c_str());
      CopyTrees(fileOffc.c_str(), {"Weight","Corrected_DetInfo","Erroneous_DetInfo"}, fileOff.c_str());
    }
    else
    {
      CopyTrees(fileOnc.c_str(), {"Corrected_DetInfo","Erroneous_DetInfo"}, fileOn.c_str());
      CopyTrees(fileOffc.c_str(), {"Corrected_DetInfo","Erroneous_DetInfo"}, fileOff.c_str());
    }

    std::cout << "MantisROOT::PrepareAnalysis -> Checking NRF Events Detected..." << std::endl;

    CheckEvents(fileOn.c_str(),weighted, true);
    CheckEvents(fileOff.c_str(),weighted, true);

    string fileOnce = "w_events_" + fileOn;
    string fileOffce = "w_events_" + fileOff;

    std::cout << "MantisROOT::PrepareAnalysis -> Copying NRF Events Detected..." << std::endl;

    if(weighted)
    {
      CopyTrees(fileOnce.c_str(), {"Weight","event_tree"}, fileOn.c_str());
      CopyTrees(fileOffce.c_str(), {"Weight","event_tree"}, fileOff.c_str());
    }
    else
    {
      CopyTrees(fileOnce.c_str(), {"event_tree"}, fileOn.c_str());
      CopyTrees(fileOffce.c_str(), {"event_tree"}, fileOff.c_str());
    }

    std::cout << "MantisROOT::PrepareAnalysis -> Conducting SNR Analysis..." << std::endl;

    Sig2Noise({fileOn.c_str(),fileOff.c_str()}, "Det", weighted, true, true, "Energy<5e-6");

    std::cout << "MantisROOT::PrepareAnalysis -> Conducting ZScore Analysis..." << std::endl;
    ZScore(fileOn.c_str(), fileOff.c_str(),{"DetInfo", "Corrected_DetInfo"}, weighted);

    std::cout << "MantisROOT::PrepareAnalysis -> " << filebases[i] << " Complete." << std::endl;

  } // end for loop

  std::cout << "MantisROOT::PrepareAnalysis -> Complete." << std::endl;
} // end PrepareAnalysis

//******************************************************************************//
//******************************************************************************//
//******************************************************************************//
// Begin Show Functions

void MantisROOT::Show_Help()
{
  std::cout << "void Help()" << std::endl;
}

void MantisROOT::Show_Help_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Help lists the available function calls and their descriptions."
  << std::endl << "To See just the Functions calls try Show()." << std::endl;
}

void MantisROOT::Show_Show()
{
  std::cout << "void Show(string function=All, bool description=false)" << std::endl;
}

void MantisROOT::Show_Show_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Show lists the available functions."
  << std::endl << "For a Specific Functions call pass the function name to Show()."
  << std::endl << "For the Description of the function in addition to the call inputs pass a bool true as the second input."
  << std::endl << "Example: mantis->Show(\"Show\",true)" << std::endl;
}

void MantisROOT::Show_OpenFile()
{
  std::cout << "TFile* OpenFile(const char* filename)" << std::endl;
}

void MantisROOT::Show_OpenFile_Description()
{
  std::cout <<"DESCRIPTION: " << std::endl << "Opens a TFile with the input filename. Returns TFile*" << std::endl;
}

void MantisROOT::Show_CombineFiles()
{
  std::cout << "void CombineFiles(std::vector<string> filenames, std::vector<string> ObjectsToCombine, const char* Outfilename)" << std::endl;
}

void MantisROOT::Show_CombineFiles_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Combines files through TChain Method."
  << std::endl << "Takes a vector of filenames with the TTree object names in the second input vector to merge into the third input (filename)."
  << std::endl << "Example: mantis->CombineFiles({\"file1.root\",\"file2.root\"},{\"IntObjIn\",\"DetInfo\"},\"Merged.root\")" << std::endl;
}

void MantisROOT::Show_CopyTrees()
{
  std::cout << "void CopyTrees(const char* filename, std::vector<string> TTreeNamesToCopy, const char* outfilename)" << std::endl;
}

void MantisROOT::Show_CopyTrees_Description()
{
  std::cout << "Clones TTrees from input 1 (filename) to a file named by input 3 outfilename."
  << std::endl << "The TTrees to be cloned are inputed as input two as a string vector."
  << std::endl << "Example: mantis->CopyTrees(\"test.root\",{\"IntObjIn\",\"DetInfo\"})" << std::endl;
}

void MantisROOT::Show_Sig2Noise()
{
  std::cout << "void Sig2Noise(std::vector<string> Filenames, string DataName, bool Weighted=false, bool Corrected=false, bool cut=false, TCut cut1=\"NA\")" << std::endl;
}

void MantisROOT::Show_Sig2Noise_Description()
{
  std::cout << "Computes the Signal to Noise Ratio in the files inputed in the string vector."
  << std::endl << "The signal to noise ratio can be computed for the Incident Interrogation Object spectrum"
  << std::endl << ", the detected spectrum, or both with the second input options: IncObj, Det, Both."
  << std::endl << "IF the TTrees contain weights be sure to set the third input bool option to true."
  << std::endl << "IF the TTrees are from a Corrected File be sure to set the fourth input bool option to true."
  << std::endl << "IF the TTrees are to have a cut placed the fifth input should be set to true and "
  << std::endl << " the sixth input should contain the TCut in parenthesis."
  << std::endl << "Example: mantis->Sig2Noise({\"TestOn.root\",\"TestOff.root\"},\"Both\", true, true, true, \"Energy<5e-6\")" << std::endl;
}

void MantisROOT::Show_ZScore()
{
  std::cout << "void ZScore(const char* filename1, const char* filename2, std::vector<string> ObjectNames, bool weighted=false)" << std::endl;
  std::cout << "void ZScore(double c1, double c2)" << std::endl;
}

void MantisROOT::Show_ZScore_Description()
{
  std::cout << "Computes weighted ZTest on input 1 filename and input 2 filename for the TTree object names in input 3 string vector."
  << std::endl << "Example: mantis->ZScore(\"TestOn.root\", \"TestOff.root\", {\"IntObjIn\",\"DetInfo\"}, true)" << std::endl;
}
void MantisROOT::Show_Integral()
{
  std::cout << "void Integral(TTree* Object)" << std::endl << "void Integral(std::vector<TTree*> Objects)"
  << std::endl << "void Integral(std::vector<TTree*> Object, TCut IntegralCut)" << std::endl;
}

void MantisROOT::Show_Integral_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Takes either a TTree object input, a vector of TTree objects, or a vector of TTree Objects with a TCut."
  << std::endl << "Computes the integral of the TTree with various methods."
  << std::endl << "If TCut is added the integral is determined after the TCut is applied."
  << std::endl << "Example: mantis->Integral(DetTree)" << std::endl;
}

void MantisROOT::Show_PredictThickness()
{
  std::cout << "void PredictThickness(std::vector<string> ObjectNames)" << std::endl
  << "void PredictThickness(std::vector<string> ObjectNames, double ResonanceEnergy)" << std::endl;
}

void MantisROOT::Show_PredictThickness_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Predicts the results of changing the thickness on the objects passed in the string vector."
  << std::endl << "If the second input is passed the thickness calculations will focus on the given resonance energy."
  << std::endl << "Example: mantis->PredictThickness({\"IntObjIn\",\"IntObjOut\"},1.73354)" << std::endl;
}

void MantisROOT::Show_RebinHisto()
{
  std::cout << "void RebinHisto(std::vector<string> inFile, std::vector<string> ObjName, std::vector<string> OutObjName, int nbins, double Emin, double Emax)"
  << std::endl << "void RebinHisto(std::vector<string> inFile, std::vector<string> ObjName,std::vector<string> OutObjName, int nbins, double Emin, double Emax, TCut cut1)"
  << std::endl;
}

void MantisROOT::Show_RebinHisto_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Rebins the TTree Objects from multiples files given histogram parameters."
  << std::endl << "Example: mantis->RebinHisto({\"file1.root\",\"file2.root\"},{\"IntObjIn\",\"IntObjOut\"},{\"Rebinned_IntObjIn\",\"Rebinned_IntObjOut\"},100,1.7334,1.7336)"
  << std::endl << "This would rebin the Incident and Emission Interrogation object spectra into TH1D objects Rebinned_IntObjIn and Rebinned_IntObjOut respectively."
  << std::endl << "The rebinned histograms would have 100 bins between 1.7334 and 1.7336 MeV." << std::endl;
}

void MantisROOT::Show_VarRebin()
{
  std::cout << "void VarRebin(vector<string>, vector<string>, vector<string>, int, double, double, TCut, double, double)" << std::endl;
}

void MantisROOT::Show_VarRebin_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "See RebinHisto. This Function allows variable binning." << std::endl;
}

void MantisROOT::Show_CheckEvents()
{
  std::cout << "void CheckEvents(const char* filename, bool Weighted, bool Corrected)" << std::endl;
}

void MantisROOT::Show_CheckEvents_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Checks TTrees NRF, Cherenkov and DetInfo for EventIDs that match."
  << std::endl << "If the events match the event data in the detector is collected and written to w_events_(filename)."
  << std::endl << "This function is called in Sig2Noise." << std::endl
  << "Example: mantis->CheckEvents(\"test.root\",true, true) would Check WEIGHTED CORRECTED events in file test.root." << std::endl;
}

void MantisROOT::Show_CheckDet()
{
  std::cout << "void CheckDet(const char* filename, bool weighted=false, int estimate=-1)" << std::endl;
}

void MantisROOT::Show_CheckDet_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Checks DetInfo TTree for matches in IntObjOut TTree event IDs."
  << std::endl << "If there is not a matching ID the DetInfo is removed for that event."
  << std::endl << "If DetInfo is a weighted spectrum include the second input bool true."
  << std::endl << "For a subset of the data include the third input." << std::endl;
}

void MantisROOT::Show_Sampling()
{
  std::cout << "void Sampling(const char* bremInputFilename, string sample_element=U, double deltaE=5.0e-6, bool checkZero=false, double non_nrf_energy_cut=1.5)" << std::endl;
}

void MantisROOT::Show_Sampling_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Creates an importance sampling distribution and prepares mantis input file brems_distributions.root."
  << std::endl << "If the user would like a different bin width for hBrems than the user can supply the bin width with input 3."
  << std::endl << "Example: mantis->Sampling(\"brem.root\",\"U\", 5e-6,true,1.5)"
  << std::endl << "would create brems_distributions.root with 5e-6 bin widths where if any bin content = 0 that bin would be set to the prior bins content"
  << std::endl << "the importance sampling distribution energies below 1.5 MeV would have importances 1/1000 of the NRF resonance energies."
  << std::endl;
}

void MantisROOT::Show_SimpleSampling()
{
  std::cout << "void SimpleSampling(const char* bremInputFilename, double deltaE=5.0e-6, double cut_energy=1.5, double weight=10000, bool checkZero=false)" << std::endl;
}

void MantisROOT::Show_SimpleSampling_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Creates an importance sampling distribution and prepares mantis input file brems_distributions.root."
  << std::endl << "If the User would like a different bin width for hBrems than the user can supply the bin width with input 2."
  << std::endl << "Example: mantis->SimpleSampling(\"brem.root\", 5e-6,1.5, 10000, true)"
  << std::endl << "would create brems_distributions.root with 5e-6 bin widths where if any bin content = 0 that bin would be set to the prior bins content"
  << std::endl << "the importance sampling distribution energies below 1.5 MeV would have importances 1/10000 of all energies above 1.5 MeV."
  << std::endl;
}

void MantisROOT::Show_CheckIntObj()
{
  std::cout << "void CheckIntObj(const char* onFile, const char* offFile, double Er=1.73354, bool Weighted=false)" << std::endl;
}

void MantisROOT::Show_CheckIntObj_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Draws a 200eV Wide Region centered on Er with 2eV bin widths for both Chopper States (On/Off)."
  << std::endl << "If the spectra contain weights be sure to add the boolean true to the fourth input." << std::endl;
}

void MantisROOT::Show_CheckAngles()
{
  std::cout << "void CheckAngles(const char* filename, int estimate)" << std::endl;
}

void MantisROOT::Show_CheckAngles_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Determines which Angles emitted from IntObjOut are Detected."
  << std::endl << "Estimate sets the limit on how many events to check. "
  << std::endl << "Usefull for determining emission angle cuts to place." << std::endl;
}

void MantisROOT::Show_CreateTKDE()
{
  std::cout << "void CreateTKDE(const char* filename, int nentries=10000)" << std::endl;
}

void MantisROOT::Show_CreateTKDE_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Creates TKDE for Bremsstrahlung Input with given number of entries." << std::endl;
}

void MantisROOT::Show_CreateScintillationDistribution()
{
  std::cout << "void CreateScintillationDistribution(vector<double> energies, vector<double> crossX)"
  << std::endl << "void CreateScintillationDistribution(string a, string b)"
  << std::endl << "void CreateScintillationDistribution()" << std::endl;
}

void MantisROOT::Show_CreateScintillationDistribution_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Creates a TGraph of the users scintillation distribution."
  << std::endl << "IF function called void of inputs the default values are plotted."
  << std::endl << "IF function called with string a, string b creates scintillation distribution based on exponential distribution."
  << std::endl;
}

void MantisROOT::Show_CreateDetEfficiencyCurve()
{
  std::cout << "void CreateDetEfficiencyCurve(vector<double>, energies vector<double> efficiencies, string PCName)"
  << std::endl << "void CreateDetEfficiencyCurve(string PCType)" << std::endl;
}

void MantisROOT::Show_CreateDetEfficiencyCurve_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Creates TGraph of the users Photocathode Quantum Efficiency."
  << std::endl << "The PC Type can be set as either GaAsP or Bialkali." << std::endl
  << "IF the user inputs their own energies/efficiencies the PCName is written to the title of the output canvas."
  << std::endl;
}

void MantisROOT::Show_Energy2Wave()
{
  std::cout << "double Energy2Wave(double energy, string unit=\"eV\")" << std::endl;
}

void MantisROOT::Show_Energy2Wave_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Returns wavelength unit meters from energy unit eV."
  << std::endl << "Unit options eV keV MeV J" << std::endl;
}

void MantisROOT::Show_Wave2Energy()
{
  std::cout << "double Wave2Energy(double wavelength, string unit=\"m\")"
  << std::endl;
}

void MantisROOT::Show_Wave2Energy_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Returns energy unit eV from wavelength unit meters."
  << std::endl << "Unit options nm mm m km" << std::endl;
}

void MantisROOT::Show_PrepareAnalysis()
{
  std::cout << "void PrepareAnalysis(vector<string> filebases, bool weighted=false)" << std::endl;
}

void MantisROOT::Show_PrepareAnalysis_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Prepares files from users filebases vector. If the files have weighted information set the second input to true"
  << std::endl << "Example: mantis->PrepareAnalysis({\"test9\",\"test10\"},true)" << std::endl
  << "Would run CheckDet, CheckEvent and CopyTrees on weighted spectra in files "
  << std::endl << "test9On-merged.root, test9Off-merged.root, test10On-merged.root and test10Off-merged.root"
  << std::endl;
}

void MantisROOT::Show_PrepInputSpectrum()
{
  std::cout << "void PrepInputSpectrum(const char* bremInputFilename, double deltaE=5.0e-6, string outfilename=\"brem.root\")" << std::endl;
}

void MantisROOT::Show_PrepInputSpectrum_Description()
{
  std::cout << "DESCRIPTION: " << std::endl << "Prepares input spectrum file for Mantis Simulation without importance sampling."
  << std::endl << "deltaE is the bin width of the Histogram and if the user wishes a nondefault outfilename enter the third input."
  << std::endl;
}

void MantisROOT::Show_GetInstance()
{
  std::cout << "Run MantisROOT* m = MantisROOT::GetInstance(bool debug=false) to get MantisROOT singleton." << std::endl;
}
//******************************************************************************//
//******************************************************************************//
//******************************************************************************//
// End Show Functions, Allow Help to be called outside of MantisROOT Class

string MantisROOT::EraseSubStr(string & mainStr, const string & toErase)
{
    // Search for the substring in string
    string newString = mainStr;
    size_t pos = newString.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        newString.erase(pos, toErase.length());
    }
    return newString;
}

void MantisROOT::CheckFile(const char* filename)
{
  if(gSystem->AccessPathName(filename))
  {
    std::cout << "File " << filename << " Not Found." << std::endl;
    exit(1);
  }
}

void Help()
{
  MantisROOT* m = MantisROOT::GetInstance();
  m->Help();
}


//
// end of file
