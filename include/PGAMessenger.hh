#ifndef PGAMessenger_h
#define PGAMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"

class PrimaryGeneratorAction;
class G4UIcmdWithADouble;
class G4UIdirectory;

class PGAMessenger: public G4UImessenger
{
public:
  PGAMessenger(PrimaryGeneratorAction*);
  ~PGAMessenger();

  void SetNewValue(G4UIcommand*, G4String); // must always be a string input
private:
  PrimaryGeneratorAction* pga;
  G4UIcmdWithADouble *Cmd, *Cmd1;
  G4UIdirectory *myDir;
};

#endif
