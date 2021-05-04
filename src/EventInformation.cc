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

#include "EventInformation.hh"
#include "G4ios.hh"

EventInformation::EventInformation()
{
        weight = 0.;
        beamEnergy = 0.;
}

EventInformation::EventInformation(const G4Event*)
{
        weight = 0.; //aTrack->GetWeight();
        beamEnergy = 0.;
}

EventInformation::EventInformation(const EventInformation* anEventInfo)
{
        weight = anEventInfo->GetWeight();
        beamEnergy = anEventInfo->GetBeamEnergy();
}

EventInformation::~EventInformation()
{
}

void EventInformation::SetWeight(G4double x)
{
        weight = x;
}

void EventInformation::SetBeamEnergy(G4double x)
{
        beamEnergy = x;
}

void EventInformation::Print() const
{
}
