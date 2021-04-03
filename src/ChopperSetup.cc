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

#include "ChopperSetup.hh"

ChopperSetup::ChopperSetup()
:chopperDensity(19.1*g/cm3), chopper_thick(30*mm), chopper_z(2*cm),
chopperOn(false), chopper_radio_abundance(0), chopperM(NULL)
{
  chopperM = new ChopperMessenger(this);
}

ChopperSetup::~ChopperSetup()
{
  delete chopperM;
}

G4VPhysicalVolume* ChopperSetup::Construct(G4LogicalVolume* logicWorld, double bremStartPos, double linac_size, double container_edge_position, bool checkOverlaps)
{
  // Setting up Chopper Isotopes
  G4Isotope* Lead204 = new G4Isotope("Lead204", 82, 204, 203.973043*g/mole);
  G4Isotope* Lead206 = new G4Isotope("Lead206", 82, 206, 205.974465*g/mole);
  G4Isotope* Lead207 = new G4Isotope("Lead207", 82, 207, 206.975897*g/mole);
  G4Isotope* Lead208 = new G4Isotope("Lead208", 82, 208, 207.976652*g/mole);

  G4Isotope* Tungsten180 = new G4Isotope("Tung180", 74, 180, 179.9467*g/mole);
  G4Isotope* Tungsten182 = new G4Isotope("Tung182", 74, 182, 181.9482*g/mole);
  G4Isotope* Tungsten183 = new G4Isotope("Tung183", 74, 183, 182.9502*g/mole);
  G4Isotope* Tungsten184 = new G4Isotope("Tung182", 74, 184, 183.9509*g/mole);
  G4Isotope* Tungsten186 = new G4Isotope("Tung186", 74, 186, 185.9543*g/mole);

  // Setting up weapons grade material Isotopes
  G4Isotope* Uranium235 = new G4Isotope("Uranium235", 92, 235, 235.04393*g/mole); // atomicnumber, number of nucleons, mass of mole
  G4Isotope* Uranium238 = new G4Isotope("Uranium238", 92, 238, 238.02891*g/mole);
  G4Isotope* Plutonium239 = new G4Isotope("Plutonium239",94, 239, 239.0521634*g/mole);
  G4Isotope* Plutonium240 = new G4Isotope("Plutonium240", 94, 240, 240.05381*g/mole);

  // Setting up Chopper Materials
  G4Element* Uranium_chopper = new G4Element("Chopper_Uranium", "U", 2); // name, element symbol, #isotopes
  G4Element* Plutonium_chopper = new G4Element("Chopper_Plutonium","Pu",2);
  G4Element* Lead_chopper = new G4Element("Chopper_Lead","Pb",4);
  Lead_chopper->AddIsotope(Lead204, 1.4*perCent);
  Lead_chopper->AddIsotope(Lead206, 24.1*perCent);
  Lead_chopper->AddIsotope(Lead207, 22.1*perCent);
  Lead_chopper->AddIsotope(Lead208, 52.4*perCent);
  G4Element* Tungsten_chopper = new G4Element("Chopper_Tungsten","W",5);
  Tungsten_chopper->AddIsotope(Tungsten180, 0.000012*perCent);
  Tungsten_chopper->AddIsotope(Tungsten182, 26.50*perCent);
  Tungsten_chopper->AddIsotope(Tungsten183, 14.31*perCent);
  Tungsten_chopper->AddIsotope(Tungsten184, 30.759988*perCent);
  Tungsten_chopper->AddIsotope(Tungsten186, 28.43*perCent);


  // *********************************************************** Set up Chopper Wheel ****************************************************************** //

  G4cout << G4endl << "ChopperSetup::Construct -> Information" << G4endl;
  G4cout << "----------------------------------------------------------------------" << G4endl;
  G4double chopper_beginning_edge_position = (bremStartPos+ chopper_z + linac_size) - chopper_thick/2.;
  G4double chopper_end_edge_position = (bremStartPos + chopper_z + linac_size) + chopper_thick/2.;
  G4double chopper_center_position = (chopper_beginning_edge_position + chopper_end_edge_position)/2.;
  DetectorInformation* detInfo = DetectorInformation::Instance();
  detInfo->setBeginChopper(chopper_beginning_edge_position);
  G4cout << "ChopperSetup::Construct -> Beginning Edge: "
          << chopper_beginning_edge_position/(cm) << " cm" << G4endl;
  G4cout << "ChopperSetup::Construct -> Chopper Center: "
          << chopper_center_position/(cm) << " cm" << G4endl;
  G4cout << "ChopperSetup::Construct -> End Edge: "
          << chopper_end_edge_position/(cm) << " cm" << G4endl;
  detInfo->setEndChop(chopper_end_edge_position);

  if(chopper_end_edge_position > container_edge_position)
  {
    G4cerr << "ChopperSetup::Construct -> ERROR: Chopper wheel location should be behind cargo container, exiting." << G4endl;
    G4cerr << "ChopperSetup::Construct -> Chopper End Edge Position -> "
            << chopper_end_edge_position/(cm) << " cm" << G4endl;
    G4cerr << "ChopperSetup::Construct -> Container Edge Position -> "
            << container_edge_position/(cm) << " cm" << G4endl;
    exit(100);
  }

  G4Tubs *solidChopper = new G4Tubs("Chop", 0*cm, 15*cm, chopper_thick/2, 0.*deg, 180.*deg);
  G4Material *chopperMat = new G4Material("chopperMaterial", chopperDensity, 1);
  G4cout << "ChopperSetup::Construct -> State: " << chopperOn << G4endl;

  if(chopperDensity == 19.1*g/cm3)
  {
    if(chopper_radio_abundance <= 0.0)
    {
      G4cerr << "Fatal Error: User Must input chopper isotope abundance as percentage > 0" << G4endl;
      exit(100);
    }

    if(chopperOn)
    {
            chopper_U235_abundance = chopper_radio_abundance;
            chopper_U238_abundance = 100. - chopper_radio_abundance;
    }
    else
    {
            chopper_U235_abundance = 100. - chopper_radio_abundance;
            chopper_U238_abundance = chopper_radio_abundance;
    }

    Uranium_chopper->AddIsotope(Uranium235, chopper_U235_abundance*perCent);
    Uranium_chopper->AddIsotope(Uranium238, chopper_U238_abundance*perCent);
    chopperMat->AddElement(Uranium_chopper,1);
    G4cout << "ChopperSetup::Construct -> Material: Uranium" << G4endl;
    G4cout << "ChopperSetup::Construct -> Fission isotope abundance: "
            << chopper_radio_abundance << " %" << G4endl;
  }
  else if(chopperDensity == 19.6*g/cm3)
  {
    if(chopper_radio_abundance <= 0.0)
    {
            G4cerr << "Fatal Error: User Must input chopper isotope abundance as percentage > 0" << G4endl;
            exit(100);
    }

    if(chopperOn)
    {
            chopper_Pu239_abundance = chopper_radio_abundance;
            chopper_Pu240_abundance = 100. - chopper_radio_abundance;
    }
    else
    {
            chopper_Pu239_abundance = 100. - chopper_radio_abundance;
            chopper_Pu240_abundance = chopper_radio_abundance;
    }

    Plutonium_chopper->AddIsotope(Plutonium239, chopper_Pu239_abundance*perCent);
    Plutonium_chopper->AddIsotope(Plutonium240, chopper_Pu240_abundance*perCent);
    chopperMat->AddElement(Plutonium_chopper, 1);
    G4cout << "ChopperSetup::Construct -> Material: Plutonium" << G4endl;
    G4cout << "ChopperSetup::Construct -> Fission isotope abundance: "
            << chopper_radio_abundance << " %" << G4endl;
  }
  else if(chopperDensity == 11.34*g/cm3)
  {
          chopperMat->AddElement(Lead_chopper,1);
          G4cout << "ChopperSetup::Construct -> Material: Lead" << G4endl;
  }
  else if(chopperDensity == 19.3*g/cm3)
  {
          chopperMat->AddElement(Tungsten_chopper,1);
          G4cout << "ChopperSetup::Construct -> Material: Tungsten" << G4endl;
  }
  else{G4cerr << "ERROR Chopper Density not found!" << G4endl; exit(100);}

  G4cout << "ChopperSetup::Construct -> Material density: " << chopperDensity/(g/cm3) << " g/cm3" << G4endl;


  G4cout << "ChopperSetup::Construct -> Thickness: " << chopper_thick/(mm)
          << " mm" << G4endl;

  SourceInformation* sInfo = SourceInformation::Instance();
  G4double source_z_pos = sInfo->GetSourceZPosition();

  G4double center_from_source = chopper_center_position/(cm) - source_z_pos;
  G4cout << "ChopperSetup::Construct -> Center distance from the source: "
          << center_from_source << " cm" << G4endl;

  G4LogicalVolume* logicChopper = new G4LogicalVolume(solidChopper, chopperMat, "Chop");

  G4VPhysicalVolume* chopper = new G4PVPlacement(0, G4ThreeVector(0, -7*cm,bremStartPos + chopper_z + linac_size),
                  logicChopper, "Chop", logicWorld, false,
                  0, checkOverlaps);

  return chopper;
}
