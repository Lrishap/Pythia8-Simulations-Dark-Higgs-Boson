#include <iostream>
#include <cmath>
#include "Pythia8/Pythia.h"

using namespace Pythia8;

int main() {
    // Setting the number of events
    int nEvent = 2210798;

    // Generation of event and particles
    Pythia pythia;
    Event& event = pythia.event;

    // Center of mass energy
    pythia.readString("Beams:eCM = 14000.");
    pythia.readString("HardQCD:hardbbbar = on"); // Enable bbbar production

    // B meson decay channels
    pythia.readString("521:onmode = off"); // Switch off all B+ decays
    pythia.readString("521:addchannel = 1 1. 0 321 50"); // B+ decaying to K+ & DarkHiggs

    pythia.readString("-521:onmode = off"); //switching off all B- decays
    pythia.readString("-521: addchannel = 1 1. 0 -321 50"); //B- decaying to K- & DarkHiggs

    pythia.readString("511:onmode = off"); //switching off all B0 decays
    pythia.readString("511: addchannel = 1 1. 0 313 50"); //B0 decaying to K0* & phi

    // Particle with ID 50 (Dark  Higgs) decay channel
    pythia.readString("50:new = DH DH 0 0 0 0.212 0.0 0.0 0.0 298703521.818 0 1 0 1 0");
    pythia.readString("50:mayDecay = on");
    pythia.readString("50:addChannel = 1 0.9 0 13 -13"); // Scalar Higgs decaying to muon pairs

    // Initializing
    pythia.init();
  
    int counter_decay_inside_ADAM = 0; 

    // Begin the event loop
    for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
        // Generate events, quit if failure
        if (!pythia.next()) {
            std::cout << "Event generation aborted prematurely, owing to error!\n";
            break;
        }

        // Loop over the event particles
        for (int i = 0; i < pythia.event.size(); ++i) {
            if (abs(event[i].id()) == 50) {
                double xDec = event[i].xDec();
                double yDec = event[i].yDec();
                double zDec = event[i].zDec();
                double azimuthAngle = event[i].phi() * 180.0 / M_PI;
                
                double dDec = sqrt(pow(xDec, 2) + pow(yDec, 2));
                if (dDec >= 13598.75 && dDec <= 22869.23417 && azimuthAngle >= 45.0 && azimuthAngle <= 135.0 && abs(zDec) <= 22500.0) {
                    counter_decay_inside_ADAM++;
                }
            }
        }
    }

    // End of event loop

    int counter = counter_decay_inside_ADAM;
    std::cout << "Dark Higgs decays inside ADAM fiducial volume: " << counter << std::endl;

    // End Pythia
    pythia.stat();

    return 0;
}