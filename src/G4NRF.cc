//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// *                                                                  *
// * This version (C) 2018 Jayson Vavrek under the MIT License.       *
// ********************************************************************
//
//      ------------ G4NRF physics process --------
//
//      File name: G4NRF
//      Author:        David Jordan (david.jordan@pnl.gov)
//      Creation date: October 2006
//
//      Modifications: Jayson Vavrek (jvavrek@mit.edu)
//        Moderate to major bug fixes, new cross section evaluation,
//        enhanced documentation.
//      Release date: August 2018
//
//      How to use this physics process:
//
//      (1) Store ENSDF-derived nuclear level and gamma emission
//          files in a directory pointed to by the environment
//          variable $G4NRFGAMMADATA.  These files include:
//             z[Z-value].a[A-value] -- default photon evaporation files
//             gamma_table_nnn.dat   -- supplementary info for gamma emission
//             level_table_nnn.dat   -- supplementary level info
//             ground_state_properties.dat
//
//
//      (2) At the top of the user's physicsList class, add:
//
//            #include "G4NRFPhysics.hh"
//
//          Then inside physicsList::ConstructPhysics(), add:
//
//            RegisterPhysics(new G4NRFPhysics("NRF", use_xsec_tables,
//                            use_xsec_integration, force_isotropic));
//
//          where use_xsec_tables, use_xsec_integration, force_isotropic
//          are boolean values for whether to use NRF cross section
//          table interpolation (instead of on-the-fly evaluation),
//          whether to use the high-accuracy numerically-integrated
//          form of the cross section, and whether to force isotropic
//          emission of NRF photons. Recommended: true, true, either.
//          Rather than set flags in the physicsList, the user may want
//          to use command-line arguments.
//
//
//      (3) Material definition requirements: Materials should be constructed
//          from elements containing G4Isotopes in order to trigger
//          calculation of NRF cross sections. NRF interactions can still
//          occur using, e.g. NIST-defined materials, but this has not been
//          tested extensively.
//
//          Example (in user's DetectorConstruction class):
//
//          // Natural Oxygen
//
//          G4Isotope *O16 = new G4Isotope(name="O16", iz=8, n=16, a=15.995*g/mole);
//          G4Isotope *O17 = new G4Isotope(name="O17", iz=8, n=17, a=16.999*g/mole);
//          G4Isotope *O18 = new G4Isotope(name="O18", iz=8, n=18, a=17.992*g/mole);
//          G4Element *NatO = new G4Element
//                       (name="Natural Oxygen", symbol="O", ncomponents=3);
//          NatO->AddIsotope(O16, abundance=99.757*perCent);
//          NatO->AddIsotope(O17, abundance= 0.038*perCent);
//          NatO->AddIsotope(O18, abundance= 0.205*perCent);
//
// -------------------------------------------------------------------

// -------------------------------------------------------------------
//
// Description:
//
// G4NRF.cc is the main Geant4 code for simulating nuclear resonance fluorescence
// (NRF). It primarily calculates the NRF cross section for gamma rays incident on
// a properly-defined material, then calculates the final state if an NRF interaction
// is determined to occur.
//
//
// Major updates to the G4NRF class by Jayson Vavrek (2018):
//
// 0) Ended up replacing NRF_xsec_calc() entirely. Old version is now called
//    NRF_xsec_calc_gaus(). New version performs numerical integration of the
//    integral rather than a Gaussain approximation, and calulates the effective
//    temperature using the new Debye temperature database file. See the methods
//    PsiIntegral() and expIntegrand().
//
// 1) Since the numerical integration can introduce a performance penalty, added
//    the option to build a table of numerically-integrated cross section values
//    vs photon energy at initialization instead of integrating on the fly. At
//    runtime, the sigma vs E table is interpolated using Geant4's c2_function
//    capabilities. This results in at least 40% faster event rates with much
//    higher accuracy, and could likely be optimized even further. See the
//    methods InterpolateCrossSection() and MakeCrossSectionTable().
//
// 2) In NRF_xsec_calc(), added some basic functionality to interrupt the code
//    when an NRF event is triggered with a cross section above some level for
//    debugging of cross section formulae. User must manually set
//      const bool interrupt = false;
//    to take advantage of this if so desired, and un-comment the lines checking
//    for interrupt in NRF_xsec_calc, etc.
//
// 3) Introduction of the print_to_standalone() function, which can be used to
//    generate a standalone plaintext NRF gamma database. A full database print
//    takes about 10-15 minutes (with a 2.4 GHz i5 on a late 2011 Mac.) User must
//    manually set
//      const bool standalone = true;
//    in this file, and may manually adjust the Z/A ranges of isotopes in the
//    print_to_standalone() method below.
//
// 4) Encapsulation of G4NRF by the G4NRFPhysics class, which allows inheritance
//    from G4VPhysicsConstructor as per Geant4 standards for adding custom physics
//    processes. Furthermore, used SetProcessType() and SetProcessSubType(), both
//    of which are set to fHadronic.
//
// -------------------------------------------------------------------

// Edits were made to remove some verbosity by Jacob E Bickus for personal use

#include "G4NRF.hh"

#include <iostream>
#include <cmath>

#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NRFNuclearLevelStore.hh"
#include "G4Exp.hh"

using std::cos;
using std::pow;

const bool interrupt = false;

G4NRF::G4NRF(const G4String& processName, G4bool Verbose_in, G4bool use_xsec_tables_in,
             G4bool use_xsec_integration_in, G4bool force_isotropic_in, G4bool standalone_in)
        : G4VDiscreteProcess(processName),
        Verbose(Verbose_in),
        use_xsec_tables(use_xsec_tables_in),
        use_xsec_integration(use_xsec_integration_in),
        force_isotropic_ang_corr(force_isotropic_in),
        standalone(standalone_in)
{

        if (use_xsec_tables && !use_xsec_integration) {
                G4cout << "Error! Table interpolation with Gaussian xsec approx not currently supported." << G4endl;
                G4cout << "Aborting..." << G4endl;
                exit(48);
        }

        pAngular_Correlation = new Angular_Correlation;

        SetProcessType(fHadronic);
        SetProcessSubType(fHadronic);

        // print gamma info to a datafile and disable some error checking in G4NRFNuclearLevelManager
        // user may change this manually to activate output
        if (standalone) {
                Verbose = true;
                G4cout << "User requesting print gamma info to a datafile!" << G4endl;
                ofstream standaloneFile("standalone.dat");
                print_to_standalone(standaloneFile);
                standaloneFile.close();
        }
}

G4NRF::~G4NRF()
{
        ;
}

void G4NRF::PrintInfoDefinition() {
        G4String comments = "NRF process.\n";
        G4cout << G4endl << GetProcessName() << ":  " << comments;
}

G4double G4NRF::GetMeanFreePath(const G4Track& aTrack, G4double previousStepSize, G4ForceCondition* condition)
{
        G4Material* aMaterial = aTrack.GetMaterial();
        G4double thePreviousStepSize = previousStepSize;
        G4ForceCondition* theCondition = condition;
        G4double GammaEnergy = aTrack.GetDynamicParticle()->GetKineticEnergy();

        const G4int NumberOfElements            = aMaterial->GetNumberOfElements();
        const G4ElementVector* theElementVector = aMaterial->GetElementVector();
        const G4double* NbOfAtomsPerVolume      = aMaterial->GetVecNbOfAtomsPerVolume();

        // Search for nearest level for NRF excitation from ground
        // state.  Search terminates at first (element, isotope, level)
        // such that the level energy is within E_TOL of the incoming
        // gamma energy, taking into account nuclear recoil upon absorption.
        // Note: Search is based on kinematics only, regardless of
        // the value of the NRF cross section for the level.

        const G4double E_TOL = 1.0 * keV;

        G4bool found_nearby_level = false;
        G4int jelm = 0;
        G4double xsec = 0.0;
        G4double sigma = 0.0; // default mean free path

        // Search elements contained in current material

        while ((jelm < NumberOfElements) && (!found_nearby_level)) {
                G4Element* pElement = (*theElementVector)[jelm];

                G4double Atom_number_density = NbOfAtomsPerVolume[jelm];

                G4int num_isotopes = pElement->GetNumberOfIsotopes();


                A_excited      = -1;
                Z_excited      = -1;
                pLevel_excited = NULL;

                // Search isotopes contained within current element

                G4int jisotope = 0;

                while ((jisotope < num_isotopes) && (!found_nearby_level)) {
                        const G4Isotope* pIsotope = pElement->GetIsotope(jisotope);
                        G4int A = pIsotope->GetN(); // n.b. N is # of nucleons, NOT neutrons!
                        G4int Z = pIsotope->GetZ();

                        pNuclearLevelManager =
                                G4NRFNuclearLevelStore::GetInstance()->GetManager(Z, A);

                        G4NRFNuclearLevel* pLevel = pNuclearLevelManager->NearestLevelRecoilAbsorb(GammaEnergy, E_TOL);

                        if (pLevel != NULL) { // i.e., kinematics permit gamma excitation

                                found_nearby_level = true; // setting this ends the search
                                A_excited = A; // stash A, Z, level pointer
                                Z_excited = Z; // for use in PostStepDoIt
                                pLevel_excited = pLevel;

                                // Cross section & mean free path calculation follows
                                if (use_xsec_integration)
                                        xsec = NRF_xsec_calc(GammaEnergy, pLevel, use_xsec_tables);
                                else
                                        xsec = NRF_xsec_calc_gaus(GammaEnergy, pLevel);

                                // Calculate isotope number density
                                const G4double* pIsotopeAbundance = pElement->GetRelativeAbundanceVector();
                                G4double frac_abundance = pIsotopeAbundance[jisotope];

                                G4double Isotope_number_density = Atom_number_density * frac_abundance;

                                // sigma is mean free path = (cross section)*(isotope number density)

                                sigma = Isotope_number_density * xsec;

                        } else {
                                jisotope++; // move on to next isotope in current element
                        }
                } // loop over isotopes (terminate if nearby level found)
                jelm++; // move on to next element in current material
        } // loop over elements (terminate if nearby level found)

        return sigma > DBL_MIN ? 1.0/sigma : DBL_MAX;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
G4double G4NRF::NRF_xsec_calc_gaus(G4double GammaEnergy, const G4NRFNuclearLevel* pLevel) {


        const G4int Z = pLevel->Z(); // isotope Z
        const G4int A = pLevel->A(); // isotope A
        G4NRFNuclearLevelManager *pManager = G4NRFNuclearLevelStore::GetInstance()->GetManager(Z, A);


        const G4double E   = GammaEnergy;
        const G4double J1   = pLevel->AngularMomentum();
        const G4double J0   = pManager->GetGroundStateSpin();
        const G4double E1   = pLevel->Energy();
        const G4double Tau0 = pLevel->Width0();


        //const G4double stat_fac  = (2.0*J1 + 1.0)/(2.0*J0 + 1.0);   // moved factor of 2 to fac1 below
        const G4double stat_fac  = (2.0*J1 + 1.0)/(2.0*J0 + 1.0)/2.0;
        const G4double lamda_bar = hbarc/E; // should be E_r, but unimportant near resonance?

        const G4double M        = A * amu_c2;
        const G4double E_recoil = E1*E1/M;
        const G4double E_r      = E1 + E_recoil/2.0;

        // Assume room temperature for now
        const G4double T     = 300.0*kelvin;
        const G4double Delta = E * sqrt(2.0*k_Boltzmann*T/M);

        const G4double Pi_To_1pt5 = 5.568328;

        const G4double fac1 = 2.0 * Pi_To_1pt5 * pow(lamda_bar, 2.0); // added factor of 2 here, which was previously in stat_fac
        const G4double fac2 = Tau0/Delta;
        const G4double tmp  = (E-E_r)/Delta;
        const G4double fac3 = exp(-tmp*tmp); // the infamous Gaussian approximation

        const G4double xsec = fac1 * stat_fac * fac2 * fac3;

        return xsec;
}

void G4NRF::SetParam_x(G4double x) {
        param_x = x;
}

G4double G4NRF::GetParam_x() const {
        return param_x;
}

void G4NRF::SetParam_t(G4double t) {
        param_t = t;
}

G4double G4NRF::GetParam_t() const {
        return param_t;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
G4double G4NRF::NRF_xsec_calc(G4double GammaEnergy, G4NRFNuclearLevel* pLevel,
                              G4bool useTables, G4int nMeshpoints, G4double sigmaBound) {

        const G4int Z = pLevel->Z(); // isotope Z
        const G4int A = pLevel->A(); // isotope A
        G4NRFNuclearLevelManager *pManager = G4NRFNuclearLevelStore::GetInstance()->GetManager(Z, A);

        const G4double E  = GammaEnergy;               // incident gamma energy
        const G4double E1 = pLevel->Energy();          // excited state energy
        const G4double J1 = pLevel->AngularMomentum(); // excited state spin
        const G4double J0 = pManager->GetGroundStateSpin(); // ground state spin

        const G4double Gamma_r  = pLevel->Width();// level width
        const G4double Gamma_0r = pLevel->Width0(); // partial width

        // compute the statistical spin factor
        const G4double stat_fac  = (2.0*J1 + 1.0)/(2.0*J0 + 1.0)/2.0;

        // compute the average recoil energy, and add half of it to the level energy
        // hitting the resonance requires just a little more energy on top of the level due to recoil
        // half the recoil comes in absorption step, half in decay
        const G4double M        = A * amu_c2;
        const G4double E_recoil = E*E/M;
        const G4double E_r      = E1 + E_recoil/2.0;

        // Get the effective temperature for Doppler broadening from the Manager; use 300 K if unknown
        const G4double T_Debye   = pManager->GetTDebye();
        const G4double T_eff_tmp = pManager->GetTeff();
        const G4double T_eff = (T_eff_tmp > 0 ? T_eff_tmp : 300*kelvin);
        const G4double Delta_eff = E * sqrt(2.0*k_Boltzmann*T_eff/M);

        const G4double rootPi = 1.77245385;

        // dimensionless parameters for the numerical integration in PsiIntegral()
        const G4double x = 2.0 * (E-E_r) / Gamma_r;
        const G4double teff = Delta_eff * Delta_eff / Gamma_r / Gamma_r;
        SetParam_x(x);
        SetParam_t(teff);

        // Compute the NRF _absorption_ cross section. Decay to final state is handled in PostStepDoIt()
        const G4double fac1 = 2.0 * rootPi * stat_fac;
        const G4double fac2 = hbarc*hbarc/E_r/E_r;
        const G4double fac3 = Gamma_0r / Delta_eff;

        G4double xsec, fac4;
        if (useTables) {
                fac4 = 0;
                if (pLevel->GetCrossSectionTable() == NULL) MakeCrossSectionTable(pLevel, T_eff);
                xsec = InterpolateCrossSection(pLevel, E);
        } else {
                fac4 = PsiIntegral(x, teff, nMeshpoints, sigmaBound);
                xsec = fac1 * fac2 * fac3 * fac4;
        }
        if (Verbose && xsec/barn > 2.0 && A == 235) {
                G4cout << std::setprecision(12);
                G4cout << "------------------------------------"    << G4endl;
                G4cout << "A:                  " << A               << G4endl;
                G4cout << "E (MeV):            " << E               << G4endl;
                G4cout << "E_level (MeV):      " << E1/MeV          << G4endl;
                G4cout << "E_r (MeV):          " << E_r/MeV         << G4endl;
                G4cout << "E_recoil (eV):      " << E_recoil/eV     << G4endl;
                G4cout << "xsec (barn):        " << xsec/barn       << G4endl;
                G4cout << "------------------------------------"    << G4endl;
                G4cout << G4endl;
        }

        if (interrupt && xsec/barn > 20.0 && E_r/MeV > 2.1 && E_r/MeV < 2.2) exit(11);
        return xsec;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
G4VParticleChange* G4NRF::PostStepDoIt(const G4Track& trackData,
                                       const G4Step&  stepData) {
        aParticleChange.Initialize(trackData);

        const G4DynamicParticle* aDynamicParticle = trackData.GetDynamicParticle();
        G4double KineticEnergy = aDynamicParticle->GetKineticEnergy();

        G4ThreeVector IncidentGammaDirection = aDynamicParticle->GetMomentumDirection();

        G4bool first_pass = true;
        G4double energy_deposit = 0.0;

        pNuclearLevelManager = G4NRFNuclearLevelStore::GetInstance()->GetManager(Z_excited, A_excited);

        if (pNuclearLevelManager) {
                const G4NRFNuclearLevel* pLevel = pLevel_excited;

                if (!pLevel) {
                        exit(13);
                }

                G4bool continue_cascade = true;

                while (continue_cascade) {
                        G4double Level_energy = pLevel->Energy();
                        G4int nLevel = pLevel->nLevel();


                        G4int jgamma;
                        G4double E_gamma = pLevel->SelectGamma(jgamma); // randomly selects from the level's list of known gammas

                        const G4NRFNuclearLevel* pLevel_next = NULL;
                        G4double Level_energy_next = 0.0; // default is transition to g.s.

                        if (nLevel > 1) { // i.e. currently above 1st excited state
                                pLevel_next = pNuclearLevelManager->NearestLevelRecoilEmit(Level_energy, fabs(E_gamma), 1.0*keV);

                                if (pLevel_next) { // found excited state at lower energy than current state
                                        Level_energy_next = pLevel_next->Energy();
                                } else { // could not find next excited state, so must be g.s. transition
                                        continue_cascade = false;
                                }
                        } else { // i.e. currently at 1st excited state, so g.s. transition is only possibility
                                continue_cascade = false;
                        }

                        G4ThreeVector emitted_gamma_direction; // NRF emission gamma direction

                        G4bool gamma_emission = E_gamma > 0.0; // E_gamma < 0 if c.e.

                        // Check for forbidden transition (i.e. no 0->0 allowed) only if the
                        // event already passed the E_gamma > 0.0 test.

                        if (gamma_emission)
                                gamma_emission = !ForbiddenTransition(pLevel, pLevel_next);

                        if (gamma_emission) { // i.e. gamma emission, not conversion electron
                                if (first_pass) {
                                        G4double J0, J, Jf; // Spin of initial, intermediate, & final levels
                                        G4int L1, L2; // Angular momentum of excitation, de-excitation gammas
                                        G4double Delta1, Delta2; // mixing ratios for excitation, de-excitation

                                        if (!force_isotropic_ang_corr) {
                                                SetupMultipolarityInfo(nLevel, E_gamma, jgamma, pLevel, pLevel_next, J0, J, Jf, L1, L2, Delta1, Delta2);

                                                emitted_gamma_direction = SampleCorrelation(J0, J, Jf, L1, L2, Delta1, Delta2);

                                                emitted_gamma_direction.rotateUz(IncidentGammaDirection);
                                        } else { // User has chosen to disable angular correlations
                                                emitted_gamma_direction = SampleIsotropic();
                                        }
                                } else { // angular correlations not included after first pass (i.e., after first (gamma, gamma) pair)
                                        emitted_gamma_direction = SampleIsotropic();
                                }

                                // Create G4DynamicParticle object for the emitted gamma and add it to the tracking stack.
                                // IMPORTANT NOTE: this currently produces _monoenergetic_ photons with energy E_gamma, rather
                                // than the incident energy minus the recoil!
                                G4DynamicParticle* aGamma = new G4DynamicParticle(G4Gamma::Gamma(), emitted_gamma_direction, E_gamma);
                                aParticleChange.AddSecondary(aGamma);
                        } // gamma emission? (i.e. E_gamma > 0?)


                        if (gamma_emission) { // calculate nuclear recoil for gamma emission
                                const G4double M        = A_excited * amu_c2;
                                const G4double E_recoil = E_gamma*E_gamma/M;
                                energy_deposit          += E_recoil;
                        } else { // conversion electron
                                energy_deposit += Level_energy - Level_energy_next;
                        }

                        first_pass = false;
                        pLevel = pLevel_next;
                } // continue de-excitation cascade?
        } else { // pNuclearLevelManager is NULL -- anomalous condition, shouldn't have occurred if G4NRF::GetMeanFreePath() behaved
                exit(14);
        }

        aParticleChange.ProposeLocalEnergyDeposit(energy_deposit);

        aParticleChange.ProposeTrackStatus(fStopAndKill);

        return G4VDiscreteProcess::PostStepDoIt(trackData, stepData);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
void G4NRF::SetupMultipolarityInfo(const G4int nLevel,
                                   const G4double E_gamma, const G4int jgamma,
                                   const G4NRFNuclearLevel* pLevel,
                                   const G4NRFNuclearLevel* pLevel_next,
                                   G4double& J0, G4double& J, G4double& Jf,
                                   G4int& L1, G4int& L2,
                                   G4double& Delta1, G4double& Delta2) {
        // Determines information on excitation & de-excitation gamma transition
        // multipolarity required for angular correlation sampling.
        //
        // Input quantities:
        //    nLevel      -- index of current level (i.e., excited state)
        //    E_gamma     -- energy of cascade gamma
        //    jgamma      -- index of cascade gamma emitted by current level
        //    pLevel      -- pointer to current level
        //    pLevel_next -- point to de-excitation level (NULL if transition to g.s.)
        //
        // Output quantities (all passed back by reference):
        //    J0     -- spin of starting level (ground state by assumption)
        //    J      -- spin of current level (i.e., excited state -- this is the intermediate state)
        //    Jf     -- spin of final level (may be g.s. or excited state of lower energy than current state)
        //    L1     -- angular momentum of excitation gamma
        //    L2     -- angular momentum of de-excitation gamma
        //    Delta1 -- mixing coefficient for excitation channel
        //    Delta2 -- mixing coefficient for de-excitation channel

        J0 = pNuclearLevelManager->GetGroundStateSpin();
        G4double P0 = pNuclearLevelManager->GetGroundStateParity();

        J = pLevel->AngularMomentum();
        G4double P = pLevel->Parity();

        G4double Pf;


        if (nLevel > 1) { // current level is higher than 1st excited
                if (pLevel_next) { // pointer to next level isn't NULL, so it's another excited state
                        Jf = pLevel_next->AngularMomentum();
                        Pf = pLevel_next->Parity();
                } else {  // de-excitation to g.s.
                        Jf = J0;
                        Pf = P0;
                }
        } else {          // currently at 1st excited state, so g.s. de-excitation is only possibility
                Jf = J0;
                Pf = P0;
        }

        // Need index to highest-energy gamma emitted from this level
        G4int ngamma = pLevel->NumberOfGammas();
        AssignMultipoles(pLevel, E_gamma, ngamma-1, J0, P0, J, P, L1, Delta1);

        if (jgamma != ngamma-1) {
                AssignMultipoles(pLevel, E_gamma, jgamma, J, P, Jf, Pf, L2, Delta2);
        } else {
                L2     = L1;
                Delta2 = Delta1;
        }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
void G4NRF::AssignMultipoles(const G4NRFNuclearLevel* pLevel,
                             const G4double E_gamma, const G4int jgamma,
                             const G4double Ji, const G4double Pi,
                             const G4double Jf, const G4double Pf,
                             G4int& L_j, G4double& Delta) {


        G4int Gamma_num_mult         = (pLevel->MultipoleNumModes()        )[jgamma];
        char Gamma_multipole_mode1  = (pLevel->MultipoleMode1()           )[jgamma];
        G4int Gamma_multipole_L1     = (pLevel->MultipoleL1()              )[jgamma];

        //   char     Gamma_multipole_mode2  = (pLevel->MultipoleMode2()           )[jgamma];
        //   G4int    Gamma_multipole_L2     = (pLevel->MultipoleL2()              )[jgamma];

        G4double Gamma_mixing_ratio     = (pLevel->MultipoleMixingRatio()     )[jgamma];
        G4int Gamma_mixing_sign_flag = (pLevel->MultipoleMixRatioSignFlag())[jgamma];


        char transition;
        switch (Gamma_num_mult) {
        case 0:
                // No ENSDF info available.  In this case, determine minimum L permitted
                // by selection rules, and calculate mixing coefficient based on
                // single-particle (Weisskopf) estimates.

                L_j     = FindMin_L(Ji, Pi, Jf, Pf, transition);
                Delta = MixingRatio_WeisskopfEstimate(transition, L_j, E_gamma, Pi, Pf);

                break;

        case 1:
                // A single (presumably dominant) transition multipole according to
                // ENSDF.  In this case, return the ENSDF multipolarity and set
                // the mixing coefficient to zero.

                L_j = Gamma_multipole_L1;
                Delta = 0.0;
                break;

        case 2:

                L_j = Gamma_multipole_L1;
                if (Gamma_mixing_sign_flag != -1) { // N.B. mixing_sign_flag = -1 accompanies
                        Delta = Gamma_mixing_ratio; // mixing_ratio = -999 for unknown ratio in ENSDF_parser_3.f
                } else {
                        transition = Gamma_multipole_mode1;
                        Delta = MixingRatio_WeisskopfEstimate(transition, L_j, E_gamma, Pi, Pf);
                }

                break;

        default:
                // Shouldn't get to this default case if ENSDF data parsing went as expected.
                G4cout << "ERROR in G4NRF::AssignMultipoles." << G4endl;
                G4cout << "Aborting." << G4endl;
                exit(15);
                break;
        }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
G4int G4NRF::FindMin_L(const G4double Ji, const G4double Pi,
                       const G4double Jf, const G4double Pf,
                       char& transition) {

        G4double L_j;

        if (Ji != Jf)
                L_j = fabs(Ji-Jf);
        else
                L_j = 1.0;

        G4int Lint = int(L_j + 0.001);

        if (Lint % 2 == 0) { // L is even
                if (Pi == Pf)
                        transition = 'E';
                else
                        transition = 'M';
        } else {         // L is odd
                if (Pi == Pf)
                        transition = 'M';
                else
                        transition = 'E';
        }

        return Lint;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
G4double G4NRF::MixingRatio_WeisskopfEstimate(char transition, const G4int L_j,
                                              const G4double E_gamma,
                                              const G4double Pi, const G4double Pf) {
        // Calcualate multipole mixing ratio based on single-particle (Weisskopf)
        // formulas for the transition probabilities.  See Krane, Introductory Nuclear
        // Physics (New York: John Wiley, 1988), pp. 332-335.


        if (L_j > 3)
                return 0.0;

        const G4int L_prime = L_j + 1;
        char multipole;
        char multipole_prime;

        switch (transition) {
        case 'E':
                multipole = 'E';
                break;
        case 'M':
                multipole = 'M';
                break;
        case 'Q': // L is even
                if (Pi == Pf)
                        multipole = 'E';
                else
                        multipole = 'M';
                break;
        case 'D': // L is odd
        case 'O':
                if (Pi == Pf)
                        multipole = 'M';
                else
                        multipole = 'E';
                break;
        default:
                G4cout << "Error in G4NRF::MixingRatio_WeisskopfEstimate." << G4endl;
                G4cout << "Aborting." << G4endl;
                exit(16);
        }

        if (multipole == 'E')
                multipole_prime = 'M';
        else
                multipole_prime = 'E';

        G4double lamda, lamda_prime;


        lamda       = Lamda_Weisskopf(multipole,       L_j,       E_gamma);
        lamda_prime = Lamda_Weisskopf(multipole_prime, L_prime, E_gamma);

        G4double Delta = lamda_prime/lamda;

        return Delta;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
G4double G4NRF::Lamda_Weisskopf(char multipole, const G4int L_j,
                                const G4double E_gamma) {

        G4double Lamda = 0.0;


        if ((L_j < 0) || (L_j > 4))
                return Lamda;

        G4double A = double(A_excited);

        G4double E_MeV = E_gamma/MeV;

        if (multipole == 'E') {
                switch (L_j) {
                case 1:
                        Lamda = (1.0e14) * pow(A, 0.666) * pow(E_MeV, 3.0);
                        break;
                case 2:
                        Lamda =  (7.3e7) * pow(A, 1.333) * pow(E_MeV, 5.0);
                        break;
                case 3:
                        Lamda =   (34.0) * pow(A, 2.0)   * pow(E_MeV, 7.0);
                        break;
                case 4:
                        Lamda = (1.1e-5) * pow(A, 2.666) * pow(E_MeV, 9.0);
                        break;
                }
        } else { // 'M'
                switch (L_j) {
                case 1:
                        Lamda = (5.6e13)                 * pow(E_MeV, 3.0);
                        break;
                case 2:
                        Lamda =  (2.5e7) * pow(A, 0.666) * pow(E_MeV, 5.0);
                        break;
                case 3:
                        Lamda =   (16.0) * pow(A, 1.333) * pow(E_MeV, 7.0);
                        break;
                case 4:
                        Lamda = (4.5e-6) * pow(A, 2.0)   * pow(E_MeV, 9.0);
                        break;
                }
        }

        return Lamda * (1.0/second);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
G4ThreeVector G4NRF::SampleCorrelation(const G4double Ji, const G4double J,  const G4double Jf,
                                       const G4int L1, const G4int L2,
                                       const G4double Delta1, const G4double Delta2) {


        G4bool correlation_info_available =
                pAngular_Correlation->ReInit(J, Ji, Jf, L1, L2, Delta1, Delta2);


        if (correlation_info_available) {
                G4double y_rnd = G4UniformRand();

                G4double cos_theta = pAngular_Correlation->Sample(y_rnd);

                G4double theta     = acos(cos_theta);
                G4double sin_theta = sin(theta);
                G4double phi       = 2.0*pi*G4UniformRand();

                G4double cos_x = sin_theta*cos(phi);
                G4double cos_y = sin_theta*sin(phi);
                G4double cos_z = cos_theta;

                G4ThreeVector new_direc(cos_x, cos_y, cos_z);

                return new_direc;
        } else { // angular momenta aren't in Angular_Correlation coefficient tables -- bail out with isotropic emission direction
                return SampleIsotropic();
        }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
G4ThreeVector G4NRF::SampleIsotropic() {
        G4double cos_theta = -1.0 + 2.0*G4UniformRand();
        G4double phi       = 2.0*pi*G4UniformRand();
        G4double theta     = acos(cos_theta);
        G4double sin_theta = sin(theta);

        G4double cos_x = sin_theta*cos(phi);
        G4double cos_y = sin_theta*sin(phi);
        G4double cos_z = cos_theta;

        G4ThreeVector new_direc(cos_x, cos_y, cos_z);

        return new_direc;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// ****************************************************************************************************
G4bool G4NRF::ForbiddenTransition(const G4NRFNuclearLevel* pLevel,
                                  const G4NRFNuclearLevel* pLevel_next) {

        G4double Ji = pLevel->AngularMomentum();
        G4double Jf;

        G4bool forbidden = false;

        if (pLevel_next)
                Jf = pLevel_next->AngularMomentum();
        else
                Jf = pNuclearLevelManager->GetGroundStateSpin();

        if ((Ji == 0.0) && (Jf == 0.0))
                forbidden = true;

        return forbidden;
}


// perform the integral for doppler broadening numerically
// this gives the most stable results
G4double G4NRF::expIntegrand(G4double y) const {
        const G4double x = GetParam_x();
        const G4double t = GetParam_t();
        return G4Exp(-(x-y)*(x-y)/4.0/t) / (1.0+y*y);
}

G4double G4NRF::PsiIntegral(G4double x, G4double t, G4int nMeshpoints, G4double sigmaBound) {
        G4double z2 = x*x/t;
        G4double ztol2 = 1.0e6;
        if (z2 > ztol2) return 0;

        // Roundabout way of passing these to expIntegrand
        SetParam_x(x);
        SetParam_t(t);

        // integrate out to plus/minus 4 "sigma" for numerical stability
        const G4double theLowerLimit = -sigmaBound*sqrt(2.0*t);
        const G4double theUpperLimit = -theLowerLimit;

        // perform the integration
        G4double sum = integrator.Simpson(this, &G4NRF::expIntegrand, theLowerLimit, theUpperLimit, nMeshpoints);

        return sum;
}

void G4NRF::MakeCrossSectionTable(G4NRFNuclearLevel* pLevel, G4double Teff) {
        G4int A = pLevel->A();
        G4int Z = pLevel->Z();
        G4double E1 = pLevel->Energy();

        G4double M = A * amu_c2;
        G4double Er = E1 + E1*E1/(2.0*M); // small approximation to use E1 instead of E, but it makes it constant for the table
        G4double Delta_eff = Er * sqrt(2.0*k_Boltzmann*Teff/M);

        std::vector<G4double> cross_sec_tab;
        std::vector<G4double> E_tab;

        // user may adjust the for loop bounds to change the NRF xsec table bounds
        // user may also adjust the inputs to NRF_xsec_calc below to change the accuracy of the table build
        for (G4double e = -10.0*Delta_eff; e <= 10.0*Delta_eff; e += Delta_eff/100.0) {
                G4double xsec = 0.0;
                if (use_xsec_integration) {
                        xsec = NRF_xsec_calc(Er + e, pLevel, false, 10000, 10.0);
                } else {
                        xsec = NRF_xsec_calc_gaus(Er + e, pLevel);
                }

                cross_sec_tab.push_back(xsec);
                E_tab.push_back(Er + e);
        }

        interpolating_function_p<G4double> *cross_sec_interp_func = new interpolating_function_p<G4double>();
        cross_sec_interp_func->load(E_tab, cross_sec_tab, true, 0, true, 0, true);
        pLevel->SetCrossSectionTable(cross_sec_interp_func);
}


G4double G4NRF::InterpolateCrossSection(const G4NRFNuclearLevel* pLevel, G4double GammaEnergy) {
        const interpolating_function_p<G4double> *xsec_table = pLevel->GetCrossSectionTable();
        if (GammaEnergy < xsec_table->xmin() || GammaEnergy > xsec_table->xmax())
                return 0;
        else
                return xsec_table->value_with_derivatives(GammaEnergy, NULL, NULL);
}

void G4NRF::print_to_standalone(ofstream& file) {
        G4cout << "Calling G4NRF::print_to_standalone()" << G4endl;

        for (int z = 92; z <= 94; ++z) {
                for (int a = 235; a <= 240; ++a) {
                        G4NRFNuclearLevelManager* pManager = G4NRFNuclearLevelStore::GetInstance()->GetManager(z, a, true);
                        if (pManager) pManager->PrintAllTabular(file);
                }
        }
}
