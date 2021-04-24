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

#include "DetBuildResPGAMessenger.hh"


DetBuildResPGAMessenger::DetBuildResPGAMessenger(DetBuildResPGA* PGA)
        : pga(PGA)
{
  myDir = new G4UIdirectory("/PGA/");
  myDir->SetGuidance("Primary Generator Action Commands");
  Cmd = new G4UIcmdWithADouble("/PGA/beam_size_x",this);
  Cmd->SetGuidance("Choose Desired Horizontal (x) Beam Size");
  Cmd->SetParameterName("beamSizeX",false);
  Cmd->SetRange("beamSizeX > 0. && beamSizeX < 80.0");
  Cmd2 = new G4UIcmdWithADouble("/PGA/beam_size_y",this);
  Cmd2->SetGuidance("Choose Desired Vertical (y) Beam Size");
  Cmd2->SetParameterName("beamSizeY",false);
  Cmd2->SetRange("beamSizeY > 0. && beamSizeY < 80.0");
}

DetBuildResPGAMessenger::~DetBuildResPGAMessenger()
{
  delete Cmd;
  delete Cmd2;
}


void DetBuildResPGAMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == Cmd)
  {
    G4double theCommand = Cmd->GetNewDoubleValue(newValue);
    pga->SetBeamSizeX(theCommand);
    G4cout << "DetBuildResPGA::SetBeamSizeX: " << theCommand << G4endl;
  }
  else if(command == Cmd2)
  {
    G4double theCommand = Cmd->GetNewDoubleValue(newValue);
    pga->SetBeamSizeY(theCommand);
    G4cout << "DetBuildResPGA::SetBeamSizeY: " << theCommand << G4endl;
  }
  else
  {
    G4cerr << "ERROR DetBuildResPGAMessenger::SetNewValue -> command != Cmd" << G4endl;
  }
}
