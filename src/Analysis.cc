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

Analysis::Analysis() : fFactoryOn(false), WEIGHTED(false)
{
  start_time = time(0);
  if(!inFile.compare("importance_sampling_input.root"))
    WEIGHTED = true;

  if(resonanceTest)
    WEIGHTED = false;
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
    NRF(manager); //             0
    BremRadiator(manager); //    1
    BremBacking(manager); //     2
    IncidentChopper(manager); // 3
    EmissionChopper(manager); // 4

    if(debug)
      std::cout << "Analysis::Book -> Brem Test Ntuples 0 - 2" << std::endl;
  }
  // for Creating Detector Response Function
  else if(detTest)
  {
    IncidentShielding(manager);  // 0
    IncidentPlexiglass(manager); // 1
    DetectorResponse(manager);   // 2
    if(debug)
      std::cout << "Analysis::Book -> Creating Detector Response Function Ntuples 0 - 1" << std::endl;
  }
  // for a Simulation with Detector Response
  else if(WResponseFunction)
  {
    if(debug)
      std::cout << "Analysis::Book -> Weighting set to: " << WEIGHTED << std::endl;
    NRF(manager, WEIGHTED); // 0
    IncidentIntObj(manager, WEIGHTED); // 1
    EmissionIntObj(manager, WEIGHTED); // 2
    IncidentShielding(manager, WEIGHTED); // 3
    IncidentPlexiglass(manager, WEIGHTED); // 4
    DetectorEstimatedInfo(manager, WEIGHTED); // 5
    if(debug)
      std::cout << "Analysis::Book -> With Detector Response Ntuples 0 - 5" << std::endl;
  }
  // for a simulation without detector response
  else
  {
    if(debug)
      std::cout << "Analysis::Book -> Weighting set to: " << WEIGHTED << std::endl;

    NRF(manager, WEIGHTED); // 0
    IncidentChopper(manager,WEIGHTED); // 1
    EmissionChopper(manager,WEIGHTED); // 2
    IncidentIntObj(manager, WEIGHTED); // 3
    EmissionIntObj(manager, WEIGHTED); // 4
    IncidentShielding(manager, WEIGHTED); // 5
    IncidentPlexiglass(manager, WEIGHTED); // 6
    IncidentWater(manager,WEIGHTED); // 7
    ScintillationPerEvent(manager,WEIGHTED); // 8
    SteppingScintillation(manager, WEIGHTED); // 9
    CherenkovPerEvent(manager,WEIGHTED); // 10
    SteppingCherenkov(manager, WEIGHTED); // 11
    PCDetection(manager, WEIGHTED); // 12
    IncidentPC(manager, WEIGHTED); // 13

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

  G4AnalysisManager* manager = G4AnalysisManager::Instance();
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
