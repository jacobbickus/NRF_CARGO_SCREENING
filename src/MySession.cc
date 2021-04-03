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

#include "MySession.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include <fstream>

extern G4String gOutName;

MySession::MySession() : G4UIsession()
{
        logFile.open(gOutName+".log");
        errFile.open(gOutName+"_error.log");
}

MySession::~MySession()
{
        logFile.close();
        errFile.close();

}

G4int MySession::ReceiveG4cerr(const G4String& cerrString)
{

        errFile << cerrString << std::flush;
        return 0;
}
G4int MySession::ReceiveG4cout(const G4String& coutString)
{
        logFile << coutString << std::flush;
        return 0;
}
