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

#include "Analysis.hh"

extern G4String gOutName;
extern G4bool debug;
extern G4bool bremTest;
extern G4bool detTest;
extern G4bool WResponseFunction;
extern G4bool resonanceTest;
extern G4String inFile;
extern G4bool IntObjTest;

Analysis::Analysis() : fFactoryOn(false), WEIGHTED(false)
{
  start_time = time(0);
  if(!inFile.compare("importance_sampling_input.root"))
    WEIGHTED = true;

  if(resonanceTest)
    WEIGHTED = false;

  manager = G4AnalysisManager::Instance();
}

Analysis::~Analysis()
{
}

void Analysis::Book()
{
  int tuple_counter = 0;
  G4AnalysisManager* manager = G4AnalysisManager::Instance();
  manager->SetVerboseLevel(0);
  // open output file
  G4bool fileOpen = manager->OpenFile(gOutName);

  if(!fileOpen)
  {
    G4cerr << "Analysis::Book(): Cannot Open " <<manager->GetFileName()<<G4endl;
    return;
  }

  // For Bremsstrahlung Test
  if(bremTest)
  {
    NRF(); //             0
    BremRadiator(); //    1
    BremBacking(); //     2
    IncidentChopper(); // 3
    EmissionChopper(); // 4

    if(debug)
      std::cout << "Analysis::Book -> Brem Test Ntuples 0 - 4" << std::endl;
  }
  // for Creating Detector Response Function
  else if(detTest)
  {
    IncidentShielding();  // 0
    IncidentPlexiglass(); // 1
    DetectorResponse();   // 2
    if(debug)
      std::cout << "Analysis::Book -> Creating Detector Response Function Ntuples 0 - 2" << std::endl;
  }
  // for a Simulation with Detector Response
  else if(WResponseFunction)
  {
    if(debug)
      std::cout << "Analysis::Book -> Weighting set to: " << WEIGHTED << std::endl;

    NRF(WEIGHTED); // 0
    IncidentIntObj(WEIGHTED); // 1
    EmissionIntObj(WEIGHTED); // 2
    IncidentShielding(WEIGHTED); // 3
    IncidentPlexiglass(WEIGHTED); // 4
    DetectorEstimatedInfo(WEIGHTED); // 5

    if(debug)
      std::cout << "Analysis::Book -> With Detector Response Ntuples 0 - 5" << std::endl;
  }
  else if(IntObjTest)
  {
    if(debug)
      std::cout << "Analysis::Book Weighting set to: " << WEIGHTED << std::endl;

    NRF(WEIGHTED);             // 0
    IncidentChopper(WEIGHTED); // 1
    EmissionChopper(WEIGHTED); // 2
    IncidentIntObj(WEIGHTED);  // 3
    EmissionIntObj(WEIGHTED);  // 4

    if(debug)
      std::cout << "Analysis::Book -> IntObj Test Ntuples 0 - 4" << std::endl;

  }
  // for a simulation without detector response
  else
  {
    if(debug)
      std::cout << "Analysis::Book -> Weighting set to: " << WEIGHTED << std::endl;

    NRF(WEIGHTED); // 0
    IncidentChopper(WEIGHTED); // 1
    EmissionChopper(WEIGHTED); // 2
    IncidentIntObj(WEIGHTED); // 3
    EmissionIntObj(WEIGHTED); // 4
    IncidentShielding(WEIGHTED); // 5
    IncidentPlexiglass(WEIGHTED); // 6
    IncidentWater(WEIGHTED); // 7
    ScintillationPerEvent(WEIGHTED); // 8
    SteppingScintillation(WEIGHTED); // 9
    CherenkovPerEvent(WEIGHTED); // 10
    SteppingCherenkov(WEIGHTED); // 11
    PCDetection(WEIGHTED); // 12
    IncidentPC(WEIGHTED); // 13

    if(debug)
      std::cout << "Analysis::Book -> Without Detector Response Ntuples 0 - 13" << std::endl;
  }
  fFactoryOn = true;

  if(debug)
    std::cout << "Analysis::Book() --> Complete!" << std::endl;
}// end of Analysis::Book

void Analysis::finish()
{
  if(!fFactoryOn)
  {
    G4cerr << "ERROR Analysis::finish: Failed to write to file" << G4endl;
    return;
  }

  manager->Write();
  manager->CloseFile();

  std::cout << std::endl << "Analysis::finish -> Ntuples are saved." << std::endl;
  G4cout << G4endl << "Analysis::finish -> Ntuples are saved. " << G4endl;
  stop_time = time(0);
  G4cout << "Analysis::finish -> Time: ["
          << ((int)std::difftime(stop_time,start_time))/3600 << " h "
          << ((int)std::difftime(stop_time,start_time)%3600)/60 << " m "
          << ((int)std::difftime(stop_time,start_time)%3600)%60 << " s]"
          << G4endl;
  std::cout << "Analysis::finish -> Time: ["
              << ((int)std::difftime(stop_time,start_time))/3600 << " h "
              << ((int)std::difftime(stop_time,start_time)%3600)/60 << " m "
              << ((int)std::difftime(stop_time,start_time)%3600)%60 << " s]"
              << std::endl;

  delete manager;
  fFactoryOn = false;
}
