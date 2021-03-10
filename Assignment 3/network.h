/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3
    FILENAME: network.h

    Header file for Network (graph) class.
    
    WRITTEN BY: S. Chapados - March, 2021
   ------------------------------------------------------------------------- */

#ifndef NETWORK_H
#define NETWORK_H

#include "router.h"
#include <vector>
using namespace std;

class Network {
    private:
        vector<Router> routers;
        int size;
        int getRouterID(string name);
        void distanceVector(Router source);
    public:
        Network(); // constructor
        ~Network(); // destructor
        vector<Router> getRouters() { return routers; } // getter method
        void addRouter(Router r); // add a router to the network
        void addLink(string from, string to, int cost); // add a link between two routers
        void printRoutingTable();
};

#endif