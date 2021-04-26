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

#include "DetBremTest.hh"

extern G4bool debug;

DetBremTest::DetBremTest(ChopperSetup* Chopper, Linac* Linac, Collimator* Collimator)
: DetectorConstruction(Chopper, Collimator, 0, Linac)
{}

DetBremTest::~DetBremTest()
{}

G4VPhysicalVolume* DetBremTest::Construct()
{

  G4double world_size_x = .15*m;
  G4double world_size_y = .15*m;
  G4double world_size_z = .15*m;

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
  G4double linac_size = 4.5*cm;
  detInfo->setLinac_Size(linac_size);
  chop->Construct(logicWorld, checkOverlaps);
  linac->Construct(logicWorld, checkOverlaps);
  collimator->Construct(logicWorld, checkOverlaps);

  //always return the physical World!!!
  if(debug)
    G4cout << "DetBremTest::Construct -> Constructed!" << G4endl << G4endl;

  return physWorld;

}
