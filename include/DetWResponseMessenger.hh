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

#ifndef DetWResponseMessenger_h
#define DetWResponseMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "DetWResponseFunction.hh"
#include "G4UIdirectory.hh"

class DetWResponseFunction;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIdirectory;

class DetWResponseMessenger : public G4UImessenger
{
public:
   DetWResponseMessenger(DetWResponseFunction*);
  ~DetWResponseMessenger();
  void SetNewValue(G4UIcommand*, G4String);

private:
  DetWResponseFunction* detResA;
  G4UIcmdWithADouble* CmdX;
  G4UIcmdWithADouble* CmdY;
  G4UIcmdWithADouble* CmdZ;
  G4UIcmdWithADouble* CmdAngle;
  G4UIcmdWithAString* CmdAttenOn;
  G4bool check_atten_on = false;
  G4UIcmdWithADouble* CmdAttenThick;
  G4UIcmdWithAString* CmdAttenMat;
  G4UIcmdWithAString* CmdAttenOn2;
  G4bool check_atten2_on = false;
  G4UIcmdWithADouble* CmdAttenThick2;
  G4UIcmdWithAString* CmdAttenMat2;
  G4UIcmdWithADouble* CmdPlexi;
  G4UIcmdWithAString* CmdCheckOverlaps;

  G4UIdirectory *myDir;
};

#endif
