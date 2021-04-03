#include "EventMessenger.hh"


EventMessenger::EventMessenger(EventAction* EventAction)
        : EventA(EventAction)
{
  myDir = new G4UIdirectory("/mantisevent/");
  myDir->SetGuidance("Event Info Commands");
  Cmd = new G4UIcmdWithADouble("/mantisevent/eventInfoFreq",this);
  Cmd->SetGuidance("Choose Desired eventInfoFreq");
  Cmd->SetParameterName("eventInfo",false);
  Cmd->SetRange("eventInfo > 0 && eventInfo < 100000000");
}

EventMessenger::~EventMessenger()
{
  delete Cmd;
}


void EventMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
  if(command == Cmd)
  {
    G4double theCommand = Cmd->GetNewDoubleValue(newValue);
    EventA->SetEventInfoFreq((int)theCommand);
    G4cout << "EventInfo Frequency set to: " << theCommand << G4endl;
  }
}
