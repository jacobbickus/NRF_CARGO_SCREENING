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

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "ChopperSetup.hh"
#include "Linac.hh"
#include "Collimator.hh"
#include "Cargo.hh"
#include "MaterialProperties.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4OpRayleigh.hh"

#include "G4Box.hh" //for box
#include "G4Sphere.hh" //for sphere
#include "G4Tubs.hh" // for tube/cylinder

#include "DetectorMessenger.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"
#include "DetectorInformation.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class DetectorMessenger;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{

public:
DetectorConstruction(ChopperSetup*, Linac*, Collimator*, Cargo*);
virtual ~DetectorConstruction();


virtual G4VPhysicalVolume* Construct();

void SetAttenuatorState(G4bool val)
{
  attenuatorState = val;
}
void SetAttenuatorThickness(G4double val)
{
   attenThickness = val;
}
void SetAttenuatorMaterial(G4String val)
{
  attenuatorMat = val;
}
void SetAttenuatorState2(G4bool val)
{
  attenuatorState2 = val;
}
void SetAttenuatorThickness2(G4double val)
{
   attenThickness2 = val;
}
void SetAttenuatorMaterial2(G4String val)
{
  attenuatorMat2 = val;
}
void SetWaterX(G4double val)
{
        water_size_x = val;
        water_size_x = water_size_x*cm;
}
void SetWaterY(G4double val)
{
        water_size_y = val;
        water_size_y = water_size_y*cm;
}
void SetWaterZ(G4double val)
{
        water_size_z = val;
        water_size_z = water_size_z*cm;
}
void SetPC_radius(G4double val)
{
        PMT_rmax = val;
        PMT_rmax = PMT_rmax*cm;
};

void SetPC_material(G4String val){pc_mat = val;}
void SetnPMT(G4int val){nPMT = val;}

void SettheAngle(G4double val){theAngle = val;}
void SetPlexiThickness(G4double val)
{
  plexiThickness = val;
  plexiThickness = plexiThickness*mm;
}
void SetTapeThickness(G4double val)
{
  tapeThick = val;
  tapeThick = tapeThick*cm;
}
void SetDetectorViewOnly(G4bool val){DetectorViewOnly = val;}
void SetMaterialVerbosity(G4bool val){material_verbose = val;}
void SetCheckOverlaps(G4bool val){checkOverlaps = val;}

private:
// Private Member Functions
void DefDetPositionConstraintLeft(double, double, double, double);
void DefDetPositionConstraintRight(double, double, double, double);
void DefDetPositionConstraintUpper(double, double, double);
// Chopper
ChopperSetup* chop;
Linac* linac;
Collimator* collimator;
Cargo* cargo;
// Brem Properties
G4double linac_size = 9*cm;

// Material, Logical and Physical Volumes
G4Material* PC_mat;

G4LogicalVolume* logicPC;
G4LogicalVolume* logicPMT;
G4VPhysicalVolume* physPC;
G4VPhysicalVolume* physWater;
G4VPhysicalVolume* physTape;

// Detector Properties

// Attenuator Properties
G4bool attenuatorState, attenuatorState2;
G4double attenThickness, attenThickness2;
G4String attenuatorMat, attenuatorMat2;

// Water Tank Properties
G4double theAngle, water_size_x, water_size_y, water_size_z;

// Plexi/Tape Properties
G4double plexiThickness, tapeThick;

// PMT Properties
G4double PMT_rmax;
G4int nPMT;
G4String pc_mat;

// Output Properties
G4bool DetectorViewOnly, material_verbose, checkOverlaps;

// Messenger
DetectorMessenger* detectorM;

};


#endif
