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

#include "DetWResponseFunction.hh"

extern G4bool debug;
extern G4bool run_without_chopper;

DetWResponseFunction::DetWResponseFunction(ChopperSetup* Chopper, Collimator* Collimator, Cargo* Cargo)
: DetectorConstruction(Chopper, Collimator, Cargo)
{
}

DetWResponseFunction::~DetWResponseFunction()
{}

G4VPhysicalVolume* DetWResponseFunction::Construct()
{
  // Materials For NIST Shielding
  G4Material *attenuator, *attenuator2, *attenuator3;
  ConstructAttenuatorMaterial(attenuator, attenuator2, attenuator3);

  G4double world_size_x = 2.5*m;
  G4double world_size_y = 2.5*m;
  G4double world_size_z = 2.5*m;

  G4Box* solidWorld = new G4Box("World", world_size_x, world_size_y, world_size_z);

  G4LogicalVolume* logicWorld =
          new G4LogicalVolume(solidWorld, //its solid
                              air, //its material
                              "World"); //its name

  // Make Physical volume ** NEVER CHANGE THIS **
  G4VPhysicalVolume* physWorld =
          new G4PVPlacement(0, //no rotation
                            G4ThreeVector(), //at (0,0,0)
                            logicWorld, //its logical volume
                            "World", //its name
                            0, //its mother  volume
                            false, //no boolean operation
                            0, //copy number
                            false); //overlaps checking

  DetectorInformation* detInfo = DetectorInformation::Instance();
  G4double bremStartPos = detInfo->GetShiftFactor();
  detInfo->setBremStartPosition(bremStartPos);
  G4double container_z_pos = 0.;

  if(!run_without_chopper)
    container_z_pos = 1.2192*m + 1.5*m - 135.9*cm - std::abs(bremStartPos)*cm;

  detInfo->setContainerZPosition(container_z_pos);
  G4double container_edge_position = container_z_pos - 1.2192*m;
  detInfo->setContainerEdgePosition(container_edge_position);
  G4double linac_size = 4.5*cm;
  detInfo->setLinac_Size(linac_size);
  detInfo->setWaterSizeY(water_size_y);

  if(!run_without_chopper)
  {
    chop->Construct(logicWorld, checkOverlaps);
    collimator->Construct(logicWorld, checkOverlaps);
  }
  else
    detInfo->setRearCollimatorPosition(container_edge_position - 50.*cm);

  cargo->Construct(logicWorld, checkOverlaps);
  cargo->CheckCargoSphereSize();
  // ******************************************************** Begin Detector Construction *************************************************************** //

  G4LogicalVolume* logicCasing = ConstructAttenuatorsAndPlexiglass(logicWorld, container_z_pos, attenuator, attenuator2, attenuator3);

  return physWorld;
}
