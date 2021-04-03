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

#ifndef PhysicsListNew_hh
#define PhysicsListNew_hh 1

#include "G4VModularPhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4DecayPhysics.hh"
#include "G4NRFPhysics.hh"

#include "G4OpticalPhysics.hh"
#ifdef G4_OPTPARAM
 #include "G4OpticalParameters.hh"
#endif
#include "G4Cerenkov.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"

#include "G4EmStandardPhysics_option4.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmProcessOptions.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronicProcessStore.hh"
#include "G4LossTableManager.hh"
#include "G4ProcessTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"


class PhysicsListNew: public G4VModularPhysicsList {
 public:
  PhysicsListNew(G4bool, G4bool, G4bool, G4bool, G4bool, G4bool);
  ~PhysicsListNew();

  void ConstructParticle();
  void ConstructPhysics();
  void ConstructProcess();
  void SetCuts();

 private:
  G4bool addNRF, use_xsec_tables, use_xsec_integration, force_isotropic, standalone, NRF_Verbose;
};

#endif
