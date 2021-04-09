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

#include "PGAMessenger.hh"


PGAMessenger::PGAMessenger(PrimaryGeneratorAction* pga_in)
        : pga(pga_in)
{
  myDir = new G4UIdirectory("/PGA/");
  myDir->SetGuidance("Primary Generator Action Commands");
  Cmd = new G4UIcmdWithADouble("/PGA/beamSize",this);
  Cmd->SetGuidance("Choose Desired Beam Size");
  Cmd->SetParameterName("beamSize",false);
  Cmd->SetRange("beamSize > 0. && beamSize < 80.0");
}

PGAMessenger::~PGAMessenger()
{
  delete Cmd;
}


void PGAMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
  if(command == Cmd)
  {
    G4double theCommand = Cmd->GetNewDoubleValue(newValue);
    pga->SetBeamSize(theCommand);
  }
  else
  {
    G4cerr << "ERROR PGAMessenger::SetNewValue -> command != Cmd" << G4endl;
  }
}
