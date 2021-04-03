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

#ifndef ChopperSetup_h
#define ChopperSetup_h 1

#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Tubs.hh" // for tube/cylinder
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "ChopperMessenger.hh"
#include "SourceInformation.hh"
#include "DetectorInformation.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class ChopperMessenger;

class ChopperSetup
{
public:
ChopperSetup();
~ChopperSetup();

G4VPhysicalVolume* Construct(G4LogicalVolume*, double, double, double, bool);
void SetChopperAbundance(G4double val){chopper_radio_abundance = val;}

void SetChopperMaterial(G4String val)
{
  if(val == "Uranium")
  {
    chopperDensity = 19.1*g/cm3;
  }
  else if(val == "Plutonium")
  {
    chopperDensity = 19.6*g/cm3;
  }
  else if(val == "Lead")
  {
    chopperDensity = 11.34*g/cm3;
  }
  else if(val == "Tungsten")
  {
    chopperDensity = 19.3*g/cm3;
  }
  else
  {
    G4cerr << "ChopperConstruction::SetChopperMaterial --> Chopper Density not Found." << G4endl;
  }
}

void SetChopper_z(G4double val)
{
        chopper_z = val;
        chopper_z = chopper_z*cm;
}

void SetChopperThick(G4double val)
{
        chopper_thick = val;
        chopper_thick = chopper_thick*mm;
}

void SetChopperOn(G4bool val){chopperOn = val;}

private:
// Chopper Properties
G4double chopperDensity, chopper_thick, chopper_z;
G4bool chopperOn;
G4double chopper_U235_abundance, chopper_U238_abundance, chopper_Pu239_abundance, chopper_Pu240_abundance;
G4double chopper_radio_abundance;
ChopperMessenger* chopperM;

};

#endif
