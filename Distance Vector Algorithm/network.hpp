/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3 - DISTANCE VECTOR ALGORITHM
    FILENAME: network.hpp

    Header file for Network (graph) class.
    
    WRITTEN BY: Morgan Chapados - March, 2021
---------------------------------------------------------------------------- */

#ifndef NETWORK_H
#define NETWORK_H

#include "router.hpp"
#include <vector>

class Network {
    private:
        std::vector<Router> routers; // list of routers on the network
        std::vector< std::vector<int> > links; // actual links between routers
        // PRIVATE FUNCTIONS --------------------------------------------------
        int getRouterID(std::string name);
        void distanceVector(int id);
    public:
        Network(); // constructor
        ~Network(); // destructor
        // PUBLIC FUNCTIONS ---------------------------------------------------
        void addRouter(Router r);
        void addLink(std::string from, std::string to, int cost);
        void removeRouter(std::string name);
        void removeLink(std::string from, std::string to);
        void printRoutingTables();
        void printRouters();
        void printLinks();
};

#endif