#ifndef EventMessenger_h
#define EventMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "EventAction.hh"
#include "EventActionWResponseFunction.hh"
#include "G4UIdirectory.hh"

class EventAction;
class EventActionWResponseFunction;
class G4UIcmdWithADouble;
class G4UIdirectory;

class EventMessenger: public G4UImessenger
{
public:
  EventMessenger(EventAction*);
  EventMessenger(EventActionWResponseFunction*);
  ~EventMessenger();

  void SetNewValue(G4UIcommand*, G4String); // must always be a string input
private:
  EventAction* EventA;
  EventActionWResponseFunction* EventAWResponse;
  G4UIcmdWithADouble* Cmd;
  G4UIcmdWithADouble* Cmd2;
  G4UIdirectory *myDir;
};

#endif
