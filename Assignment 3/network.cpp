/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3
    FILENAME: network.cpp

    Method implementations for Network (graph) class.
    
    WRITTEN BY: S. Chapados - March, 2021
   ------------------------------------------------------------------------- */

#include "network.h"
#include "router.h"
#include <vector>
#include <regex> // to check if a string is an IP address
#include <cmath> // for infinity and min function
using namespace std;

// constructor
Network::Network() {
    routers = vector<Router>();
}

// Destructor
Network::~Network() {
    routers.clear(); // delete all routers on the network
}

// find a router's ID given name or ip address
int Network::getRouterID(string name) {
    bool isIP = false;
    // check if it's an IP
    if (regex_match(name, regex("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
        isIP = true;
    // search for name or IP
    for (int i = 0; i < routers.size(); ++i) {
        if (isIP && routers[i].getAddress() == name)
            return i;
        else if (!isIP && routers[i].getName() == name)
            return i;
    }
    return -1; // if not found
}

void Network::addRouter(Router r) {
    r.setID(routers.size()); // set to next ID in sequence
    routers.push_back(r); // add to end of list

    // initialize routing table
    vector<int> table = vector<int>();
    for (int i = 0; i < routers.size(); ++i) {
        if (i == r.getID())
            table.push_back(0); // set link to self to 0
        else
            table.push_back(INFINITY); // add default link to other routers
    }

    // add routing table to all other routers
    for (int i = 0; i < routers.size()-1; ++i) {
        for (int j = 0; j < routers.size()-1; ++j)
            routers.at(i).getTable().at(j).push_back(INFINITY);
        routers.at(i).getTable().push_back(table);
        if (i == routers.size()-1) // copy last table into new router
            routers.at(r.getID()).setTable(routers.at(i).getTable());
    }
}

void Network::addLink(string from, string to, int cost) {
    int f, t;
    // make sure routers exist and get their IDs
    if ((f = getRouterID(from)) > -1 && (t = getRouterID(to)) > -1) {
        // add the new link to each router's table
        routers.at(f).getTable().at(f).at(t) = cost;
        routers.at(f).getTable().at(t).at(f) = cost;
        routers.at(t).getTable().at(f).at(t) = cost;
        routers.at(t).getTable().at(t).at(f) = cost;
    }
    // call DV algorithm to update everyone
    for (int i = 0; i < routers.size(); ++i)
        distanceVector(routers.at(i));
}

string Network::printRoutingTable() {
    string s = "";
    for (int i = 0; i < routers.size(); ++i) {
        for (int j = 0; j < routers.size(); ++j) { 
            s += routers.at(0).getTable().at(i).at(j);
            s += " ";
        }
        s += "\n";
    }
    return s;
}

void Network::distanceVector(Router source) {

}