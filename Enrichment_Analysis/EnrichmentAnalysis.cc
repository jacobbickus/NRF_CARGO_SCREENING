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
// Description:
// c++ source file to run on Areg Danagoulian's grasshopper code .dat output files
// This file collects the data printed to grasshopper's output for enriched and
// depleted runs and compares the number of counts incident the detector using 
// a Z-Score test 

#include "EnrichmentAnalysis.hh"

int main(int argc, char* argv[])
{
    string enriched_filename = argv[1];
    string depleted_filename = argv[2];
    Variables enriched_result;
    Variables depleted_result;
    
    infile.open(enriched_filename);
    checkinFile(enriched_filename);
    while(infile && !infile.eof())
    { // read columns
        infile.ignore(100,'\n'); // ignore headers
        infile >> enriched_result.E_beam >> enriched_result.E_incident >> enriched_result.EventID >> enriched_result.ParticleName >> enriched_result.ParticleProcess >> enriched_result.time >> enriched_result.detNumber;
        enriched_result.E_beamv.push_back(enriched_result.E_beam);
        enriched_result.E_incidentv.push_back(enriched_result.E_incident);
    }
    
    infile.close();
    cout << "Enrichement File Read." << endl;
    
    infile.open(depleted_filename);
    checkinFile(depleted_filename);
    while(infile && !infile.eof())
    { // read columns
        infile.ignore(100,'\n'); // ignore headers
        infile >> depleted_result.E_beam >> depleted_result.E_incident >> depleted_result.EventID >> depleted_result.ParticleName >> depleted_result.ParticleProcess >> depleted_result.time >> depleted_result.detNumber;
        depleted_result.E_beamv.push_back(depleted_result.E_beam);
        depleted_result.E_incidentv.push_back(depleted_result.E_incident);
    }
    infile.close();
    cout << "Depleted File Read." << endl;
    cout << "Files Read, displaying results..." <<endl;
    int enriched_photons = enriched_result.E_beamv.size();
    int depleted_photons = depleted_result.E_beamv.size();
    cout << "Number of Enriched Photons: " << enriched_photons <<endl;
    cout << "Number of Depleted Photons: " << depleted_photons << endl;
    // z test
    double z_test = abs(enriched_photons - depleted_photons)/sqrt(pow(sqrt(enriched_photons),2) + pow(sqrt(depleted_photons),2));
    cout << "Z-test result: " << z_test << endl;
    return 0;
}
