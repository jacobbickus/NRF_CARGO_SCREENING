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

#ifndef Cargo_h
#define Cargo_h 1

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
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4NistManager.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "CargoMessenger.hh"
#include "DetectorInformation.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class CargoMessenger;

class Cargo
{
public:
Cargo();
~Cargo();

void Construct(G4LogicalVolume*, double, double, double, bool);
void CheckCargoSphereSize();

void AddCargoSphere(){cargo_spheres++;};
void SetCargoSphereRadius(double r){cargo_sphere_radii.push_back(r);};

void SetCargoSpherePosition(G4ThreeVector pos)
{
  G4ThreeVector cargo_sphere3v = pos;
  cargo_sphere_position.push_back(cargo_sphere3v);
};

void SetCargoSphereMaterial(G4String m){cargo_sphere_material.push_back(m);};

void AddCargoBox(){cargo_boxes++;};

void SetCargoBoxSize(G4ThreeVector boxsize)
{
  G4ThreeVector cargo_box_size3v = boxsize;
  cargo_box_size.push_back(cargo_box_size3v);
};

void SetCargoBoxPosition(G4ThreeVector pos)
{
  G4ThreeVector cargo_box_position3v = pos;
  cargo_box_position.push_back(cargo_box_position3v);
};

void SetCargoBoxMaterial(G4String m){cargo_box_material.push_back(m);};

void SetIntObj_radius(G4double val)
{
        IntObj_rad = val;
        IntObj_rad = IntObj_rad*cm;
}
void SetIntObjAbundance(G4double val)
{
        intObj_radio_abundance = val;
}

void SetIntObj(G4String val)
{
  if(val == "Uranium")
  {
          IntObj_Selection = val;
          intObjDensity = 19.1*g/cm3;
  }
  else if(val == "Plutonium")
  {
          IntObj_Selection = val;
          intObjDensity = 19.6*g/cm3;
  }
  else if(val == "NaturalU")
  {
    IntObj_Selection = "Natural Uranium";
    intObjDensity = 19.1*g/cm3;
  }
  else if(val == "NaturalPu")
  {
    IntObj_Selection = "Natural Plutonium";
    intObjDensity = 19.6*g/cm3;
  }
  else if(val == "Lead")
  {
    IntObj_Selection = "Lead";
    intObjDensity = 11.4*g/cm3;
  }
  else if(val == "Steel")
  {
    IntObj_Selection = "Steel";
    intObjDensity = 8.05*g/cm3;
  }
  else if(val == "Plastic")
  {
    IntObj_Selection = "Plastic";
    intObjDensity = 0.94*g/cm3;
  }
  else{G4cerr << "ERROR: Cargo -> IntObj not correctly chosen" << G4endl;}
}

void SetRemoveContainer(G4bool val)
{
  RemoveContainer=val;
  G4cout << "Cargo:: WARNING Container has been removed!" << G4endl;
}

private:
  bool CheckCargoSphere(G4ThreeVector, double);
  bool CheckCargoBox(G4ThreeVector, G4ThreeVector);
  void CheckCargoBoxSize();
  void PlaceCargoBoxes();
  void PlaceCargoSpheres();


  G4bool RemoveContainer;
  G4double IntObj_rad, intObjDensity;
  G4String IntObj_Selection;
  G4double intObj_radio_abundance;
  G4double intObj_U235_abundance, intObj_U238_abundance, intObj_Pu239_abundance, intObj_Pu240_abundance;

  // Cargo SPHERE  and CARGO BOX
  G4LogicalVolume* logicContainer;
  G4LogicalVolume* logicHollowC;
  G4int cargo_spheres, cargo_boxes;
  std::vector<G4double> cargo_sphere_radii;
  std::vector<G4ThreeVector> cargo_sphere_position, cargo_box_size, cargo_box_position;
  std::vector<G4String> cargo_sphere_material, cargo_box_material;
  G4bool checkOverlaps;
  CargoMessenger* cargoM;

};

#endif
