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

#ifndef MaterialProperties_h
#define MaterialProperties_h 1

#include "G4SystemOfUnits.hh"
#include "G4MaterialTable.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4MaterialPropertiesTable.hh"
#include <cassert>

class MaterialProperties
{
public:
  MaterialProperties();
  ~MaterialProperties();
  G4MaterialPropertiesTable* SetWaterProperties();
  G4MaterialPropertiesTable* SetCasingProperties();
  G4MaterialPropertiesTable* SetCasingOpticalProperties();
  G4MaterialPropertiesTable* SetTapeProperties();
  G4MaterialPropertiesTable* SetTapeOpticalProperties();
  G4MaterialPropertiesTable* SetPMTProperties();
  G4MaterialPropertiesTable* SetPMTOpticalProperties();
  G4MaterialPropertiesTable* SetPCProperties(G4String);
  G4MaterialPropertiesTable* SetAirProperties();

private:
  G4double PHOTON_ENERGY_IOF, PHOTON_ENERGY_ABS, WATER_REF, WATER_ABS, PHOTON_ENERGY_SCINT, WATER_SCINT;
};

#endif
