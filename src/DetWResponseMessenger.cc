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

#include "DetectorMessenger.hh"


DetWResponseMessenger::DetWResponseMessenger(DetWResponseFunction* DetectorAction)
        : detResA(DetectorAction)
{
  myDir = new G4UIdirectory("/mydet/");
  myDir->SetGuidance("Detector Setup Commands");
  CmdX = new G4UIcmdWithADouble("/mydet/WaterX",this);
  CmdY = new G4UIcmdWithADouble("/mydet/WaterY",this);
  CmdZ = new G4UIcmdWithADouble("/mydet/WaterZ",this);
  CmdAngle = new G4UIcmdWithADouble("/mydet/Angle",this);
  CmdAttenOn = new G4UIcmdWithAString("/mydet/attenuator", this);
  CmdAttenOn2 = new G4UIcmdWithAString("/mydet/attenuator2",this);
  CmdAttenThick = new G4UIcmdWithADouble("/mydet/attenuatorThickness",this);
  CmdAttenThick2 = new G4UIcmdWithADouble("/mydet/attenuatorThickness2",this);
  CmdAttenMat = new G4UIcmdWithAString("/mydet/attenuatorMaterial",this);
  CmdAttenMat2 = new G4UIcmdWithAString("/mydet/attenuatorMaterial2",this);
  CmdPlexi = new G4UIcmdWithADouble("/mydet/PlexiglassThickness",this);
  CmdCheckOverlaps = new G4UIcmdWithAString("/material/CheckOverlaps",this);
  CmdX->SetGuidance("Choose Desired X Size of Water Tank");
  CmdY->SetGuidance("Choose Desired Y Size of Water Tank");
  CmdZ->SetGuidance("Choose Desired Z Size of Water Tank");
  CmdAngle->SetGuidance("Choose desired Detector BackScatter Angle in Degrees");
  CmdAttenOn->SetGuidance("Choose if Attenuator Present or not");
  CmdAttenThick->SetGuidance("Choose Desired attenuator thickness");
  CmdAttenMat->SetGuidance("Choose desired attenuator material from NIST materials");
  CmdAttenOn2->SetGuidance("Choose if Attenuator Second Layer Present or not");
  CmdAttenThick2->SetGuidance("Choose Desired attenuator thickness");
  CmdAttenMat2->SetGuidance("Choose desired attenuator material from NIST materials");
  CmdPlexi->SetGuidance("Choose desired plexiglass thickness in mm");
  CmdCheckOverlaps->SetGuidance("Choose to check for geometric overlaps");
  CmdAngle->SetParameterName("Angle",false);
  CmdAngle->SetRange("Angle > 90 && Angle < 180");
  CmdAttenMat->SetCandidates("G4_Pb G4_Cu G4_Zn G4_Ag G4_Cd G4_Th G4_U G4_Au G4_W G4_Fe G4_POLYETHYLENE G4_POLYPROPYLENE G4_POLYSTYRENE G4_POLYVINYL_CHLORIDE G4_POLYCARBONATE");
  CmdAttenMat2->SetCandidates("G4_Pb G4_Cu G4_Zn G4_Ag G4_Cd G4_Th G4_U G4_Au G4_W G4_Fe G4_POLYETHYLENE G4_POLYPROPYLENE G4_POLYSTYRENE G4_POLYVINYL_CHLORIDE G4_POLYCARBONATE");
  CmdCheckOverlaps->SetCandidates("True true False false");
}

DetWResponseMessenger::~DetWResponseMessenger()
{
  delete CmdX;
  delete CmdY;
  delete CmdZ;
  delete CmdAngle;
  delete CmdAttenOn;
  delete CmdAttenThick;
  delete CmdAttenMat;
  delete CmdAttenOn2;
  delete CmdAttenThick2;
  delete CmdAttenMat2;
  delete CmdPlexi;
  delete CmdCheckOverlaps;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == CmdX)
  {
    G4double theCommandX = CmdX->GetNewDoubleValue(newValue);
    detResA->SetWaterX(theCommandX);
    G4cout << "The Water X-Dimension Size manually set to: " << theCommandX << " cm" << G4endl;
  }
  else if(command == CmdY)
  {
    G4double theCommandY = CmdY->GetNewDoubleValue(newValue);
    detResA->SetWaterY(theCommandY);
    G4cout << "The Water Y-Dimension Size manually set to: " << theCommandY << " cm" << G4endl;
  }
  else if(command == CmdZ)
  {
    G4double theCommandZ = CmdZ->GetNewDoubleValue(newValue);
    detResA->SetWaterZ(theCommandZ);
    G4cout << "The Water Z-Dimension Size manually set to: " << theCommandZ << " cm" << G4endl;
  }
  else if(command == CmdAngle)
  {
    G4double thecmdAngle = CmdAngle->GetNewDoubleValue(newValue);
    detResA->SettheAngle(thecmdAngle);
    G4cout << "The Detector angle manually set to: " << thecmdAngle << " degrees" << G4endl;
  }
  else if(command == CmdAttenOn)
  {
    G4String theCmdAttenOn = newValue;
    if(theCmdAttenOn == "Off" || theCmdAttenOn == "off")
    {
      detResA->SetAttenuatorState(false);
      detResA->SetAttenuatorMaterial("G4_AIR");
      check_atten_on = false;
    }
    else
    {
      check_atten_on = true;
      detResA->SetAttenuatorState(true);
    }
    G4cout << "The first attenuation layer manually set to: " << theCmdAttenOn << G4endl;
  }
  else if(command == CmdAttenThick)
  {
    if(check_atten_on)
    {
      G4double theCmdAttendThickness = CmdAttenThick->GetNewDoubleValue(newValue);
      detResA->SetAttenuatorThickness(theCmdAttendThickness);
      G4cout << "The first attenuation layer thickness manually set to: " << theCmdAttendThickness << " cm" << G4endl;
    }
  }
  else if(command == CmdAttenMat)
  {
    if(check_atten_on)
    {
      G4String theAttenMaterial = newValue;
      detResA->SetAttenuatorMaterial(theAttenMaterial);
      G4cout << "The first attenuation layer material manually set to: " << theAttenMaterial << G4endl;
    }
  }
  else if(command == CmdAttenOn2)
  {
    G4String theCmdAttenOn2 = newValue;
    if(theCmdAttenOn2 == "Off" || theCmdAttenOn2 == "off")
    {
      detResA->SetAttenuatorState2(false);
      detResA->SetAttenuatorMaterial2("G4_AIR");
      check_atten2_on = false;
    }
    else
    {
      check_atten2_on = true;
      detResA->SetAttenuatorState2(true);
    }
    G4cout << "The second attenuation layer manually set to: "
            << theCmdAttenOn2 << G4endl;
  }
  else if(command == CmdAttenThick2)
  {
    if(check_atten2_on)
    {
      G4double theCmdAttendThickness2 = CmdAttenThick2->GetNewDoubleValue(newValue);
      detResA->SetAttenuatorThickness2(theCmdAttendThickness2);
      G4cout << "The second attenuation layer thickness manually set to: " << theCmdAttendThickness2 << " cm" << G4endl;
    }
  }
  else if(command == CmdAttenMat2)
  {
    if(check_atten2_on)
    {
      G4String theAttenMaterial2 = newValue;
      detResA->SetAttenuatorMaterial2(theAttenMaterial2);
      G4cout << "The second attenuation layer material manually set to: " << theAttenMaterial2 << G4endl;
    }
  }
  else if(command == CmdPlexi)
  {
    G4double plexiThickness = CmdPlexi->GetNewDoubleValue(newValue);
    detResA->SetPlexiThickness(plexiThickness);
    G4cout << "The Plexiglass thickness manually set to: " << plexiThickness << " cm" << G4endl;
  }
  else if(command == CmdCheckOverlaps)
  {
    G4String theCheck = newValue;
    G4bool theCheckBool = true;
    if(theCheck == "false" || theCheck == "False")
    {
      G4cout << "CheckOverlaps set to off!" << G4endl;
      theCheckBool = false;
    }
    detResA->SetCheckOverlaps(theCheckBool);
  }
  else
  {
    G4cerr << "ERROR DetectorMessenger :: SetDetectorInputValue command != Cmd" << G4endl;
  }
}
