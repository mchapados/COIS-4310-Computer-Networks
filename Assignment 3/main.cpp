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

#include "router.hpp"
#include "network.hpp"
#include <iostream>

int main() {
    // create network
    Network Kanto;

    // add routers to network
    Kanto.addRouter(Router("000.000.001.001", "Oak's Lab"));
    Kanto.addRouter(Router("000.000.002.001", "Viridian Pokemon Centre"));
    Kanto.addRouter(Router("000.000.002.002", "Viridian Gym"));
    Kanto.addRouter(Router("000.000.003.001", "Pewter Pokemon Centre"));

    // create links
    Kanto.addLink("Oak's Lab", "Viridian Pokemon Centre", 4);
    Kanto.addLink("Oak's Lab", "Viridian Gym", 6);
    Kanto.addLink("Viridian Pokemon Centre", "Viridian Gym", 1);
    Kanto.addLink("Viridian Gym", "Pewter Pokemon Centre", 2);
    // after last link is created, Oak's Lab -> Viridian Gym should be updated to 5

    std::cout << "\nRouters on the Network:\n";
    Kanto.printRouters();
    std::cout << "\nLink Table:\n";
    Kanto.printLinks();
    std::cout << "\nRouting Tables:\n";
    Kanto.printRoutingTables();

    return 0;
}