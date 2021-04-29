#ifndef EventMessenger_h
#define EventMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "BaseEventAction.hh"
#include "G4UIdirectory.hh"

class BaseEventAction;
class G4UIcmdWithADouble;
class G4UIdirectory;

class EventMessenger: public G4UImessenger
{
public:
  EventMessenger(BaseEventAction*);
  ~EventMessenger();

  void SetNewValue(G4UIcommand*, G4String); // must always be a string input
private:
  BaseEventAction* EventA;
  G4UIcmdWithADouble* Cmd;
  G4UIdirectory *myDir;
};

#endif
