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

#ifndef DetBuildResPGAMessenger_h
#define DetBuildResPGAMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "DetBuildResPGA.hh"
#include "G4UIdirectory.hh"

class DetBuildResPGA;
class G4UIcmdWithADouble;
class G4UIdirectory;

class DetBuildResPGAMessenger: public G4UImessenger
{
public:
  DetBuildResPGAMessenger(DetBuildResPGA*);
  ~DetBuildResPGAMessenger();

  void SetNewValue(G4UIcommand*, G4String); // must always be a string input
private:
  DetBuildResPGA* pga;
  G4UIcmdWithADouble *Cmd;
  G4UIcmdWithADouble *Cmd2;
  G4UIdirectory *myDir;
};

#endif
