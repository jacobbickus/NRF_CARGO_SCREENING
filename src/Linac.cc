#include "Linac.hh"

Linac::Linac()
{}
Linac::~Linac()
{}

void Linac::Construct(G4LogicalVolume* logicWorld, bool checkOverlaps)
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material *tungsten = nist->FindOrBuildMaterial("G4_W");
  G4Material *copper = nist->FindOrBuildMaterial("G4_Cu");
  G4Material *gold = nist->FindOrBuildMaterial("G4_Au");
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material *myVacuum = new G4Material("Vacuum", 1.e-5*g/cm3, 1, kStateGas, 273.15, 2.e-2*bar);
  myVacuum->AddMaterial(air,1);
  DetectorInformation* detInfo = DetectorInformation::Instance();
  G4double bremBacking_thickness = 5.0*mm;

  // Linac
  G4double linac_radius = 3.5*cm;
  detInfo->setLinac_MaxRadius(linac_radius);
  G4double linac_size = detInfo->GetLinacSize();
  G4double bremStartPos = detInfo->GetBremStartPosition();

  G4Tubs *solidLinac = new G4Tubs("Linac",0, linac_radius, linac_size, 0*deg, 360*deg);
  G4LogicalVolume* logicalLinac = new G4LogicalVolume(solidLinac, tungsten, "Linac");
  new G4PVPlacement(0, G4ThreeVector(0,0, bremStartPos), logicalLinac, "Linac", logicWorld, false, 0, checkOverlaps);
  detInfo->setEndLinac(bremStartPos+linac_size/2.);
  detInfo->setLinac_MinRadius(linac_radius - 1*cm);
  // Vacuum
  G4Tubs *solidVacuum = new G4Tubs("Vacuum", 0, linac_radius - 2*cm, linac_size, 0*deg, 360*deg);
  G4LogicalVolume* logicalVacuum = new G4LogicalVolume(solidVacuum, myVacuum, "Vacuum");
  new G4PVPlacement(0, G4ThreeVector(0,0,0), logicalVacuum, "Vac", logicalLinac, false,0,checkOverlaps);

// Make Brem target

// Brem Backing
  G4double bremBacking_xy = 1.5*mm;
  G4Box *solidBremTargetBacking = new G4Box("BremBacking", bremBacking_xy, bremBacking_xy, bremBacking_thickness/2.0);
  G4LogicalVolume* logicBremTargetBacking = new G4LogicalVolume(solidBremTargetBacking, copper, "BremBacking");
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicBremTargetBacking, "BremBacking", logicalVacuum, false, 0, checkOverlaps);

// Brem Radiator
  G4double bremTarget_thickness = 0.102*mm;
  G4double brem_target_position = -(bremBacking_thickness/2.0) + bremTarget_thickness/2.0;
  G4Tubs *solidBremTarget = new G4Tubs("Brem", 0.*mm, 1.0*mm, bremTarget_thickness/2.0,0.*degree,360.*degree);
  G4LogicalVolume* logicBremTarget = new G4LogicalVolume(solidBremTarget, gold, "Brem");
  new G4PVPlacement(0, G4ThreeVector(0, 0, brem_target_position),logicBremTarget,"Brem", logicBremTargetBacking, false, 0, checkOverlaps);
  G4double brem_target_edge_position = bremStartPos + brem_target_position + bremTarget_thickness/2.0;
  G4cout << "Linac::Construct -> Brem Target Beginning Edge Position: " << brem_target_edge_position/(cm) << " cm" << G4endl << G4endl;
  detInfo->setBremTargetBeginPosition(brem_target_edge_position);
}
