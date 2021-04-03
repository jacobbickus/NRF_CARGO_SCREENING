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

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "G4Types.hh"

extern G4bool debug;

ActionInitialization::ActionInitialization()
        : G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::Build() const
{
    if(debug)
        std::cout << "ActionInitialization::Build() -> Begin!" << std::endl;

    HistoManager* histo = new HistoManager();
    PrimaryGeneratorAction* pga = new PrimaryGeneratorAction();
    SetUserAction(pga);
    SetUserAction(new RunAction(histo,pga));
    EventAction* event = new EventAction();
    SetUserAction(event);
    SetUserAction(new SteppingAction(event));
    SetUserAction(new StackingAction());

    if(debug)
        std::cout << "ActionInitialization::Build() -> End!" << std::endl;
}
