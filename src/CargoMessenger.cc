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

#include "CargoMessenger.hh"

CargoMessenger::CargoMessenger(Cargo* Cargo)
: cargo(Cargo)
{
  myDir = new G4UIdirectory("/IntObj/");
  myDir->SetGuidance("Interrogation Object Options");
  Cmd = new G4UIcmdWithADouble("/IntObj/Radius",this);
  Cmd1 = new G4UIcmdWithADouble("/IntObj/Abundance",this);
  Cmd2 = new G4UIcmdWithAString("/IntObj/Material",this);

  Cmd->SetGuidance("Choose Desired radius Size of Interrogation Target");
  Cmd1->SetGuidance("Choose Desired fission isotope abundance(enrichment) of Interrogation Target");
  Cmd2->SetGuidance("Choose Desired target");

  Cmd1->SetParameterName("abundance",false);
  Cmd1->SetRange("abundance > 0 && abundance < 100");
  Cmd2->SetCandidates("Uranium NaturalU Plutonium NaturalPu Lead Steel Plastic");

  myDir2 = new G4UIdirectory("/Container/");
  myDir2->SetGuidance("Container Options");
  CmdRemoveObjects = new G4UIcmdWithAString("Container/Remove",this);
  CmdRemoveObjects->SetGuidance("Choose to remove objects for simulation");
  CmdRemoveObjects->SetCandidates("True true False false");

  myDir3 = new G4UIdirectory("/Cargo/");
  myDir3->SetGuidance("Cargo Object Options");

  CmdAddSphere = new G4UIcmdWithAString("/Cargo/AddSphere",this);
  CmdAddSphere->SetCandidates("True true False false");

  CmdSphereRadius = new G4UIcmdWithADouble("/Cargo/SphereRadius",this);
  CmdSphereRadius->SetParameterName("radius",false);
  CmdSphereRadius->SetRange("radius>0");

  CmdSpherePosition = new G4UIcmdWith3Vector("/Cargo/SpherePosition",this);

  CmdSphereMaterial = new G4UIcmdWithAString("/Cargo/SphereMaterial",this);

  CmdAddBox = new G4UIcmdWithAString("/Cargo/AddBox",this);
  CmdAddBox->SetCandidates("True true False false");

  CmdBoxSize = new G4UIcmdWith3Vector("/Cargo/BoxSize",this);

  CmdBoxPosition = new G4UIcmdWith3Vector("/Cargo/BoxPosition",this);

  CmdBoxMaterial = new G4UIcmdWithAString("/Cargo/BoxMaterial",this);

}

CargoMessenger::~CargoMessenger()
{
  delete Cmd;
  delete Cmd1;
  delete Cmd2;

  delete CmdRemoveObjects;

  delete CmdAddSphere;
  delete CmdSphereRadius;
  delete CmdSpherePosition;
  delete CmdSphereMaterial;

  delete CmdAddBox;
  delete CmdBoxSize;
  delete CmdBoxPosition;
  delete CmdBoxMaterial;
}

void CargoMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == Cmd)
  {
    G4double theCommandtX = Cmd->GetNewDoubleValue(newValue);
    cargo->SetIntObj_radius(theCommandtX);
  }
  else if(command == Cmd1)
  {
    G4double theCommandtrad = Cmd1->GetNewDoubleValue(newValue);
    cargo->SetIntObjAbundance(theCommandtrad);
  }
  else if(command == Cmd2)
  {
    G4String theCommandtsel = newValue;
    cargo->SetIntObj(theCommandtsel);
  }
  else if(command == CmdRemoveObjects)
  {
    G4String theCommand = newValue;
    if(theCommand == "True" || theCommand == "true")
      cargo->SetRemoveContainer(true);
  }

  // Cargo SPHERE
  else if(command == CmdAddSphere)
  {
    G4String theCommand = newValue;
    if(theCommand == "True" || theCommand == "true")
      cargo->AddCargoSphere();
  }
  else if(command == CmdSphereRadius)
  {
    G4double theCommand = CmdSphereRadius->GetNewDoubleValue(newValue);
    cargo->SetCargoSphereRadius(theCommand);
  }
  else if(command == CmdSpherePosition)
  {
    G4ThreeVector theCommand = CmdSpherePosition->GetNew3VectorValue(newValue);
    cargo->SetCargoSpherePosition(theCommand);
  }
  else if(command == CmdSphereMaterial)
  {
    G4String theCommand = newValue;
    cargo->SetCargoSphereMaterial(theCommand);
  }

  // Cargo BOX
  else if(command == CmdAddBox)
  {
    G4String theCommand = newValue;
    if(theCommand == "True" || theCommand == "true")
    {
      cargo->AddCargoBox();
    }
  }
  else if(command == CmdBoxSize)
  {
    G4ThreeVector theCommand = CmdBoxSize->GetNew3VectorValue(newValue);
    cargo->SetCargoBoxSize(theCommand);
  }
  else if(command == CmdBoxPosition)
  {
    G4ThreeVector theCommand = CmdBoxPosition->GetNew3VectorValue(newValue);
    cargo->SetCargoBoxPosition(theCommand);
  }
  else if(command == CmdBoxMaterial)
  {
    G4String theCommand = newValue;
    cargo->SetCargoBoxMaterial(theCommand);
  }
}
