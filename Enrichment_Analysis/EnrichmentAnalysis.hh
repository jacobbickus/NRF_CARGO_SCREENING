#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <math.h>

using namespace std;

ifstream infile;

void checkinFile(string filename){
    if(!infile){//check to insure file opens
        cout << "Unable to open file " << filename <<endl;
        exit(1);
    }
    else{
        cout << "Successfully read " << filename << endl;
        
    }
}

struct Variables{
    vector<double> E_beamv;
    double E_beam;
    vector<double> E_incidentv;
    double E_incident;
    int EventID;
    string ParticleName;
    string ParticleProcess;
    double time;
    double detNumber;
};

