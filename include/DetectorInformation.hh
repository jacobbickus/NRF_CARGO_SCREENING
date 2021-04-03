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

#ifndef DetectorInformation_h
#define DetectorInformation_h 1

#include "globals.hh"
#include "G4Types.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

class DetectorInformation
{
  static DetectorInformation *instance;

  DetectorInformation();
public:
  static DetectorInformation *Instance()
  {
    if(!instance)
    {
      instance = new DetectorInformation;
    }
    return instance;
  }

  // Set Data Functions here
  void setBeginChopper(G4double pos){BeginChop = pos;}
  G4double getBeginChopper()const{return BeginChop;}
  void setEndChop(G4double z_pos){EndChop = z_pos;}
  G4double getEndChop()const{return EndChop;}
  void setEndIntObj(G4double z_pos_con, G4double con_z_size)
  {
    EndIntObj = z_pos_con + con_z_size/2;
    G4cout << "DetectorInformation::Z-Cut set to: " << EndIntObj/(cm) << " cm" << G4endl << G4endl;
  }
  G4double getEndIntObj()const{return EndIntObj;}
  void setRearCollimatorPosition(G4double val){RearColPos = val;}
  G4double getRearCollimatorPosition(void){return RearColPos;}

  G4double BeginChop, EndChop, EndIntObj, RearColPos;
  ~DetectorInformation();
};

#endif
