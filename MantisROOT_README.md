MANTISROOT
==

Analysis Package for Mantis Simulations providing the User with quick analysis on Mantis output files.

LOADING INTO ROOT 
==

root MantisROOT.cc

`> MantisROOT* m = new MantisROOT(debug=false)`

MEMBER FUNCTIONS 
==

***************************************************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************

CheckAngles
==

void CheckAngles(const char* filename, int estimate=-1)

DESCRIPTION: 

Determines which Angles emitted from IntObjOut are Detected.
Estimate sets the limit on how many events to check. Set estimate to -1 for all events.
Usefull for determining emission angle cuts to place for developing efficient simulations 


***************************************************************************************************************************************************************

CheckDet
==

void CheckDet(const char* filename, bool weighted=false, int estimate=-1)

DESCRIPTION: 
Checks DetInfo TTree for matches in IntObjOut TTree event IDs.
If there is not a matching ID the DetInfo is removed for that event.
If DetInfo is a weighted spectrum include the second input bool true.
For a subset of the data include the third input. Set estimate to -1 for all events 


***************************************************************************************************************************************************************

CheckEvents
==

void CheckEvents(const char* filename, bool Weighted, bool Corrected)

DESCRIPTION: 
Checks TTrees NRF, Cherenkov and DetInfo for EventIDs that match.
If the events match the event data in the detector is collected and written to w_events_(filename).
This function is called in Sig2Noise.
Corrected refers to whether or not the Corrected_DetInfo has been written to filename after running CheckDet.
Example: mantis->CheckEvents("test.root",true, true) would Check WEIGHTED CORRECTED events in file test.root.

***************************************************************************************************************************************************************
