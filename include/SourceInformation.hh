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

#ifndef SourceInformation_h
#define SourceInformation_h 1

#include "G4ios.hh"
#include "G4Types.hh"
#include "globals.hh"

class SourceInformation
{
  static SourceInformation *instance;
  G4double z_pos;

  SourceInformation();
public:
  static SourceInformation *Instance()
  {
    if(!instance)
    {
      instance = new SourceInformation;
    }
    return instance;
  }

  void SetSourceZPosition(G4double);
  G4double GetSourceZPosition()const{return z_pos;}
  ~SourceInformation();
};

#endif
