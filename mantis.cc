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

// declare global variables
G4long seed;
G4bool output;
// String global variables
G4String macro, root_output_name, gOutName, inFile;
// boolean global variables
G4bool bremTest, resonanceTest, debug, addNRF, printEvents, SampleEnergyRangebool;
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
  G4cerr << "mantis [-h help] [-m macro=mantis.in] [-a chosen_energy=-1.] [-s seed=1] [-o output_name] [-t bremTest=false] " <<
          "[-r resonance_test=false] [-p standalone=false] [-v NRF_Verbose=false] [-n addNRF=true] " <<
          "[-i inFile] [-d debug] [-f printEvents] [-u SampleEnergyRange] [-w uniform_width]"
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
  G4bool force_isotropic = false;
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
  G4String debug_in = "false";
  debug = false;
  // Primary Generator Defaults
  G4String resonance_in = "false";
  resonanceTest = false;
  chosen_energy = -1.;
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
      if      (G4String(argv[i]) == "-h") PrintUsage();
      else if (G4String(argv[i]) == "-m") macro = argv[i+1];
      else if (G4String(argv[i]) == "-a") chosen_energy = std::stod(argv[i+1]);
      else if (G4String(argv[i]) == "-s") seed = atoi(argv[i+1]);
      else if (G4String(argv[i]) == "-o") root_output_name = argv[i+1];
      else if (G4String(argv[i]) == "-t") bremTest_in = argv[i+1];
      else if (G4String(argv[i]) == "-r") resonance_in = argv[i+1];
      else if (G4String(argv[i]) == "-p") standalone_in = argv[i+1];
      else if (G4String(argv[i]) == "-v") verbose_in = argv[i+1];
      else if (G4String(argv[i]) == "-n") addNRF_in = argv[i+1];
      else if (G4String(argv[i]) == "-i") inFile = argv[i+1];
      else if (G4String(argv[i]) == "-d") debug_in = argv[i+1];
      else if (G4String(argv[i]) == "-f") printEvents_in = argv[i+1];
      else if (G4String(argv[i]) == "-u") SampleEnergyRange_in = argv[i+1];
      else if (G4String(argv[i]) == "-w") uniform_width = std::stod(argv[i+1]);
      else
      {
        PrintUsage();
        return 1;
      }
  }

        // Handle Output File
        std::cout << "Output Filename: " << root_output_name << std::endl;
        std::string RootOutputFile = (std::string)root_output_name;
        if(RootOutputFile.find(".root")<RootOutputFile.length()) {
                gOutName=(std::string)RootOutputFile.substr(0, RootOutputFile.find(".root"));
        }
        else gOutName=(std::string)root_output_name;

        // Handle Debugging
        if(debug_in == "True" || debug_in == "true")
        {
          std::cout << "Debugging mode set." << std::endl;
          debug = true;
        }

        G4UImanager* UI = G4UImanager::GetUIpointer();
        MySession* LoggedSession = new MySession;

        if(!ui && macro != "vis_save.mac")
        {
                output = true;
                UI->SetCoutDestination(LoggedSession);
        }

        // Physics List Options
        if(standalone_in == "True" || standalone_in == "true")
        {
                G4cout << "Standalone File Requested." << G4endl;
                standalone = true;
        }
        if(verbose_in == "True" || verbose_in == "true")
        {
                G4cout << "NRF Verbose set to: " << verbose_in << G4endl;
                NRF_Verbose = true;
        }
        if(addNRF_in == "False" || addNRF_in == "false")
        {
                //std::cout << "NRF Physics turned OFF!" << std::endl;
                G4cout << "NRF Physics turned OFF!" << G4endl;
                addNRF = false;
        }
        if(printEvents_in == "True" || printEvents_in == "true")
        {
          G4cout << "Printing Events to std::cout" << G4endl;
          printEvents = true;
        }

        // Primary Generator Options
        if(bremTest_in == "True" || bremTest_in == "true")
        {
                G4cout << "Conducting Brem Test!" << G4endl;
                bremTest = true;
        }

        if(resonance_in == "True" || resonance_in == "true")
        {
                G4cout << "Completing Resonance Test!" << G4endl;
                resonanceTest = true;
        }

        if(SampleEnergyRange_in == "True" || SampleEnergyRange_in == "true")
        {
          G4cout << "Sampling Uniform Centered on " << chosen_energy
          << " with normal width " << uniform_width << std::endl;
          SampleEnergyRangebool = true;
        }

        // Some User Error Checking
        if(bremTest && resonanceTest)
        {
                G4cerr << "FATAL ERROR mantis.cc -> Cannot test bremsstrahlung and resonance during the same run!" << G4endl;
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

        G4cout << "Seed set to: " << seed << G4endl;
        std::cout << "Seed set to: " << seed << std::endl;

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
        //std::cout << "visManager Initialized" << std::endl;
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
//std::cout << "Deleting visManager..." << std::endl;
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
