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
  void setBremStartPosition(G4double pos){bremStartPos = pos;}
  G4double GetBremStartPosition()const{return bremStartPos;}
  void setLinac_Size(G4double theSize){linac_size = theSize;}
  G4double GetLinacSize()const{return linac_size;}
  void setContainerZPosition(G4double pos){container_z_pos = pos;}
  G4double GetContainerZPosition()const{return container_z_pos;}
  void setContainerEdgePosition(G4double pos){container_edge_position = pos;}
  G4double GetContainerEdgePosition()const{return container_edge_position;}
  void setWaterSizeY(G4double y){water_size_y = y;}
  G4double GetWaterSizeY()const{return water_size_y;}

  void setBeginChopper(G4double pos){BeginChop = pos;}
  G4double getBeginChopper()const{return BeginChop;}
  void setEndChop(G4double z_pos){EndChop = z_pos;}
  G4double getEndChop()const{return EndChop;}
  void setEndLinac(G4double z){end_linac = z;}
  G4double GetEndLinac()const{return end_linac;}

  void setBremTargetBeginPosition(G4double pos){brem_target_edge_position = pos;}
  G4double GetBremTargetBeginPosition()const{return brem_target_edge_position;}

  void setEndIntObj(G4double z_pos_con, G4double con_z_size)
  {
    EndIntObj = z_pos_con + con_z_size/2;
    G4cout << "DetectorInformation::Z-Cut set to: " << EndIntObj/(cm) << " cm" << G4endl << G4endl;
  }
  G4double getEndIntObj()const{return EndIntObj;}
  void setRearCollimatorPosition(G4double val){RearColPos = val;}
  G4double getRearCollimatorPosition(void){return RearColPos;}
  void setLinac_MinRadius(G4double r){linac_min_radius = r;}
  G4double GetLinacMinRadius()const{return linac_min_radius;}
  void setLinac_MaxRadius(G4double r){linac_max_radius = r;}
  G4double GetLinacMaxRadius()const{return linac_max_radius;}
  void setDistanceToChop(G4double d){distance_to_chop = d;}
  G4double GetDistanceToChop()const{return distance_to_chop;}

  G4double bremStartPos, linac_size, container_z_pos, water_size_y, container_edge_position;
  G4double BeginChop, EndChop, EndIntObj, RearColPos, linac_min_radius, linac_max_radius, distance_to_chop;
  G4double end_linac, brem_target_edge_position;
  ~DetectorInformation();
};

#endif
