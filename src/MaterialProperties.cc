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

#include "MaterialProperties.hh"

MaterialProperties::MaterialProperties()
{
}

G4MaterialPropertiesTable* MaterialProperties::SetWaterProperties()
{
  // ************************** WATER ********************************//

  G4double photonEnergyIOF[] =
  { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
    2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
    2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
    2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
    2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
    3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
    3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
    3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV
  };
  const G4int nEntriesIOF = sizeof(photonEnergyIOF)/sizeof(G4double);

  G4double photonEnergyAbs[] =
  {1.72*eV, 1.746197183*eV, 1.771142857*eV, 1.796811594*eV,1.823235294*eV,
   1.850447761*eV, 1.878484848*eV, 1.907384615*eV, 1.9371875*eV,
   1.967936508*eV, 1.999677419*eV, 2.032459016*eV, 2.066333333*eV,
   2.101355932*eV,2.137586207*eV, 2.175087719*eV, 2.213928571*eV, 2.254181818*eV,
   2.295925926*eV, 2.339245283*eV, 2.384230769*eV, 2.430980392*eV, 2.4796*eV,
   2.530204082*eV, 2.582916667*eV, 2.63787234*eV, 2.695217391*eV,
   2.755111111*eV, 2.817727273*eV, 2.883255814*eV, 2.951904762*eV,
   3.023902439*eV, 3.0995*eV, 3.178974359*eV, 3.262631579*eV,
   3.542285714*eV, 4.132666667*eV, 4.508363636*eV, 4.9592*eV
  };
  const G4int nEntriesAbs = sizeof(photonEnergyAbs)/sizeof(G4double);

  G4double refractiveIndex1[] =
  { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
    1.346,  1.3465, 1.347,  1.3475, 1.348,
    1.3485, 1.3492, 1.35,   1.3505, 1.351,
    1.3518, 1.3522, 1.3530, 1.3535, 1.354,
    1.3545, 1.355,  1.3555, 1.356,  1.3568,
    1.3572, 1.358,  1.3585, 1.359,  1.3595,
    1.36,   1.3608
  };
  assert(sizeof(refractiveIndex1) == sizeof(photonEnergyIOF));

  G4double absorption[] =
  {0.812347685*m, 1.209189843*m, 1.602564103*m, 1.937984496*m, 2.150537634*m,
   2.277904328*m, 2.43902439*m, 2.941176471*m, 3.217503218*m, 3.429355281*m,
   3.629764065*m, 3.78214826*m, 4.496402878*m, 7.4019245*m, 11.16071429*m, 14.38848921*m,
   16.15508885*m, 17.69911504*m, 21.09704641*m, 23.04147465*m, 24.44987775*m,
   30.76923077*m, 49.01960784*m,66.66666667*m,78.74015748*m,94.33962264*m,
   102.145046*m, 108.4598698*m, 157.480315*m, 202.020202*m, 220.2643172*m,
   211.4164905*m, 150.8295626*m, 117.5088132*m, 87.95074758*m,
   60.60606061*m,18.11594203*m,11.79245283*m,9.652509653*m
  };
  assert(sizeof(absorption) == sizeof(photonEnergyAbs));

  // wavelenths 650, 625, 600, 575, 550, 525,
  // 500, 475, 450, 425, 400, 375,
  // 360 (approx peak), 350, 340, 325, 300, 250
  G4double photonEnergyScintillation[] =
  {
    1.90744*eV, 1.9837*eV, 2.0664*eV, 2.156*eV, 2.25425*eV, 2.3615*eV,
    2.4796*eV, 2.6101*eV, 2.75520*eV, 2.9173*eV, 3.0996*eV, 3.306*eV,
    3.44400*eV, 3.542405*eV, 3.64659*eV, 3.8149*eV, 4.132806*eV, 4.95936*eV
  };
  const G4int nEntriesScint = sizeof(photonEnergyScintillation)/sizeof(G4double);

  G4double scintilD[] =
  {
    0.134451, 0.148294, 0.164921, 0.185046, 0.209947, 0.240969,
    0.280459, 0.331664, 0.399644, 0.492194, 0.622117, 0.811069,
    0.941421, 0.23927, 0.0561113, 0.00538955, 6.45194e-05, 6.49917e-10
  };

  assert(sizeof(scintilD) == sizeof(photonEnergyScintillation));

  G4MaterialPropertiesTable* waterMPT = new G4MaterialPropertiesTable();

  waterMPT->AddProperty("RINDEX", photonEnergyIOF, refractiveIndex1, nEntriesIOF)->SetSpline(true);
  waterMPT->AddProperty("ABSLENGTH",photonEnergyAbs, absorption, nEntriesAbs)->SetSpline(true);
  // Data from Onken et al "Time response of water-based liquid scintillator from X-ray excitation"
  // Assumption made that fast and slow components follow same distribution
  waterMPT->AddProperty("FASTCOMPONENT",photonEnergyScintillation, scintilD, nEntriesScint)->SetSpline(true); // Energy Distribution for prompt scintillation
  waterMPT->AddProperty("SLOWCOMPONENT",photonEnergyScintillation, scintilD, nEntriesScint)->SetSpline(true); // Energy distribution for decay scintillation

  // Data from Caravaca et al. "Characterization of water-based liquid scintillator for Cherenkov and scintillation separation"
  waterMPT->AddConstProperty("SCINTILLATIONYIELD",234.0/MeV); // Light Yield Constant Number of photons/MeV
  waterMPT->AddConstProperty("RESOLUTIONSCALE",1.0); // the normal distribution will follow a normal standard deviation width
  waterMPT->AddConstProperty("FASTTIMECONSTANT", 2.25*ns); // value of a 1% WbLS
  waterMPT->AddConstProperty("SLOWTIMECONSTANT",15.10*ns); // value of a 1% WbLS
  waterMPT->AddConstProperty("YIELDRATIO",0.96); // Ratio of fast/slow(delay) photons

  G4double energy_water[] = {
          1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
          1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
          1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
          1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
          1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
          2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
          2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
          2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
          2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
          2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
          3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
          3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
          3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
          4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
          5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
  };

  const G4int numentries_water = sizeof(energy_water)/sizeof(G4double);

  G4double mie_water[] = {
          167024.4*m, 158726.7*m, 150742  *m,
          143062.5*m, 135680.2*m, 128587.4*m,
          121776.3*m, 115239.5*m, 108969.5*m,
          102958.8*m, 97200.35*m, 91686.86*m,
          86411.33*m, 81366.79*m, 76546.42*m,
          71943.46*m, 67551.29*m, 63363.36*m,
          59373.25*m, 55574.61*m, 51961.24*m,
          48527.00*m, 45265.87*m, 42171.94*m,
          39239.39*m, 36462.50*m, 33835.68*m,
          31353.41*m, 29010.30*m, 26801.03*m,
          24720.42*m, 22763.36*m, 20924.88*m,
          19200.07*m, 17584.16*m, 16072.45*m,
          14660.38*m, 13343.46*m, 12117.33*m,
          10977.70*m, 9920.416*m, 8941.407*m,
          8036.711*m, 7202.470*m, 6434.927*m,
          5730.429*m, 5085.425*m, 4496.467*m,
          3960.210*m, 3473.413*m, 3032.937*m,
          2635.746*m, 2278.907*m, 1959.588*m,
          1675.064*m, 1422.710*m, 1200.004*m,
          1004.528*m, 833.9666*m, 686.1063*m
  };

  assert(sizeof(mie_water) == sizeof(energy_water));

  G4double mie_water_const[3]={0.99,0.99,0.8};

  waterMPT->AddProperty("MIEHG",energy_water,mie_water,numentries_water)->SetSpline(true);
  waterMPT->AddConstProperty("MIEHG_FORWARD",mie_water_const[0]);
  waterMPT->AddConstProperty("MIEHG_BACKWARD",mie_water_const[1]);
  waterMPT->AddConstProperty("MIEHG_FORWARD_RATIO",mie_water_const[2]);

  return waterMPT;
}

G4MaterialPropertiesTable* MaterialProperties::SetCasingProperties()
{
  G4MaterialPropertiesTable* casingMPT = new G4MaterialPropertiesTable();
  G4double ephotonPlexi[] = {1.1808067*eV,1.2336786*eV,1.298269*eV,1.3699967*eV,1.4501135*eV,1.5401826*eV,1.6421815*eV,1.7586482*eV,1.8928962*eV,2.049334*eV,
                             2.2339586*eV,2.4551426*eV, 2.7249385*eV,2.850223*eV,3.0*eV,4.0*eV};
 G4double plexiIOF[] = {1.4813,1.482,1.4827,1.4832,1.4837,1.4842,1.485,1.4862,1.488,1.4904,1.4933,1.4962,1.4996,1.5027,1.5027,1.5027};
 assert(sizeof(plexiIOF) == sizeof(ephotonPlexi));
 G4double plexiAbs[] = {1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,30.0*m,30.0*m,11.4*m,11.4*m,7.3*m,7.3*m, 5.0*m, 1.0*mm};
 assert(sizeof(plexiAbs) == sizeof(ephotonPlexi));
  const G4int numPlexi = sizeof(ephotonPlexi)/sizeof(G4double);
  casingMPT->AddProperty("RINDEX", ephotonPlexi, plexiIOF, numPlexi);
  casingMPT->AddProperty("ABSLENGTH", ephotonPlexi, plexiAbs, numPlexi);

  return casingMPT;
}

G4MaterialPropertiesTable* MaterialProperties::SetCasingOpticalProperties()
{
  G4MaterialPropertiesTable* casingOPMPT = new G4MaterialPropertiesTable();
  G4double ephotonPlexi[] = {1.1808067*eV,1.2336786*eV,1.298269*eV,1.3699967*eV,1.4501135*eV,1.5401826*eV,1.6421815*eV,1.7586482*eV,1.8928962*eV,2.049334*eV,
                             2.2339586*eV,2.4551426*eV, 2.7249385*eV,2.850223*eV,3.0*eV,4.0*eV};
 G4double plexiIOF[] = {1.4813,1.482,1.4827,1.4832,1.4837,1.4842,1.485,1.4862,1.488,1.4904,1.4933,1.4962,1.4996,1.5027,1.5027,1.5027};
 assert(sizeof(plexiIOF) == sizeof(ephotonPlexi));
 G4double plexiAbs[] = {1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,30.0*m,30.0*m,11.4*m,11.4*m,7.3*m,7.3*m, 5.0*m, 1.0*mm};
 assert(sizeof(plexiAbs) == sizeof(ephotonPlexi));
  G4double plexiReflectivity[] = {0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02}; // transparent
  G4double plexiTrans[] = {.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98};
  G4double plexiEff[] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};

  assert(sizeof(plexiReflectivity) == sizeof(ephotonPlexi));
  assert(sizeof(plexiTrans) == sizeof(ephotonPlexi));
  assert(sizeof(plexiEff) == sizeof(ephotonPlexi));
  const G4int numPlexi = sizeof(ephotonPlexi)/sizeof(G4double);
  casingOPMPT->AddProperty("REFLECTIVITY", ephotonPlexi, plexiReflectivity, numPlexi);
  casingOPMPT->AddProperty("TRANSMITTANCE", ephotonPlexi, plexiTrans, numPlexi);
  casingOPMPT->AddProperty("RINDEX", ephotonPlexi, plexiIOF, numPlexi);
  casingOPMPT->AddProperty("EFFICIENCY", ephotonPlexi, plexiEff, numPlexi);
  casingOPMPT->AddProperty("ABSLENGTH", ephotonPlexi, plexiAbs, numPlexi);

  return casingOPMPT;
}

G4MaterialPropertiesTable* MaterialProperties::SetTapeProperties()
{
  G4MaterialPropertiesTable* tapeMPT = new G4MaterialPropertiesTable();
  G4double ephotonTape[] = {1.1427161*eV,1.2915073*eV,1.4586435*eV,1.6984206*eV,2.0160114*eV,2.5303*eV,3.262755*eV, 4.0*eV};
  const G4int numTape = sizeof(ephotonTape)/sizeof(G4double);
  G4double tapeIOF[] = {1.32,1.32,1.32,1.32,1.32,1.32,1.33,1.33};
  G4double tapeAbs[] = {1000*m, 1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m};
  assert(sizeof(tapeIOF) == sizeof(ephotonTape));
  assert(sizeof(tapeAbs) == sizeof(ephotonTape));
  tapeMPT->AddProperty("RINDEX", ephotonTape, tapeIOF, numTape);
  tapeMPT->AddProperty("ABSLENGTH", ephotonTape, tapeAbs, numTape);

  return tapeMPT;
}

G4MaterialPropertiesTable* MaterialProperties::SetTapeOpticalProperties()
{
  G4MaterialPropertiesTable* tapeOPMPT = new G4MaterialPropertiesTable();
  G4double ephotonTape[] = {1.1427161*eV,1.2915073*eV,1.4586435*eV,1.6984206*eV,2.0160114*eV,2.5303*eV,3.262755*eV, 4.0*eV};
  const G4int numTape = sizeof(ephotonTape)/sizeof(G4double);
  G4double tapeIOF[] = {1.32,1.32,1.32,1.32,1.32,1.32,1.33,1.33};
  G4double tapeAbs[] = {1000*m, 1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m};
  assert(sizeof(tapeIOF) == sizeof(ephotonTape));
  assert(sizeof(tapeAbs) == sizeof(ephotonTape));
  G4double tapeReflectivity[] = {.98,.98,.98,.98,.98,.98,.98,.98};
  G4double tapeTrans[] = {0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02};
  G4double tapeEff[] = {0.,0.,0.,0.,0.,0.,0.,0.};

  assert(sizeof(tapeReflectivity) == sizeof(ephotonTape));
  assert(sizeof(tapeEff) == sizeof(ephotonTape));
  assert(sizeof(tapeTrans) == sizeof(ephotonTape));

  tapeOPMPT->AddProperty("REFLECTIVITY", ephotonTape, tapeReflectivity, numTape);
  tapeOPMPT->AddProperty("TRANSMITTANCE", ephotonTape, tapeTrans, numTape);
  tapeOPMPT->AddProperty("EFFICIENCY", ephotonTape, tapeEff, numTape);

  return tapeOPMPT;
}

G4MaterialPropertiesTable* MaterialProperties::SetPMTProperties()
{
  G4MaterialPropertiesTable* pmtMPT = new G4MaterialPropertiesTable();

 G4double ephotonPMT[] = {0.4959388*eV, 0.6199*eV, 0.8265*eV, 1.239*eV, 1.653*eV, 2.480*eV,
                          2.755*eV, 3.10*eV, 3.54*eV, 4.133*eV};
 const G4int num = sizeof(ephotonPMT)/sizeof(G4double);

 G4double pmt_iof[] = {1.486, 1.4945, 1.5013, 1.5075, 1.5118, 1.5214, 1.5253, 1.5308, 1.5392, 1.5528};
 assert(sizeof(pmt_iof) == sizeof(ephotonPMT));
  G4double abslength[] = {1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,
                          1000*m,1000*m, 1000*m}; // treating glass as transparent
  assert(sizeof(abslength) == sizeof(ephotonPMT));
  pmtMPT->AddProperty("RINDEX", ephotonPMT, pmt_iof, num)->SetSpline(true);
  pmtMPT->AddProperty("ABSLENGTH", ephotonPMT, abslength, num);

  return pmtMPT;
}

G4MaterialPropertiesTable* MaterialProperties::SetPMTOpticalProperties()
{
  G4MaterialPropertiesTable* pmtOPMPT = new G4MaterialPropertiesTable();
  G4double ephotonPMT[] = {0.4959388*eV, 0.6199*eV, 0.8265*eV, 1.239*eV, 1.653*eV, 2.480*eV,
                           2.755*eV, 3.10*eV, 3.54*eV, 4.133*eV};
  const G4int num = sizeof(ephotonPMT)/sizeof(G4double);

  G4double pmt_iof[] = {1.486, 1.4945, 1.5013, 1.5075, 1.5118, 1.5214, 1.5253, 1.5308, 1.5392, 1.5528};
  assert(sizeof(pmt_iof) == sizeof(ephotonPMT));
  G4double ephotonPMT2[] = {1.7711*eV, 1.9074*eV, 2.0663*eV, 2.2542*eV, 2.4796*eV,
                            2.7551*eV, 3.0995*eV, 3.5423*eV, 4.133*eV, 4.428*eV, 4.959*eV, 6.199*eV};
  const G4int num2 = sizeof(ephotonPMT2)/sizeof(G4double);

  G4double reflectivity[] = {0.03822, 0.0392,  0.040,  0.041,
                             0.0415,  0.0428,  0.0433,  0.0440, 0.0451, 0.0469 };
  G4double transmittance[] = {0.962, 0.961, 0.96, 0.959, 0.959, 0.958, 0.957,0.956,
                              0.955, 0.954};
  assert(sizeof(reflectivity) == sizeof(ephotonPMT));
  assert(sizeof(transmittance) == sizeof(ephotonPMT2));
  G4double efficiency[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  assert(sizeof(efficiency) == sizeof(ephotonPMT));

  pmtOPMPT->AddProperty("REFLECTIVITY", ephotonPMT, reflectivity, num)->SetSpline(true);
  pmtOPMPT->AddProperty("TRANSMITTANCE", ephotonPMT2, transmittance, num2);
  pmtOPMPT->AddProperty("EFFICIENCY", ephotonPMT, efficiency, num);
  pmtOPMPT->AddProperty("RINDEX", ephotonPMT, pmt_iof, num)->SetSpline(true);

  return pmtOPMPT;
}

G4MaterialPropertiesTable* MaterialProperties::SetPCProperties(G4String pc_mat)
{
  G4MaterialPropertiesTable* pc_MPT = new G4MaterialPropertiesTable();
  G4double ephotonPMT[] = {0.4959388*eV, 0.6199*eV, 0.8265*eV, 1.239*eV, 1.653*eV, 2.480*eV,
                           2.755*eV, 3.10*eV, 3.54*eV, 4.133*eV};
  const G4int num = sizeof(ephotonPMT)/sizeof(G4double);
  G4double ephotonPMT2[] = {1.7711*eV, 1.9074*eV, 2.0663*eV, 2.2542*eV, 2.4796*eV,
                            2.7551*eV, 3.0995*eV, 3.5423*eV, 4.133*eV, 4.428*eV, 4.959*eV, 6.199*eV};
  const G4int num2 = sizeof(ephotonPMT2)/sizeof(G4double);
  G4double ephotonPMT3[] = {1.823*eV, 1.8644316*eV, 1.907457*eV, 1.952515*eV,
                            2.0*eV, 2.049*eV, 2.101*eV, 2.156*eV, 2.2140125*eV, 2.2749*eV, 2.339*eV,
                            2.407*eV, 2.480*eV,2.56*eV,2.6379*eV, 2.725*eV, 2.817*eV, 2.917*eV,
                            3.024017*eV, 3.138853*eV, 3.262755*eV};
  const G4int num3 = sizeof(ephotonPMT3)/sizeof(G4double);
  G4double Ga_As_photocath_EFF[]={0.25*perCent,36.246*perCent,39.8*perCent,40.0*perCent,36.0*perCent,30.0*perCent,
                                  24.0*perCent, 15.0*perCent,4.8*perCent, 0.6*perCent, 0.25*perCent, 0.10*perCent}; //Enables 'detection' of photons
  assert(sizeof(Ga_As_photocath_EFF) == sizeof(ephotonPMT2));
  G4double Ga_As_photocath_ReR[]={3.346,3.3588,3.3877,3.4857,3.6209,4.0919,4.6678,4.6102,3.8200, 3.7322};
  assert(sizeof(Ga_As_photocath_ReR) == sizeof(ephotonPMT));
  G4double Ga_As_photocath_ImR[]={0.0056,0.0071,0.0096,0.01523, 0.106, 0.41247, 1.0094, 2.0488, 2.0051, 1.9908};
  assert(sizeof(Ga_As_photocath_ImR) == sizeof(ephotonPMT));
  G4double bialkali_photocath_EFF[] = {0.015*perCent, 0.2*perCent, 3.0*perCent, 7.0*perCent, 18.5*perCent, 25.0*perCent,
                                       28.0*perCent, 26.0*perCent, 6.0*perCent, 0.19*perCent, 0.1*perCent, 0.01*perCent};
  assert(sizeof(bialkali_photocath_EFF) == sizeof(ephotonPMT2));
  G4double bialkali_photocath_ReR[] = {2.96,2.95,2.95,2.95,2.96, 2.98, 3.01, 3.06,
                                       3.12, 3.20, 3.26, 3.09, 3.0,3.0,3.0,2.87,2.7,2.61,2.38,2.18,1.92};
  assert(sizeof(bialkali_photocath_ReR) == sizeof(ephotonPMT3));
  G4double bialkali_photocath_ImR[] = {0.33,0.34,0.34,0.35,0.37,0.38,0.42,0.46,
                                       0.53,0.63,0.86,1.05,1.06,1.11,1.34,1.44,1.50,1.53,1.71,1.69,1.69};
  assert(sizeof(bialkali_photocath_ImR) == sizeof(ephotonPMT3));

  if(pc_mat == "GaAsP")
  {
    pc_MPT->AddProperty("EFFICIENCY", ephotonPMT2,Ga_As_photocath_EFF, num2)->SetSpline(true);
    pc_MPT->AddProperty("REALRINDEX", ephotonPMT,Ga_As_photocath_ReR,num)->SetSpline(true);
    pc_MPT->AddProperty("IMAGINARYRINDEX", ephotonPMT, Ga_As_photocath_ImR, num)->SetSpline(true);
  }
  else if(pc_mat == "Bialkali")
  {
    pc_MPT->AddProperty("EFFICIENCY", ephotonPMT2,bialkali_photocath_EFF, num)->SetSpline(true);
    pc_MPT->AddProperty("REALRINDEX", ephotonPMT3,bialkali_photocath_ReR,num3)->SetSpline(true);
    pc_MPT->AddProperty("IMAGINARYRINDEX", ephotonPMT3, bialkali_photocath_ImR, num3)->SetSpline(true);
  }

  return pc_MPT;
}

G4MaterialPropertiesTable* MaterialProperties::SetAirProperties()
{
  G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
  G4double photonEnergyIOF[] =
  { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
    2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
    2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
    2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
    2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
    3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
    3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
    3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV
  };
  G4double refractiveIndex2[] =
  {
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00
  };
  const G4int nEntriesIOF = sizeof(photonEnergyIOF)/sizeof(G4double);
  airMPT->AddProperty("RINDEX", photonEnergyIOF, refractiveIndex2, nEntriesIOF);

  return airMPT;
}
