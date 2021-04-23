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

#ifndef DetWResponseFunction_h
#define DetWResponseFunction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "ChopperSetup.hh"
#include "Collimator.hh"
#include "Cargo.hh"
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

#include "G4Box.hh" //for box
#include "G4Sphere.hh" //for sphere
#include "G4Tubs.hh" // for tube/cylinder

#include "DetWResponseMessenger.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"
#include "DetectorInformation.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class DetWResponseMessenger;

/// Detector construction class to define materials and geometry.

class DetWResponseFunction : public G4VUserDetectorConstruction
{
public:
  DetWResponseFunction(ChopperSetup*, Collimator*, Cargo*);
  virtual ~DetWResponseFunction();

  virtual G4VPhysicalVolume* Construct();

  void SetAttenuatorState(G4bool val){attenuatorState = val;}
  void SetAttenuatorThickness(G4double val){attenThickness = val;}
  void SetAttenuatorMaterial(G4String val){attenuatorMat = val;}
  void SetAttenuatorState2(G4bool val){attenuatorState2 = val;}
  void SetAttenuatorThickness2(G4double val){attenThickness2 = val;}
  void SetAttenuatorMaterial2(G4String val){attenuatorMat2 = val;}
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
  void SettheAngle(G4double val){theAngle = val;}
  void SetPlexiThickness(G4double val)
  {
    plexiThickness = val;
    plexiThickness = plexiThickness*mm;
  }
  void SetCheckOverlaps(G4bool val){checkOverlaps = val;}

private:
  ChopperSetup* chop;
  Collimator* collimator;
  Cargo* cargo;

  // Attenuator Properties
  G4bool attenuatorState, attenuatorState2;
  G4double attenThickness, attenThickness2;
  G4String attenuatorMat, attenuatorMat2;

  // Water Tank Properties
  G4double theAngle, water_size_x, water_size_y, water_size_z;
  G4double plexiThickness;

  G4bool checkOverlaps;

  // Messenger
  DetWResponseMessenger* detectorM;

};

#endif
