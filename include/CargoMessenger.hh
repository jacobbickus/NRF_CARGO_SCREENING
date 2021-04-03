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

#ifndef CargoMessenger_h
#define CargoMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "Cargo.hh"
#include "G4UIdirectory.hh"

class Cargo;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;
class G4UIdirectory;

class CargoMessenger: public G4UImessenger
{
public:
  CargoMessenger(Cargo*);
  ~CargoMessenger();

  void SetNewValue(G4UIcommand*, G4String); // must always be a string input
private:
  Cargo* cargo;
  G4UIcmdWithADouble* Cmd;
  G4UIcmdWithADouble* Cmd1;
  G4UIcmdWithAString* Cmd2;
  G4UIcmdWithAString* CmdRemoveObjects;
  // Cargo SPHERE
  G4UIcmdWithAString* CmdAddSphere;
  G4UIcmdWithADouble* CmdSphereRadius;
  G4UIcmdWith3Vector* CmdSpherePosition;
  G4UIcmdWithAString* CmdSphereMaterial;
  // Cargo BOX
  G4UIcmdWithAString* CmdAddBox;
  G4UIcmdWith3Vector* CmdBoxSize;
  G4UIcmdWith3Vector* CmdBoxPosition;
  G4UIcmdWithAString* CmdBoxMaterial;

  G4UIdirectory *myDir;
  G4UIdirectory *myDir2;
  G4UIdirectory *myDir3;
};

#endif
