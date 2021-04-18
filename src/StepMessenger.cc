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

#include "StepMessenger.hh"

extern G4bool WResponseFunction;

StepMessenger::StepMessenger(SteppingAction* stepAction)
        : stepA(stepAction)
{
  myDir = new G4UIdirectory("/output/");
  myDir->SetGuidance("Output Commands");
  Cmd = new G4UIcmdWithAString("/output/myoutput",this);
  Cmd->SetGuidance("Choose Desired Outputs");
  Cmd->SetGuidance("Choice: ChopIncData, ChopOutData, NRFData, IntObjInData, IntObjOutData, ShieldingData, PlexiData, WaterIncData, ScintillationData, ScintillationData2, CherenkovData, CherenkovData2, DetData, none (default)");
  Cmd->SetParameterName("choice",false);
  Cmd->SetDefaultValue("none");
  Cmd->SetCandidates("ChopIncData ChopOutData NRFData IntObjInData IntObjOutData ShieldingData PlexiData WaterIncData ScintillationData ScintillationData2 CherenkovData CherenkovData2 DetData none");
}

StepMessenger::StepMessenger(SteppingWResponseFunction* stepAction)
: stepAWResponseFunction(stepAction)
{
  myDir = new G4UIdirectory("/output/");
  myDir->SetGuidance("Output Commands");
  Cmd = new G4UIcmdWithAString("/output/myoutput",this);
  Cmd->SetGuidance("Choose Desired Outputs");
  Cmd->SetGuidance("Choice: ChopIncData, ChopOutData, NRFData, IntObjInData, IntObjOutData, ShieldingData, PlexiData, WaterIncData, ScintillationData, ScintillationData2, CherenkovData, CherenkovData2, DetData, none (default)");
  Cmd->SetParameterName("choice",false);
  Cmd->SetDefaultValue("none");
  Cmd->SetCandidates("ChopIncData ChopOutData NRFData IntObjInData IntObjOutData ShieldingData PlexiData WaterIncData ScintillationData ScintillationData2 CherenkovData CherenkovData2 DetData none");
}

StepMessenger::~StepMessenger()
{
  delete Cmd;
}


void StepMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
  if(command == Cmd)
  {
    G4String theCommand = newValue;
    if(theCommand == "ChopIncData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetChopperIncDataFlag(1);
      else
        stepA->SetChopperIncDataFlag(1);

      G4cout << G4endl << "User Selected Chopper Incident Data." << G4endl;
    }
    else if(theCommand == "ChopOutData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetChopperOutDataFlag(1);
      else
        stepA->SetChopperOutDataFlag(1);

      G4cout << G4endl << "User Selected Chopper Emission Data." << G4endl;
    }
    else if(theCommand == "NRFData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetNRFDataFlag(1);
      else
        stepA->SetNRFDataFlag(1);

      G4cout << G4endl << "User Selected NRF Data." << G4endl;
    }
    else if(theCommand == "IntObjInData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetIntObjInDataFlag(1);
      else
        stepA->SetIntObjInDataFlag(1);

      G4cout << G4endl << "User Selected Interrogation Object Incident Data." << G4endl;
    }
    else if(theCommand == "IntObjOutData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetIntObjOutDataFlag(1);
      else
        stepA->SetIntObjOutDataFlag(1);

      G4cout << G4endl << "User Selected Interrogation Object Emission Data." << G4endl;
    }
    else if(theCommand == "ShieldingData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetShieldingIncDataFlag(1);
      else
        stepA->SetShieldingIncDataFlag(1);

      G4cout << G4endl << "User Selected Incident Shielding Data." << G4endl;
    }
    else if(theCommand == "PlexiData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetPlexiIncDataFlag(1);
      else
        stepA->SetPlexiIncDataFlag(1);

      G4cout << G4endl << "User Selected Incident Plexiglass Data." << G4endl;
    }
    else if(theCommand == "WaterIncData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetWaterIncDataFlag(1);
      else
        stepA->SetWaterIncDataFlag(1);

      G4cout << G4endl << "User Selected Incident Water Data." <<G4endl;
    }
    else if(theCommand == "ScintillationData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetScintillationDataFlag(1);
      else
        stepA->SetScintillationDataFlag(1);

      G4cout << G4endl << "User Selected Scintillation Data." << G4endl;
    }
    else if(theCommand == "ScintillationData2")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetScintillation2DataFlag(1);
      else
        stepA->SetScintillation2DataFlag(1);

      G4cout << G4endl << "User Selected Scintillation2 Data." << G4endl;
    }
    else if(theCommand == "CherenkovData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetCherenkovDataFlag(1);
      else
        stepA->SetCherenkovDataFlag(1);

      G4cout << G4endl << "User Selected Cherenkov Data." <<G4endl;
    }
    else if(theCommand == "CherenkovData2")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetCherenkov2DataFlag(1);
      else
        stepA->SetCherenkov2DataFlag(1);

      G4cout << G4endl << "User Selected Cherenkov2 Data." << G4endl;
    }
    else if(theCommand == "DetData")
    {
      if(WResponseFunction)
        stepAWResponseFunction->SetDetDataFlag(1);
      else
        stepA->SetDetDataFlag(1);

      G4cout << G4endl << "User Selected Detected Data."<<G4endl;
    }
    else if(theCommand == "none")
    {
            G4cout << G4endl << "No output requested." << G4endl;
    }
    else
    {
      G4cerr << "ERROR: StepMessenger::SetStepInputValue Command not found." << G4endl;
    }

  }
  else
  {
    G4cerr << "ERROR StepMessenger :: SetStepInputValue command != Cmd" << G4endl;
  }
}
