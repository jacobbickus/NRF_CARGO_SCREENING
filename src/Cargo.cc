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

#include "Cargo.hh"

Cargo::Cargo()
:
RemoveContainer(false),
IntObj_rad(4.5*cm), intObjDensity(19.1*g/cm3), IntObj_Selection("Uranium"),
intObj_radio_abundance(0), cargo_spheres(0), cargo_boxes(0), checkOverlaps(true),
cargoM(NULL)
{
  cargoM = new CargoMessenger(this);
}

Cargo::~Cargo()
{
  delete cargoM;
}

void Cargo::Construct(G4LogicalVolume* logicWorld, double container_z_pos, double container_edge_position, double chopper_end_edge_position, bool checkO)
{
  checkOverlaps = checkO;
  G4NistManager* nist = G4NistManager::Instance();
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material *steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material *poly = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material *lead = nist->FindOrBuildMaterial("G4_Pb");
  // Setting up weapons grade material Isotopes
  G4Isotope* Uranium235 = new G4Isotope("Uranium235", 92, 235, 235.04393*g/mole); // atomicnumber, number of nucleons, mass of mole
  G4Isotope* Uranium238 = new G4Isotope("Uranium238", 92, 238, 238.02891*g/mole);
  G4Isotope* Plutonium239 = new G4Isotope("Plutonium239",94, 239, 239.0521634*g/mole);
  G4Isotope* Plutonium240 = new G4Isotope("Plutonium240", 94, 240, 240.05381*g/mole);

  // Setting up Interrogation object materials
  G4Element* Uranium_interrogation = new G4Element("Interrogation_Uranium","U",2);
  G4Element* Plutonium_interrogation = new G4Element("Interrogation_Plutonium","Pu",2);
  G4Material* natural_uranium = nist->FindOrBuildMaterial("G4_U");
  G4Material* natural_plutonium = nist->FindOrBuildMaterial("G4_Pu");

  // Set up shipping container environment (8ft wide and 8.5ft high)
  G4double c_thick = 0.1905*cm; // approx 0.075 inch thick
  G4Box *solidContainer = new G4Box("Container", 0.3048*m, 1.2954*m, 1.2192*m);
  G4cout << "Cargo::Construct -> Edge of Container Placement: " << container_edge_position/(cm) << " cm" << G4endl << G4endl;
  logicContainer = new G4LogicalVolume(solidContainer, steel, "Container");

  if(!RemoveContainer)
  {
    G4cout << "Cargo::Construct -> Building Container!" << G4endl;
    new G4PVPlacement(0,
                    G4ThreeVector(0, 0, container_z_pos),
                    logicContainer, "Con-Steel",logicWorld,
                    false,0,checkOverlaps);
  }

  // make container hollow
  G4Box *hollowContainer = new G4Box("ContainerAir", 0.3048*m -c_thick, 1.2954*m -c_thick, 1.2192*m -c_thick);
  logicHollowC = new G4LogicalVolume(hollowContainer, air, "hollowContainer");

  if(!RemoveContainer)
  {
    G4cout << "Cargo::Construct -> Container Built!" << G4endl;
    new G4PVPlacement(0, G4ThreeVector(),
                      logicHollowC, "Con-Air",logicContainer, false,
                      0,checkOverlaps);
  }

  // Set Up the Interrogation Object
    G4Sphere* solidIntObj = new G4Sphere("InterogationObject", 0, IntObj_rad, 0, 2*pi, 0, pi);

    intObj_U235_abundance = intObj_radio_abundance;
    intObj_U238_abundance = 100. - intObj_radio_abundance;
    intObj_Pu239_abundance = intObj_radio_abundance;
    intObj_Pu240_abundance = 100. - intObj_radio_abundance;
    G4Material* intObjMat = new G4Material("IntObj", intObjDensity, 1);
    intObjMat->SetName(IntObj_Selection);
    G4cout << G4endl << "Cargo::Construct -> Interrogation Object Information" << G4endl;
    G4cout << "----------------------------------------------------------------------" << G4endl;
    if(IntObj_Selection == "Uranium")
    {
      if(intObj_radio_abundance <= 0.0)
      {
        G4cerr << "Fatal Error: Cargo::Construct -> User must input interrogation object isotope abundance for Uranium 235" << G4endl;
        exit(100);
      }
      Uranium_interrogation->AddIsotope(Uranium235, intObj_U235_abundance*perCent);
      Uranium_interrogation->AddIsotope(Uranium238, intObj_U238_abundance*perCent);
      intObjMat->AddElement(Uranium_interrogation,1);
    }
    else if(IntObj_Selection == "Plutonium")
    {
      if(intObj_radio_abundance <= 0.0)
      {
        G4cerr << "Fatal Error: Cargo::Construct -> User must input interrogation object isotope abundance for Plutonium 239" << G4endl;
        exit(100);
      }
      Plutonium_interrogation->AddIsotope(Plutonium239, intObj_Pu239_abundance*perCent);
      Plutonium_interrogation->AddIsotope(Plutonium240, intObj_Pu240_abundance*perCent);
      intObjMat->AddElement(Plutonium_interrogation,1);
    }
    else if(IntObj_Selection == "Natural Uranium")
    {
            intObjMat->AddMaterial(natural_uranium,1);
    }
    else if(IntObj_Selection == "Natural Plutonium")
    {
            intObjMat->AddMaterial(natural_plutonium,1);
    }
    else if(IntObj_Selection == "Lead")
    {
            intObjMat->AddMaterial(lead,1);
    }
    else if(IntObj_Selection == "Steel")
    {
            intObjMat->AddMaterial(steel,1);
    }
    else if(IntObj_Selection == "Plastic")
    {
            intObjMat->AddMaterial(poly,1);
    }
    else{G4cerr << "ERROR: Cargo::Construct -> Interogation Material not found."<<G4endl;}

    G4cout << "Cargo::Construct -> Interogation Object Material: "
           << intObjMat->GetName() << G4endl;
    if(IntObj_Selection == "Uranium" || IntObj_Selection == "Plutonium")
    {
            G4cout << "Cargo::Construct -> Interrogation Object Abundance: " << intObj_radio_abundance << " %" << G4endl;
    }
    G4cout << "Cargo::Construct -> Interrogation Object Radius: " << IntObj_rad/(cm) << " cm" << G4endl;
    G4cout << "Cargo::Construct -> Interrogation Object Density: " << intObjDensity/(g/cm3) << " g/cm3" << G4endl;

    G4LogicalVolume* logicIntObj = new G4LogicalVolume(solidIntObj, intObjMat,"IntObj");
    G4cout << "Cargo::Construct -> Begin of Interrogation Object: " << container_z_pos/(cm) + 0/(cm) -  IntObj_rad/(cm) << " cm" << G4endl;
    DetectorInformation* detInfo = DetectorInformation::Instance();
    detInfo->setEndIntObj(container_z_pos, 1.2192*m);

    if(!RemoveContainer)
    {
      new G4PVPlacement(0,
                         G4ThreeVector(0, 0, 0),
                         logicIntObj, "IntObj", logicHollowC, false,
                         0, checkOverlaps);
    }
    else
    {
      new G4PVPlacement(0,
                        G4ThreeVector(0,0,chopper_end_edge_position+1.0*m),
                        logicIntObj, "IntObj",logicWorld, false, 0, checkOverlaps);
    }
    CheckCargoBoxSize();
    PlaceCargoSpheres();
    PlaceCargoBoxes();
}

void Cargo::PlaceCargoSpheres()
{
  if(cargo_spheres == 0)
    return;

  G4cout << "Cargo::PlaceCargoSpheres -> Adding " << cargo_spheres << " spheres to cargo container." << G4endl;

  for(int i=0;i<cargo_spheres;++i)
  {
    G4double sphere_radius = cargo_sphere_radii[i]*cm;
    G4ThreeVector sphere_position = cargo_sphere_position[i]*cm;
    G4String sphere_material = cargo_sphere_material[i];
    G4String sphere_name = sphere_material + "_" + std::to_string(i);
    G4NistManager* nist = G4NistManager::Instance();
    G4Material *sphereMAT = nist->FindOrBuildMaterial(sphere_material);
    bool check = CheckCargoSphere(sphere_position/(m), sphere_radius/(m));
    if(!check)
    {
      G4cerr << "Cargo::PlaceCargoSpheres -> ERROR Cannot Place Cargo Outside of Container." << G4endl;
      exit(1);
    }
    G4Sphere* CargoSphere = new G4Sphere("CargoSphere",0, sphere_radius, 0, 2*pi, 0, pi);
    G4LogicalVolume* CargoSphereLogical = new G4LogicalVolume(CargoSphere, sphereMAT, "CargoSphereLogical");
    new G4PVPlacement(0, sphere_position, CargoSphereLogical, sphere_name.c_str(), logicHollowC, false, 0, checkOverlaps);

  }
}

void Cargo::PlaceCargoBoxes()
{
  if(cargo_boxes == 0)
    return;

  G4cout << "Cargo::PlaceCargoBoxes -> Adding " << cargo_boxes << " boxes to cargo container." << G4endl;
  for(int i=0;i<cargo_boxes;++i)
  {
    G4ThreeVector box_size = cargo_box_size[i]*cm;
    G4ThreeVector box_position = cargo_box_position[i]*cm;
    G4String box_material = cargo_box_material[i];
    G4String box_name = box_material + "_" + std::to_string(i);
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* boxMAT = nist->FindOrBuildMaterial(box_material);
    bool check = CheckCargoBox(box_position/(m), box_size/(m));
    if(!check)
    {
      G4cerr << "Cargo::PlaceCargoBoxes -> ERROR Cannot Place Cargo Outside of Container." << G4endl;
      exit(1);
    }
    G4double x = box_size.x();
    G4double y = box_size.y();
    G4double z = box_size.z();
    G4Box* CargoBox = new G4Box("CargoBox", x, y, z);
    G4LogicalVolume* CargoBoxLogical = new G4LogicalVolume(CargoBox, boxMAT, "CargoBoxLogical");
    new G4PVPlacement(0,box_position, CargoBoxLogical, box_name.c_str(), logicHollowC, false, 0, checkOverlaps);
  }
}

void Cargo::CheckCargoBoxSize()
{
  if(cargo_box_size.size() != cargo_box_position.size() || cargo_box_size.size() != cargo_box_material.size())
  {
    G4cerr << "Cargo::CheckCargoBoxSize -> ERROR Missing Box Information!" << G4endl;
    G4cerr << "Cargo::CheckCargoBoxSize -> Box Sizes Size: " << cargo_box_size.size() << G4endl;
    G4cerr << "Cargo::CheckCargoBoxSize -> Box Positions Size: " << cargo_box_position.size() << G4endl;
    G4cerr << "Cargo::CheckCargoBoxSize -> Box Materials Size: " << cargo_box_material.size() << G4endl;
    exit(1);
  }
}

void Cargo::CheckCargoSphereSize()
{
  if(cargo_sphere_radii.size() != cargo_sphere_position.size() || cargo_sphere_radii.size() != cargo_sphere_material.size())
  {
    G4cerr << "Cargo::CheckCargoSphereSize -> ERROR Missing Sphere Information!" << G4endl;
    G4cerr << "Cargo::CheckCargoSphereSize -> Sphere Radii Size: " << cargo_sphere_radii.size() << G4endl;
    G4cerr << "Cargo::CheckCargoSphereSize -> Sphere Positions Size: " << cargo_sphere_position.size() << G4endl;
    G4cerr << "Cargo::CheckCargoSphereSize -> Sphere Materials Size: " << cargo_sphere_material.size() << G4endl;
    exit(1);
  }
}

bool Cargo::CheckCargoSphere(G4ThreeVector sphere_position, G4double sphere_radius)
{
  if(std::abs(sphere_position.x()) + std::abs(sphere_radius)*2. > 0.3048)
  {
    G4cerr << "Cargo::CheckCargoSphere -> ERROR Container X Dimension Breech." << G4endl;
    G4cerr << "Cargo::CheckCargoSphere -> " << std::abs(sphere_position.x()) + std::abs(sphere_radius)*2. << G4endl;
    return false;
  }
  if(std::abs(sphere_position.y()) + std::abs(sphere_radius)*2. > 1.2954)
  {
    G4cerr << "Cargo::CheckCargoSphere -> ERROR Container Y Dimension Breech." << G4endl;
    G4cerr << "Cargo::CheckCargoSphere -> " << std::abs(sphere_position.y()) + std::abs(sphere_radius)*2. << G4endl;
    return false;
  }
  if(std::abs(sphere_position.z()) + std::abs(sphere_radius)*2. > 1.2192)
  {
    G4cerr << "Cargo::CheckCargoSphere -> ERROR Container Z Dimension Breech." << G4endl;
    G4cerr << "Cargo::CheckCargoSphere -> " << std::abs(sphere_position.z()) + std::abs(sphere_radius)*2. << G4endl;
    return false;
  }
  return true;
}

bool Cargo::CheckCargoBox(G4ThreeVector box_position, G4ThreeVector box_size)
{
  if(std::abs(box_position.x())+std::abs(box_size.x()) > 0.3048)
  {
    G4cerr << "Cargo::CheckCargoBox -> ERROR Container X Dimension Breeched." << G4endl;
    G4cerr << "Cargo::CheckCargoBox -> " << std::abs(box_position.x())+std::abs(box_size.x()) << G4endl;
    return false;
  }
  if(std::abs(box_position.y())+std::abs(box_size.y()) > 1.2954)
  {
    G4cerr << "Cargo::CheckCargoBox -> ERROR Container Y Dimension Breeched." << G4endl;
    G4cerr << "Cargo::CheckCargoBox -> " << std::abs(box_position.y())+std::abs(box_size.y()) << G4endl;
    return false;
  }
  if(std::abs(box_position.z())+std::abs(box_size.z()) > 1.2192)
  {
    G4cerr << "Cargo::CheckCargoBox -> ERROR Container Z Dimension Breeched." << G4endl;
    G4cerr << "Cargo::CheckCargoBox -> " << std::abs(box_position.z())+std::abs(box_size.z()) << G4endl;
    return false;
  }
  return true;
}
