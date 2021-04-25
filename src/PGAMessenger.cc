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


PGAMessenger::PGAMessenger(BasePGA* pga_in)
        : pga(pga_in)
{
  myDir = new G4UIdirectory("/PGA/");
  myDir->SetGuidance("Primary Generator Action Commands");
  Cmd = new G4UIcmdWithADouble("/PGA/beamSize",this);
  Cmd->SetGuidance("Choose Desired Beam Size");
  Cmd->SetParameterName("beamSize",false);
  Cmd->SetRange("beamSize > 0. && beamSize < 80.0");
  Cmd2 = new G4UIcmdWithADouble("/PGA/beam_size_x",this);
  Cmd2->SetGuidance("Choose Desired Horizontal (x) Beam Size");
  Cmd2->SetParameterName("beamSizeX",false);
  Cmd2->SetRange("beamSizeX > 0. && beamSizeX < 100.0");
  Cmd3 = new G4UIcmdWithADouble("/PGA/beam_size_y",this);
  Cmd3->SetGuidance("Choose Desired Vertical (y) Beam Size");
  Cmd3->SetParameterName("beamSizeY",false);
  Cmd3->SetRange("beamSizeY > 0. && beamSizeY < 300.0");
}

PGAMessenger::~PGAMessenger()
{
  delete Cmd;
  delete Cmd2;
  delete Cmd3;
}


void PGAMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
  if(command == Cmd)
  {
    G4double theCommand = Cmd->GetNewDoubleValue(newValue);
    pga->SetBeamSize(theCommand);
  }
  if(command == Cmd2)
  {
    G4double theCommand = Cmd2->GetNewDoubleValue(newValue);
    pga->SetBeamSizeX(theCommand);
    G4cout << "PGAMEssenger::SetBeamSizeX: " << theCommand << G4endl;
  }
  else if(command == Cmd3)
  {
    G4double theCommand = Cmd3->GetNewDoubleValue(newValue);
    pga->SetBeamSizeY(theCommand);
    G4cout << "PGAMessenger::SetBeamSizeY: " << theCommand << G4endl;
  }
}
