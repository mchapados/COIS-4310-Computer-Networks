/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3 - DISTANCE VECTOR ALGORITHM
    FILENAME: main.cpp

    WRITTEN BY: S. Chapados - March, 2021
    
    PROGRAM DESCRIPTION:
    Implements the distance vector routing algorithm on a simulated network
    of routers modelled as an undirected adjacency matrix.
    
    USAGE: ./main
    PARAMETERS: NONE
---------------------------------------------------------------------------- */

#include <iostream>
#include "router.h"
#include "network.h"
using namespace std;

int main() {
    // create network
    Network Kanto = Network();
    cout << "\nNetwork created";
    
    // create routers
    Router OaksLab = Router("000.000.001.001", "Oak's Lab");
    Router ViridianPC = Router("000.000.002.001", "Viridian Pokemon Centre");
    Router ViridianGym = Router("000.000.002.002", "Viridian Gym");
    cout << "\nRouters created";

    // add routers to network
    Kanto.addRouter(OaksLab);
    cout << "\nAdded Oak's Lab";
    Kanto.addRouter(ViridianPC);
    cout << "\nAdded Viridian Pokemon Centre";
    Kanto.addRouter(ViridianGym);
    cout << "\nAdded Viridian Gym";

    // create links
    Kanto.addLink("Oak's Lab", "Viridian Pokemon Centre", 4);
    cout << "\nCreated link 1";
    Kanto.addLink("Oak's Lab", "Viridian Gym", 6);
    cout << "\nCreated link 2";
    Kanto.addLink("Viridian Gym", "Viridian Pokemon Centre", 1);
    cout << "\nCreated link 3";
    // after last link is created, Oak's Lab -> Viridian Gym should be updated to 5

    // print routing table
    Kanto.printRoutingTables();

    return 0;
}