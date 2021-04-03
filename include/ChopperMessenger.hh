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

#ifndef ChopperMessenger_h
#define ChopperMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "ChopperSetup.hh"
#include "G4UIdirectory.hh"

class ChopperSetup;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIdirectory;

class ChopperMessenger: public G4UImessenger
{
public:
  ChopperMessenger(ChopperSetup*);
  ~ChopperMessenger();

  void SetNewValue(G4UIcommand*, G4String); // must always be a string input
private:
  ChopperSetup* ChopperA;
  G4UIcmdWithAString* CmdChopMaterial;
  G4UIcmdWithADouble* CmdChopthick;
  G4UIcmdWithADouble* CmdChopZ;
  G4UIcmdWithAString* CmdChopperOn;
  G4UIcmdWithADouble* CmdChopperAbundance;

  G4UIdirectory *myDir;
};

#endif
