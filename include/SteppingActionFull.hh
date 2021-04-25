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

#ifndef SteppingActionFull_h
#define SteppingActionFull_h 1

#include "G4UserSteppingAction.hh"
#include "BaseSteppingAction.hh"

class SteppingActionFull : public G4UserSteppingAction, public BaseSteppingAction
{
public:
  SteppingActionFull(EventAction*);
  virtual ~SteppingActionFull();
  virtual UserSteppingAction(G4Step*);

private:
    void FillScintAndCherenkov(G4int, G4int, const std::vector<const G4Track*>*);
};

#endif
