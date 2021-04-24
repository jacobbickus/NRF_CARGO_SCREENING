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

DetWResponseFunction::DetWResponseFunction(ChopperSetup* Chopper, Collimator* Collimator, Cargo* Cargo)
: DetectorConstruction(),
chop(Chopper), collimator(Collimator), cargo(Cargo),
// Attenuator Properties
attenuatorState(false), attenuatorState2(false), attenThickness(0.001*mm), attenThickness2(0.001*mm), attenuatorMat("G4_AIR"), attenuatorMat2("G4_AIR"),
// Water Tank properties
theAngle(150.0), water_size_x(28*cm), water_size_y(225*cm), water_size_z(1*cm),
// plexi/tape properties
plexiThickness(0.18*mm),
// Output Properties
checkOverlaps(true)
{
  detectorM = new DetWResponseMessenger(this);
}

DetWResponseFunction::~DetWResponseFunction()
{
  delete detectorM;
}

G4VPhysicalVolume* DetWResponseFunction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // ************************************* Set up Materials ***************************************** //
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");

  // Materials For NIST Shielding
  G4Material *attenuator = nist->FindOrBuildMaterial(attenuatorMat);
  G4Material *low_z_attenuator = nist->FindOrBuildMaterial(attenuatorMat2);

  G4Element *elO = new G4Element("Oxygen", "O", 8, 16.0*g/mole);
  G4Element *elH = new G4Element("Hydrogen", "H", 1, 1.00794*g/mole);
  G4Material *plexiglass = nist->FindOrBuildMaterial("G4_PLEXIGLASS");

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
  G4double container_z_pos = 1.2192*m + 1.5*m - 135.9*cm - std::abs(bremStartPos)*cm;
  detInfo->setContainerZPosition(container_z_pos);
  G4double container_edge_position = container_z_pos - 1.2192*m;
  detInfo->setContainerEdgePosition(container_edge_position);
  G4double linac_size = 4.5*cm;
  detInfo->setLinac_Size(linac_size);
  detInfo->setWaterSizeY(water_size_y);

  chop->Construct(logicWorld, checkOverlaps);
  collimator->Construct(logicWorld, checkOverlaps);
  cargo->Construct(logicWorld, checkOverlaps);
  cargo->CheckCargoSphereSize();
  // ******************************************************** Begin Detector Construction *************************************************************** //

    // First Attenuation Layer
    // THIS IS THE MOTHER VOLUME INSIDE WORLD ALL OTHER DETECTOR VOLUMES ARE DAUGHTERS OF THIS VOLUME
    G4Box* solidAttenuator = new G4Box("Attenuator", water_size_x + attenThickness/2. + attenThickness2/2., water_size_y + attenThickness/2. + attenThickness2/2.,
                                       water_size_z + attenThickness/2. + attenThickness2/2.);
    G4LogicalVolume* logicAttenuator = new G4LogicalVolume(solidAttenuator, attenuator, "Attenuator");
    if(attenuatorState)
    {
      G4cout << G4endl << "DetWResponseFunction::Construct -> Attenuator Information" << G4endl;
      G4cout << "----------------------------------------------------------------------" << G4endl;
      G4cout << "DetWResponseFunction::Construct -> Attenuator Thickness set to: " << attenThickness << " mm of " << attenuator->GetName() << G4endl;
    }
    else
    {
      attenuatorState2 = false;
      attenThickness2 = 0.001*mm; // of air
      G4cout<< "DetWResponseFunction::Construct -> Second Attenuator State automatically set to Off." << G4endl;
    }

    G4double water_z_pos = detInfo->getRearCollimatorPosition();
    G4double myangle = (180. - theAngle)*pi/180.;
    G4double water_x_pos = tan(myangle)*(container_z_pos - water_z_pos);
    G4double detDistance = water_x_pos/sin(myangle) + water_size_z;
    G4RotationMatrix* waterRot = new G4RotationMatrix;
    G4RotationMatrix* waterRot2 = new G4RotationMatrix;

    waterRot->rotateY((180. - theAngle)*deg);
    waterRot2->rotateY((180. + theAngle)*deg);
    new G4PVPlacement(waterRot,
                      G4ThreeVector(water_x_pos,0,water_z_pos), logicAttenuator,
                      "Attenuator1Lay1L", logicWorld, false, 0, checkOverlaps);
    new G4PVPlacement(waterRot2,
                      G4ThreeVector(-1*water_x_pos,0,water_z_pos), logicAttenuator,
                      "Attenuator1Lay1R", logicWorld, false, 0, checkOverlaps);


  // Option to add second layer of low Z attenuation material

    G4Box* solidSecondAttenuator = new G4Box("LowZAttenuator", water_size_x + attenThickness2/2., water_size_y+attenThickness2/2.,
                                             water_size_z+attenThickness2/2.);
    G4LogicalVolume* logicSecondAttenuator = new G4LogicalVolume(solidSecondAttenuator, low_z_attenuator, "LowZAttenuator");
    new G4PVPlacement(0,G4ThreeVector(0,0,0), logicSecondAttenuator, "LowZAttenuator", logicAttenuator, false, 0, checkOverlaps);
    if(attenuatorState2)
    {
      G4cout << "DetWResponseFunction::Construct -> Second Attenuator set to: " << attenThickness2 << " mm of " << low_z_attenuator->GetName() << G4endl;
    }

  // Make Water Casing (Plexiglass)

    G4Box* solidCasing = new G4Box("Plexiglass", water_size_x, water_size_y, water_size_z);
    G4LogicalVolume* logicCasing = new G4LogicalVolume(solidCasing, plexiglass, "Plexiglass");
    new G4PVPlacement(0,G4ThreeVector(0,0,0), logicCasing, "Plexiglass", logicSecondAttenuator, false, 0, checkOverlaps);
    G4cout << G4endl << "DetWResponseFunction::Construct -> Water Tank Information" << G4endl;
    G4cout << "----------------------------------------------------------------------" << G4endl;
    if(plexiThickness != 0.18*mm)
    {
      G4cout << "DetWResponseFunction::Construct -> Plexiglass Thickness Changed to: "
      << plexiThickness << " mm" << G4endl;
    }
    else
    {
      G4cout << G4endl
      << "DetWResponseFunction::Construct -> Plexiglass Thickness set to default: "
      << plexiThickness << " mm" << G4endl;
    }

    return physWorld;
}
