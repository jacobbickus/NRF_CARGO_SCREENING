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
#include "DetResponsePrimaryGenerator.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "SteppingBremTest.hh"
#include "SteppingDetTest.hh"
#include "SteppingWResponseFunction.hh"
#include "StackingAction.hh"
#include "EventAction.hh"
#include "EventActionWResponseFunction.hh"
#include "Analysis.hh"
#include "G4Types.hh"
#include "DetectorResponseFunction.hh"

extern G4bool debug;
extern G4bool detTest;
extern G4bool bremTest;
extern G4bool WResponseFunction;

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

    Analysis* analysis = new Analysis();

    DetResponsePrimaryGenerator* detResponsePga = 0;
    PrimaryGeneratorAction* pga = 0;
    if(detTest)
    {
      detResponsePga = new DetResponsePrimaryGenerator();
      SetUserAction(detResponsePga);
      SetUserAction(new RunAction(analysis, detResponsePga));
    }
    else
    {
      pga = new PrimaryGeneratorAction();
      SetUserAction(pga);
      SetUserAction(new RunAction(analysis,pga));
    }

    EventActionWResponseFunction* eventWResponseFunction = 0;
    EventAction* event = 0;

    if(WResponseFunction)
    {
      if(debug)
        std::cout << "ActionInitialization::Build() -> Beam Max Energy: " << pga->GetBeamMax() << " MeV" << std::endl;

      DetectorResponseFunction* r_function = DetectorResponseFunction::Instance(pga->GetBeamMax());
      eventWResponseFunction = new EventActionWResponseFunction();
      SetUserAction(eventWResponseFunction);
    }
    else
    {
      event = new EventAction();
      SetUserAction(event);
    }

    if(bremTest)
      SetUserAction(new SteppingBremTest(event));
    else if(detTest)
      SetUserAction(new SteppingDetTest(event));
    else if(WResponseFunction)
    {
      if(debug)
        std::cout << "ActionInitialization::Build -> SteppingWResponseFunction." << std::endl;
        SetUserAction(new SteppingWResponseFunction(eventWResponseFunction));
    }
    else
      SetUserAction(new SteppingAction(event));

    SetUserAction(new StackingAction());

    if(debug)
        std::cout << "ActionInitialization::Build() -> End!" << std::endl;
}
