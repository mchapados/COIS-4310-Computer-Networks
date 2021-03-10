/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3 - DISTANCE VECTOR ALGORITHM
    FILENAME: network.h

    Header file for Network (graph) class.
    
    WRITTEN BY: S. Chapados - March, 2021
---------------------------------------------------------------------------- */

#ifndef NETWORK_H
#define NETWORK_H

#include "router.h"
#include <vector>
using namespace std;

class Network {
    private:
        vector<Router> routers; // list of routers on the network
        int size; // number of routers on the network
        // PRIVATE FUNCTIONS --------------------------------------------------
        int getRouterID(string name);
        void updateNeighbours(int id);
        void distanceVector(Router source);
    public:
        Network(); // constructor
        ~Network(); // destructor
        vector<Router> getRouters() { return routers; } // getter method
        // PUBLIC FUNCTIONS ---------------------------------------------------
        void addRouter(Router r);
        void addLink(string from, string to, int cost);
        void printRoutingTables();
};

#endif