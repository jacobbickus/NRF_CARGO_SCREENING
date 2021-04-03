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

#include "Collimator.hh"

Collimator::Collimator()
{;}
Collimator::~Collimator()
{;}

void Collimator::Construct(G4LogicalVolume* logicWorld, double bremStartPos, double linac_size, double container_z_pos, double water_size_y, bool checkOverlaps)
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material *lead = nist->FindOrBuildMaterial("G4_Pb");
  G4double colimator_size = 50*cm;
  G4double col_position = 1.0*cm + container_z_pos - 1.2192*m - colimator_size; // should go 1cm past the container
  G4double col_edge_position = col_position + colimator_size;
  G4double rearCol_Z_pos = bremStartPos - linac_size - 50.0*cm;
  DetectorInformation* detInfo = DetectorInformation::Instance();
  detInfo->setRearCollimatorPosition(rearCol_Z_pos);

  G4Box *solidCollimator =
                        new G4Box("Collimator", 1*cm, water_size_y, colimator_size);
  G4Box *solidCollimatorRear =
                        new G4Box("Collimator",0.3048*m - 2*cm, water_size_y, 5*cm);
  G4LogicalVolume *logicCollimator =
                        new G4LogicalVolume(solidCollimator, lead, "Collimator");
  G4LogicalVolume *logicCollimatorRear =
                        new G4LogicalVolume(solidCollimatorRear, lead, "Collimator");
  G4cout << G4endl << "Collimator::Construct -> Information" << G4endl;
  G4cout << "----------------------------------------------------------------------"
  << G4endl;
  G4cout << "Collimator::Construct -> Edge of Colimator placement: "
  << col_edge_position/(cm) << " cm" << G4endl << G4endl;

  new G4PVPlacement(0, G4ThreeVector(-0.3048*m - 1*cm, 0, col_position),
                    logicCollimator, "ColL-Pb", logicWorld,
                    false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0.3048*m + 1*cm, 0, col_position),
                    logicCollimator, "ColRi-Pb", logicWorld,
                    false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0,rearCol_Z_pos),
                    logicCollimatorRear, "ColRe-Pb", logicWorld,
                    false, 0, checkOverlaps);
}
