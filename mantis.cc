// Always include
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "DetectorConstruction.hh"
#include "PhysicsListNew.hh"
#include "ActionInitialization.hh"
#include "ChopperSetup.hh"
#include "Linac.hh"
#include "Collimator.hh"
#include "Cargo.hh"
// Typcially include
#include "time.h"
#include "Randomize.hh"
#include "G4Types.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#include "G4TrajectoryDrawByParticleID.hh"
G4VisManager* visManager;
#endif

#include "G4UIExecutive.hh"

// For G4cout and G4cerr handling
#include "MySession.hh"
#include "G4ios.hh"
#include "G4UIsession.hh"

// For FileSystem Handling
#include <sys/stat.h>

#include <algorithm>
#include <cctype>
#include <string>

// declare global variables
G4long seed;
G4bool output;
// String global variables
G4String macro, root_output_name, gOutName, inFile, response_function_file;
// boolean global variables
G4bool ResponseFunction, detTest, bremTest, resonanceTest, debug, addNRF, printEvents, SampleEnergyRangebool;
// double global variables
G4double uniform_width, chosen_energy;

void check_file_exists(const std::string &p)
{
  std::cout << "Checking Database Path: " << p << std::endl;
  struct stat buffer;

  if(stat (p.c_str(), &buffer) == 0)
    std::cout << p << " exists." << std::endl;
  else
  {
    std::cout << p << " does not exist." << std::endl;
    std::cout << "User MUST Set the correct Path for the NRF Database as an environment variable." <<
    std::endl << "See README For Details!" << std::endl;
    exit(1);
  }
}

namespace
{
  void PrintUsage()
  {
    G4cerr << "Usage: " << G4endl;
    G4cerr << "mantis [-h --help]                              Prints this Usage Screen" << G4endl
    << "      [--Macro=mantis.in]                              Macro File to be read for user input options -> Required!" << G4endl
    << "      [--File_To_Sample=brems_distributions.root]      Input File Containing hBrems bremsstrahlung input spectrum (ROOT Format TH1D*) to sample from." << G4endl
    << "      [--Seed=1]                                       Simulation Seed." << G4endl
    << "      [--Output_Filename=test.root]                    Data will be written to this file." << G4endl
    << "      [--Energy=-1.]                                   Sets the energy of the primary particle to the user's value in MeV" << G4endl
    << "      [--Detector_Response_File=DetectorResponse.root] Input File with Detector Response Function TProfile" << G4endl
    << "      [--Uniform_Width=0.005]                          Sets the uniform distribution width. Requires SampleEnergyRange Boolean to be passed as true." << G4endl
    << "      [-d --Debug=false]                               Runtime Boolean option for developers to place program in debugging mode printing statements at various spots in the program" << G4endl
    << "      [-i --Force_Isotropic=true]                      Forces nrf isotropic emission." << G4endl
    << "      [-n --NRF=true]                                  IF set to false NRF Physics will be removed from physicsList! The default is set to true." << G4endl
    << "      [-p --Print_Events=false]                        Runtime Boolean option to print event tracker to std::cout instead of G4cout to file" << G4endl
    << "      [-r --Detector_Response_Input=false]             Runs Mantis Simulation with Detector Response Function Input" << G4endl
    << "      [-s --Print_Standalone_Data=false]               Calls G4NRF to print a file of NRF Energies (takes up to 15 min)" << G4endl
    << "      [-v --Verbose=false]                             Sets NRF Physics Verbosity" << G4endl
    << G4endl << "Testing Options: Detector Response, Bremsstrahlung Test, Resonance Test" << G4endl << G4endl
    << "      [-t1 --Detector_Response_Test=false]             Create Detector Response Function" << G4endl
    << "      [-t2 --Brem_Test=false]                          For creating a bremsstrahlung beam for a secondary simulation input. Requires energy flag to be passed with max bremsstrahlung energy" << G4endl
    << "      [-t3 --Resonance_Test=false]                     Tests Resonance energies by having the input spectrum a normal distribution centered on Uranium resonance energies." << G4endl
    << "      [-t4 --Sample_Energy_Range=false]                Samples from a normal distribution centered on user's energy." << G4endl
    << G4endl;
    exit(1);
  }

}

int main(int argc,char **argv)
{
  // Check to make sure user set up environment correctly
  if(getenv("G4NRFGAMMADATA") == NULL)
  {
    std::cout << "mantis.cc -> FATAL ERROR: User must set environmental variable G4NRFGAMMADATA!" << std::endl;
    exit(1);
  }

  check_file_exists((std::string)getenv("G4NRFGAMMADATA"));

  // Defaults
  G4int start_time = time(0);
  // Physics List Defaults
  G4bool use_xsec_tables = true;
  G4bool use_xsec_integration = true;
  G4bool force_isotropic = true;
  G4String force_isotropic_in = "true";
  G4String standalone_in = "false";
  G4String verbose_in = "false";
  G4String addNRF_in = "true";
  G4String printEvents_in = "false";
  G4String SampleEnergyRange_in = "false";

  G4bool standalone = false;
  G4bool NRF_Verbose = false;
  addNRF = true;
  printEvents = false;
  // Run Defaults
  macro = "mantis.in";
  seed = 1;
  inFile = "brems_distributions.root";
  response_function_file = "DetectorResponse.root";
  G4String debug_in = "false";
  debug = false;
  // Primary Generator Defaults
  G4String resonance_in = "false";
  resonanceTest = false;
  chosen_energy = -1.;
  G4String ResponseFunction_in = "false";
  ResponseFunction = false;
  G4String detTest_in = "false";
  detTest=false;
  G4String bremTest_in = "false";
  bremTest = false;
  SampleEnergyRangebool = false;
  uniform_width = 0.005; // units MeV

  // Output Defaults
  output = false;

  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;

  if ( argc == 1 )
  {
    ui = new G4UIExecutive(argc, argv);
  }

  for (G4int i=1; i<argc; i=i+2)
  {
    //std::cout << i << std::endl;
      std::string input = argv[i];
      std::transform(input.begin(),input.end(),input.begin(),[](unsigned char c){return std::tolower(c);});

      if      (G4String(input) == "-h") PrintUsage();
      else if (G4String(input) == "--help") PrintUsage();
      else if (G4String(input) == "--macro")
      {
        macro = argv[i+1];
        std::cout << "Macro Input: " << macro << std::endl;
      }
      else if (G4String(input) == "--energy")
      {
        chosen_energy = std::stod(argv[i+1]);
        std::cout << "Chosen Energy: " << chosen_energy << std::endl;
      }
      else if (G4String(input) == "--seed")
      {
        seed = atoi(argv[i+1]);
        std::cout << "Seed: " << seed << std::endl;
      }
      else if (G4String(input) == "--output_filename")
      {
        root_output_name = argv[i+1];
        std::cout << "Output Filename: " << root_output_name << std::endl;
      }
      else if (G4String(input) == "--detector_response_input")
      {
        ResponseFunction_in = argv[i+1];
        std::cout << "Detector Response Run: " << ResponseFunction_in << std::endl;
      }
      else if (G4String(input) == "-r")
      {
        std::cout << "Detector Response Run: TRUE" << std::endl;
        ResponseFunction = true;
        i = i-1;
      }
      else if (G4String(input) == "--detector_response_file")
      {
        response_function_file = argv[i+1];
        std::cout << "Detector Response Input File: " << response_function_file << std::endl;
      }
      else if (G4String(input) == "--detector_response_test")
      {
        detTest_in = argv[i+1];
        std::cout << "Detector Response TEST: " << detTest_in << std::endl;
      }
      else if (G4String(input) == "-t1")
      {
        std::cout << "Detector Response TEST: TRUE" << std::endl;
        detTest = true;
        i = i-1;
      }
      else if (G4String(input) == "--brem_test")
      {
        bremTest_in = argv[i+1];
        if(bremTest_in == "True" || bremTest_in == "true")
        {
          std::cout << "Conducting Brem Test!" << std::endl;
          bremTest = true;
        }
      }
      else if (G4String(input) == "-t2")
      {
        std::cout << "Bremsstrahlung TEST: TRUE" << std::endl;
        bremTest = true;
        i = i-1;
      }
      else if (G4String(input) == "--resonance_test")
      {
        resonance_in = argv[i+1];
        if(resonance_in == "True" || resonance_in == "true")
        {
          G4cout << "Completing Resonance Test!" << G4endl;
          resonanceTest = true;
        }
      }
      else if (G4String(input) == "-t3")
      {
        std::cout << "Resonance TEST: TRUE" << std::endl;
        resonanceTest = true;
        i = i-1;
      }
      else if (G4String(input) == "--force_isotropic")
      {
        force_isotropic_in = argv[i+1];
        if(force_isotropic_in == "False" || force_isotropic_in == "false")
        {
          std::cout << "NRF Force Isotropic turned OFF!" << std::endl;
          force_isotropic = false;
        }
      }
      else if (G4String(input) == "-i")
      {
        std::cout << "NRF Force Isotropic Turned ON!" << std::endl;
        force_isotropic = true;
        i = i-1;
      }
      else if (G4String(input) == "--print_standalone_data")
      {
        standalone_in = argv[i+1];
        if(standalone_in == "True" || standalone_in == "true")
        {
          std::cout << "Standalone File Requested." << std::endl;
          standalone = true;
        }
      }
      else if (G4String(input) == "-s")
      {
        std::cout << "Standalone File Requested." << std::endl;
        standalone = true;
        i = i-1;
      }
      else if (G4String(input) == "--verbose")
      {
        verbose_in = argv[i+1];
        if(verbose_in == "True" || verbose_in == "true")
        {
          std::cout << "NRF Verbose set to: " << verbose_in << std::endl;
          NRF_Verbose = true;
        }
      }
      else if (G4String(input) == "-v")
      {
        std::cout << "NRF Verbose set to: TRUE" << std::endl;
        NRF_Verbose = true;
        i =i-1;
      }
      else if (G4String(input) == "--nrf")
      {
        addNRF_in = argv[i+1];
        if(addNRF_in == "False" || addNRF_in == "false")
        {
          std::cout << "NRF Physics turned OFF!" << std::endl;
          addNRF = false;
        }
      }
      else if (G4String(input) == "-n")
      {
        std::cout << "NRF Physics Turned ON!" << std::endl;
        addNRF = true;
        i=i-1;
      }
      else if (G4String(input) == "--file_to_sample")
      {
        inFile = argv[i+1];
        std::cout << "Sampling from file: " << inFile << std::endl;
      }
      else if (G4String(input) == "--debug")
      {
        debug_in = argv[i+1];
        if(debug_in == "True" || debug_in == "true")
        {
          std::cout << "Debugging mode set." << std::endl;
          debug = true;
        }
      }
      else if (G4String(input) == "-d")
      {
        std::cout << "Debugging mode set." << std::endl;
        debug = true;
        i=i-1;
      }
      else if (G4String(input) == "--print_events")
      {
        printEvents_in = argv[i+1];
        if(printEvents_in == "True" || printEvents_in == "true")
        {
          std::cout << "Printing Events to std::cout" << std::endl;
          printEvents = true;
        }
      }
      else if (G4String(input) == "-p")
      {
        std::cout << "Printing Events to std::cout" << std::endl;
        printEvents = true;
        i=i-1;
      }
      else if (G4String(input) == "--sample_energy_range")
      {
        SampleEnergyRange_in = argv[i+1];

        if(SampleEnergyRange_in == "True" || SampleEnergyRange_in == "true")
          SampleEnergyRangebool = true;

      }
      else if (G4String(input) == "-t4")
      {
        SampleEnergyRangebool = true;
        i=i-1;
      }
      else if (G4String(input) == "--uniform_width")
      {
        uniform_width = std::stod(argv[i+1]);
        std::cout << "Uniform Width Set to: " << uniform_width << std::endl;
      }
      else
      {
        std::cerr << "FATAL ERROR: " << std::endl << "User Inputs: " << std::endl;
        for(G4int i=1;i<argc;i++)
        {
          std::cerr << argv[i] << std::endl;
        }
        std::cerr << std::endl << "USAGE HELP: " << std::endl;
        PrintUsage();
        return 1;
      }
  }

  // Handle Output File
  std::string RootOutputFile = (std::string)root_output_name;
  if(RootOutputFile.find(".root")<RootOutputFile.length())
    gOutName=(std::string)RootOutputFile.substr(0, RootOutputFile.find(".root"));
  else gOutName=(std::string)root_output_name;

  if(SampleEnergyRange_in == "True" || SampleEnergyRange_in == "true")
  {
    G4cout << "Sampling Uniform Centered on " << chosen_energy
    << " with normal width " << uniform_width << std::endl;
  }

  G4UImanager* UI = G4UImanager::GetUIpointer();
  MySession* LoggedSession = new MySession;

  if(!ui && macro != "vis_save.mac")
  {
    output = true;
    UI->SetCoutDestination(LoggedSession);
  }

  if(chosen_energy > 0)
    inFile = "NO_INPUT_FILE";

  // Some User Error Checking
  if(bremTest && resonanceTest)
  {
    G4cerr << "FATAL ERROR mantis.cc -> Cannot test bremsstrahlung and resonance during the same run!" << G4endl;
    exit(1);
  }
  if(bremTest && detTest)
  {
    G4cerr << "FATAL ERROR mantis.cc -> Cannot test Bremsstrahlung and Detector Response during the same run!" << G4endl;
    exit(1);
  }
  if(detTest && resonanceTest)
  {
    G4cerr << "FATAL ERROR mantis.cc -> Cannot test Detector Response and Resonance Test!" << G4endl;
    exit(1);
  }
  if(bremTest && chosen_energy < 0)
  {
    G4cerr << "FATAL ERROR mantis.cc -> Cannot test bremsstrahlung without option -a input energy!" << G4endl;
    exit(1);
  }

  if(SampleEnergyRangebool && chosen_energy < 0)
  {
    G4cerr << "FATAL ERROR mantis.cc -> Cannot Sample Energy Range without choosing uniform center!" << G4endl;
    exit(1);
  }

  if(SampleEnergyRangebool && uniform_width < 0)
  {
    G4cerr << "FATAL ERROR mantis.cc -> Cannot Sample Energy Range with a width < 0." << G4endl;
    exit(1);
  }

  // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
  CLHEP::HepRandom::setTheSeed(seed);

  // construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // set mandatory initialization classes
  ChopperSetup* chopper = new ChopperSetup();
  Linac* linac = new Linac();
  Collimator* collimator = new Collimator();
  Cargo* cargo = new Cargo();
  runManager->SetUserInitialization(new DetectorConstruction(chopper, linac, collimator, cargo));
  runManager->SetUserInitialization(new PhysicsListNew(addNRF, use_xsec_tables, use_xsec_integration, force_isotropic, standalone, NRF_Verbose));
  runManager->SetUserInitialization(new ActionInitialization());

#ifdef G4VIS_USE
  if(ui || macro == "vis_save.mac")
  {
          visManager = new G4VisExecutive();
          visManager->Initialize();
  }
#endif

  if(!ui)
  {
    G4String command = "/control/execute ";
    UI->ApplyCommand(command+macro);
  }
  else
  {
    // interactive mode
    UI->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

#ifdef G4VIS_USE
  if(ui || macro == "vis_save.mac")
  {
    delete visManager;
  }
#endif

  G4int stop_time = time(0);
  G4cout << G4endl << "----------------------------------------------------------------------" << G4endl;
  G4cout << G4endl << "----------------------------------------------------------------------" << G4endl;
  G4cout << G4endl << "The MC took: [" << ((int)std::difftime(stop_time,start_time))/3600
          << " h " << ((int)std::difftime(stop_time,start_time)%3600)/60 << " m "
          << ((int)std::difftime(stop_time,start_time)%3600)%60 << " s]"
          << G4endl << G4endl;

  delete LoggedSession;
  delete runManager;

  return 0;
}
