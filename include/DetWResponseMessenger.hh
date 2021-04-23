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

#ifndef DetWResponseMessenger_h
#define DetWResponseMessenger_h 1

#include "globals.hh"
#include "DetWResponseFunction.hh"
#include "DetectorMessenger.hh"

class DetWResponseFunction;

class DetWResponseMessenger : public DetectorMessenger
{

public:
   DetWResponseMessenger(DetWResponseFunction*);
  ~DetWResponseMessenger();
  void SetNewValue(G4UIcommand*, G4String);

private:
  DetWResponseFunction* detResA;
};

#endif
