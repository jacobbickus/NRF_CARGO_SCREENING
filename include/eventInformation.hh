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

#ifndef EventInformation_h
#define EventInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4Allocator.hh"
#include "G4VUserEventInformation.hh"

class EventInformation : public G4VUserEventInformation {
public:
EventInformation();
EventInformation(const G4Event*);
EventInformation(const EventInformation*);
virtual ~EventInformation();

inline G4double GetWeight() const
{
  return weight;
}
void SetWeight(G4double);

inline G4double GetBeamEnergy() const
{
  return beamEnergy;
}
void SetBeamEnergy(G4double);
void Print() const;
private:
G4double weight;
G4double beamEnergy;
};

#endif
