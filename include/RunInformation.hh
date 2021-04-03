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

#ifndef RunInformation_h
#define RunInformation_h 1

#include "globals.hh"
#include "G4Types.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

class RunInformation
{
  static RunInformation *instance;

  RunInformation();
public:
  static RunInformation *Instance()
  {
    if(!instance)
    {
      instance = new RunInformation;
    }
    return instance;
  }

  // Set Data Functions here
  void AddCerenkovEnergy(G4double en) {fCerenkovEnergy += en;}
  G4double GetTotalCerenkovEnergy()const {return fCerenkovEnergy;}
  void AddScintillationEnergy(G4double en) {fScintEnergy += en;}
  G4double GetTotalScintEnergy()const {return fScintEnergy;}
  void AddCerenkov(void) {fCerenkovCount++;} // changed from +=
  G4int GetTotalCerenkov()const {return fCerenkovCount;}
  void AddScintillation(void) {fScintCount++;}
  G4int GetTotalScintCount()const {return fScintCount;}
  void AddTotalSurface(void) {fTotalSurface += 1;}
  G4int GetTotalSurface()const {return fTotalSurface;}
  void AddNRF(void){fNRF++;}
  G4int GetTotalNRF()const {return fNRF;}
  void AddStatusKilledPosition(void){fStatusKilledPosition++;}
  G4int GetTotalKilledPosition()const {return fStatusKilledPosition;}
  void AddStatusKilledTime(void){fStatusKilledTime++;}
  G4int GetTotalKilledTime()const {return fStatusKilledTime;}
  void AddStatusKilledThetaAngle(void){fStatusKilledThetaAngle++;}
  G4int GetTotalKilledTheta()const {return fStatusKilledThetaAngle;}
  void AddStatusKilledPhiAngle(void){fStatusKilledPhiAngle++;}
  G4int GetTotalKilledPhi()const {return fStatusKilledPhiAngle;}

  G4double fCerenkovEnergy, fScintEnergy;
  G4int fTotalSurface, fCerenkovCount, fScintCount, fNRF, fStatusKilledPosition, fStatusKilledTime, fStatusKilledThetaAngle, fStatusKilledPhiAngle;
  ~RunInformation();
};

#endif
