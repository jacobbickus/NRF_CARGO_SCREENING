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
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

#include "ChopperMessenger.hh"


ChopperMessenger::ChopperMessenger(ChopperSetup* ChopperAction)
        : ChopperA(ChopperAction)
{
  myDir = new G4UIdirectory("/chopper/");
  myDir->SetGuidance("Chopper Command Directory");
  CmdChopMaterial = new G4UIcmdWithAString("/chopper/material",this);
  CmdChopthick = new G4UIcmdWithADouble("/chopper/thickness", this);
  CmdChopZ = new G4UIcmdWithADouble("/chopper/distance", this);
  CmdChopperOn = new G4UIcmdWithAString("/chopper/state", this);
  CmdChopperAbundance = new G4UIcmdWithADouble("/chopper/abundance",this);

  CmdChopMaterial->SetGuidance("Choose desired Chopper Material");
  CmdChopthick->SetGuidance("Choose desired chopper thickness");
  CmdChopZ->SetGuidance("Choose desired chopper distance from brem beam");
  CmdChopperOn->SetGuidance("Choose desired chopper wheel state");
  CmdChopperAbundance->SetGuidance("Choose desired chopper wheel material isotope abundance(enrichment)");

  CmdChopMaterial->SetParameterName("ChopperMaterial",false);
  CmdChopZ->SetParameterName("chopperZ", false);
  CmdChopZ->SetRange("chopperZ > 0.5 && chopperZ < 6");
  CmdChopthick->SetParameterName("chopperthickness",false);
  CmdChopperOn->SetParameterName("chopperOn",false);
  CmdChopperAbundance->SetParameterName("chopperAbundance",false);
  CmdChopperAbundance->SetRange("chopperAbundance > 0 && chopperAbundance < 100");

  CmdChopperOn->SetCandidates("On on Off off");
  CmdChopMaterial->SetCandidates("Uranium Plutonium Lead Tungsten");

}

ChopperMessenger::~ChopperMessenger()
{
  delete CmdChopMaterial;
  delete CmdChopthick;
  delete CmdChopZ;
  delete CmdChopperOn;
  delete CmdChopperAbundance;
}


void ChopperMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue

  if(command == CmdChopZ)
  {
    G4double thecmdchopz = CmdChopZ->GetNewDoubleValue(newValue);
    ChopperA->SetChopper_z(thecmdchopz);
    //G4cout << "ChopperMessenger::SetNewValue -> The Chopper distance from the source manually set to: " << thecmdchopz << " cm" << G4endl;
  }
  else if(command == CmdChopthick)
  {
    G4double thecmdchopthick = CmdChopthick->GetNewDoubleValue(newValue);
    ChopperA->SetChopperThick(thecmdchopthick);
    //G4cout << "ChopperMessenger::SetNewValue -> The Chopper thickness manually set to: " << thecmdchopthick << " mm" <<G4endl;
  }
  else if(command == CmdChopperOn)
  {
    G4String thecmdchopperon = newValue;
    if(thecmdchopperon == "On" || thecmdchopperon == "on")
    {
      ChopperA->SetChopperOn(true);
      //G4cout << "ChopperMessenger::SetNewValue -> The Chopper state set to On!" << G4endl;
    }
    else
    {
      ChopperA->SetChopperOn(false);
      //G4cout << "ChopperMessenger::SetNewValue -> The Chopper state set to Off!" << G4endl;
    }
  }
  else if(command == CmdChopperAbundance)
  {
    G4double thechopperabundance = CmdChopperAbundance->GetNewDoubleValue(newValue);
    ChopperA->SetChopperAbundance(thechopperabundance);
    //G4cout << "ChopperMessenger::SetNewValue -> The Chopper isotope abundance manually set to: " << thechopperabundance << " percent" << G4endl;
  }

  else if(command == CmdChopMaterial)
  {
    G4String theCmdChopMaterial = newValue;
    ChopperA->SetChopperMaterial(theCmdChopMaterial);
    //G4cout << "ChopperMessenger::SetNewValue -> The chopper material manually set to: " << theCmdChopMaterial << G4endl;
  }
  else
  {
    G4cerr << "ERROR ChopperMessenger :: SetChopperInputValue command != Cmd" << G4endl;
  }
}
