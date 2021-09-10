/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3 - DISTANCE VECTOR ALGORITHM
    FILENAME: main.cpp

    WRITTEN BY: Morgan Chapados - March, 2021
    
    PROGRAM DESCRIPTION:
    Implements the distance vector routing algorithm on a simulated network
    of routers modelled as an undirected adjacency matrix.
    
    USAGE: ./main
    Uncomment each section to run different tests. For expected results,
    only one test should be uncommented at a time.
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

    // ORIGINAL NETWORK TEST
    std::cout << "\nRouters on the Network:\n";
    Kanto.printRouters();
    std::cout << "\nLink Table:\n";
    Kanto.printLinks();
    std::cout << "\nRouting Tables:\n\n";
    Kanto.printRoutingTables();


    // TEST 1: REMOVE LINK

    // std::cout << "Removing link from Viridian Pokemon Centre to Viridian Gym...\n";
    // Kanto.removeLink("Viridian Pokemon Centre", "Viridian Gym");
    // std::cout << "\nLink Table:\n";
    // Kanto.printLinks();
    // std::cout << "\nRouting Tables:\n\n";
    // Kanto.printRoutingTables();


    // TEST 2: REMOVE LAST ROUTER

    // std::cout << "Removing last router (Pewter Pokemon Centre)...\n";
    // Kanto.removeRouter("Pewter Pokemon Centre");
    // std::cout << "\nLink Table:\n";
    // Kanto.printLinks();
    // std::cout << "\nRouting Tables:\n\n";
    // Kanto.printRoutingTables();


    // TEST 3: REMOVE MIDDLE ROUTER

    // std::cout << "Removing Viridian Pokemon Centre router...\n";
    // Kanto.removeRouter("Viridian Pokemon Centre");
    // std::cout << "\nLink Table:\n";
    // Kanto.printLinks();
    // std::cout << "\nRouting Tables:\n\n";
    // Kanto.printRoutingTables();


    // TEST 4: ADD ROUTERS AND LINKS

    // std::cout << "Adding to the network...\n";
    // Kanto.addRouter(Router("000.000.003.002", "Pewter Gym"));
    // Kanto.addRouter(Router("000.000.003.003", "Pewter Museum"));
    // Kanto.addLink("Pewter Gym", "Pewter Museum", 1);
    // Kanto.addLink("Pewter Pokemon Centre", "Pewter Museum", 3);

    // std::cout << "\nRouters on the Network:\n";
    // Kanto.printRouters();
    // std::cout << "\nRouting Tables:\n\n";
    // Kanto.printRoutingTables();

    // Kanto.addLink("Pewter Gym", "Pewter Pokemon Centre", 1);
    // std::cout << "\nAfter adding link from Pewter Gym to Pewter Pokemon Centre:\n\n";
    // Kanto.printRoutingTables();


    return 0;
}