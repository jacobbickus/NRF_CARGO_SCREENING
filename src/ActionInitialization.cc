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
#include "PGA.hh"
#include "PGABremTest.hh"
#include "PGADetResponseTest.hh"
#include "PGAIntObj.hh"
#include "RunAction.hh"
#include "SteppingActionFull.hh"
#include "SteppingBremTest.hh"
#include "SteppingDetTest.hh"
#include "SteppingIntObj.hh"
#include "SteppingWithoutChopper.hh"
#include "SteppingWResponseFunction.hh"
#include "StackingAction.hh"
#include "EventAction.hh"
#include "EventActionWResponseFunction.hh"
#include "Analysis.hh"
#include "G4Types.hh"
#include "DetectorResponseFunction.hh"
#include "SourceInformation.hh"

extern G4bool debug;
extern G4bool detTest;
extern G4bool bremTest;
extern G4bool WResponseFunction;
extern G4bool run_without_chopper;
extern G4bool IntObjTest;
extern G4bool run_without_chopper;

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

    // Check to make sure inputs wouldn't cause logic fail...
    CheckInputs();
    Analysis* analysis = new Analysis();

    // PRIMARY GENERATOR ACTION DEPENDS ON TYPE OF TEST
    // SetUserAction(G4USERPRIMARYGENERATORACTION)
    // -t1
    if(detTest)
      SetUserAction(new PGADetResponseTest());
    // -t2
    else if(bremTest)
      SetUserAction(new PGABremTest());
    // -t5
    else if(run_without_chopper)
      SetUserAction(new PGAIntObj());
    // ANY OTHER TYPE OF SIMULATION
    else
      SetUserAction(new PGA());

    // RUN ACTION NEVER CHANGES
    // SetUserAction(G4USERRUNACTION)
    SetUserAction(new RunAction(analysis));

    // EVENT ACTION CAN EITHER BE WResponseFunction or !WResponseFunction
    // SetUserAction(G4USEREVENTACTION)
    EventActionWResponseFunction* eventWResponseFunction = 0;
    EventAction* event = 0;

    if(WResponseFunction)
    {
      SourceInformation* sInfo = SourceInformation::Instance();
      G4double beamMax = sInfo->GetBeamMax();

      if(debug)
        std::cout << "ActionInitialization::Build() -> Beam Max Energy: " << beamMax << " MeV" << std::endl;

      DetectorResponseFunction* r_function = DetectorResponseFunction::Instance(beamMax);
      eventWResponseFunction = new EventActionWResponseFunction();
      SetUserAction(eventWResponseFunction);
    }
    else
    {
      event = new EventAction();
      SetUserAction(event);
    }

    // SetUserAction(G4USERSTEPPINGACTION)
    // -t2
    if(bremTest)
    {
      if(debug)
        std::cout << "ActionInitialization::Build -> SteppingBremTest." << std::endl;

      SetUserAction(new SteppingBremTest());
    }
    // -t1
    if(detTest)
    {
      if(debug)
        std::cout << "ActionInitialization::Build -> SteppingDetTest." << std::endl;

      SetUserAction(new SteppingDetTest(event));
    }
    // -t6
    if(IntObjTest)
    {
      if(debug)
        std::cout << "ActionInitialization::Build -> SteppingIntObj." << std::endl;

      SetUserAction(new SteppingIntObj(event));
    }

    // if WResponseFunction ALWAYS USE SteppingWResponseFunction
    // -r --Detector_Response_File
    // USER CAN RUN WResponseFunction && run_without_chopper (-r, -t5)
    // THIS IS THE MOST OPTIMIZED SIMULATION (AND LEAST PRECISE)
    if(WResponseFunction)
    {
      if(debug)
        std::cout << "ActionInitialization::Build -> SteppingWResponseFunction." << std::endl;

      SetUserAction(new SteppingWResponseFunction(eventWResponseFunction));
    }
    // ELSE IF !WResponseFunction CAN EITHER BE A RUN WITHOUT CHOPPER TEST or A FULL SIMULATION RUN
    else
    {
      // -t5
      if(run_without_chopper)
      {
        if(debug)
          std::cout << "ActionInitialization::Build -> SteppingWithoutChopper." << std::endl;

        SetUserAction(new SteppingWithoutChopper(event));
      }
      // USER IS NOT RUNNING ANY SPECIAL SIMULATION TESTS --> FULL SIMULATION RUN (MOST PRECISE)
      else
      {
        if(debug)
          std::cout << "ActionInitialization::Build -> SteppingActionFull." << std::endl;

        SetUserAction(new SteppingActionFull(event));
      }
    }

    // STACKING ACTION DOES NOT CHANGE
    SetUserAction(new StackingAction());

    if(debug)
        std::cout << "ActionInitialization::Build() -> End!" << std::endl;
}




void ActionInitialization::CheckInputs()const
{
  // Once again conduct USER ERROR CHECKS
  if(bremTest && detTest)
  {
    G4cerr << "ActionInitialization::Build -> FATAL ERROR CANNOT Conduct Bremsstrahlung Test and Detector Response Test in the same run." << G4endl;
    exit(1);
  }

  if(bremTest && WResponseFunction)
  {
    G4cerr << "ActionInitialization::Build -> FATAL ERROR CANNOT Conduct Bremsstrahlung Test With A Response Function." << G4endl;
    exit(1);
  }

  if(detTest && WResponseFunction)
  {
    G4cerr << "ActionInitialization::Build -> FATAL ERROR CANNOT Conduct Detector Response Test with a Response Function!" << G4endl;
    exit(1);
  }

  if(IntObjTest && WResponseFunction)
  {
    G4cerr << "ActionInitialization::Build -> FATAL ERROR CANNOT Conduct Interrogation Object Beam Test with Response Function!" << G4endl;
    exit(1);
  }

  if(bremTest && IntObjTest)
  {
    G4cerr << "ActionInitialization::Build -> FATAL ERROR CANNOT Conduct Bremsstrahlung Test with Interrogation Object Test!" << G4endl;
    exit(1);
  }

  if(detTest && IntObjTest)
  {
    G4cerr << "ActionInitialization::Build -> FATAL ERROR CANNOT Conduct Detector Response Test with an Interrogation Object Test!" << G4endl;
    exit(1);
  }
}
