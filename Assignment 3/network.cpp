/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3 - DISTANCE VECTOR ALGORITHM
    FILENAME: network.cpp

    Method implementations for Network (graph) class.
    
    WRITTEN BY: S. Chapados - March, 2021
---------------------------------------------------------------------------- */

#include "network.h"
#include "router.h"
#include <vector>
#include <regex> // to check if a string is an IP address
#include <cmath> // for infinity and min function
#include <iostream> // for debugging
using namespace std;

// constructor
Network::Network() {
    routers = vector<Router>();
    size = 0;
}

// Destructor
Network::~Network() {
    routers.clear(); // delete all routers on the network
}

/*  ---------------------------------------------------------------------------
    FUNCTION: getRouterID
    DESCRIPTION: Finds a router's ID on the network given name or address.
    RETURNS: int

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
int Network::getRouterID(string name) {
    bool isIP = false;
    // check if it's an IP
    if (regex_match(name, regex("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
        isIP = true;
    // search for name or IP
    for (int i = 0; i < routers.size(); ++i) {
        if (isIP && routers.at(i).getAddress() == name)
            return i;
        else if (!isIP && routers.at(i).getName() == name)
            return i;
    }
    return -1; // if not found
}

/*  ---------------------------------------------------------------------------
    FUNCTION: addRouter
    DESCRIPTION: Adds a new router to the network, assigns it an ID, and
    initializes its routing table.
    RETURNS: VOID

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
void Network::addRouter(Router r) {
    vector<int> table; // new router's table
    r.setID(size); // set to next ID in sequence
    routers.push_back(r); // add to end of list
    ++size;

    // initialize routing table
    for (int i = 0; i < size; ++i) {
        if (i == r.getID())
            table.push_back(0); // set link to self to 0
        else
            table.push_back(INFINITY); // add default link to other routers
    }

    if (size == 1) // this is the first router in the network
        routers.at(0).updateTable(0, table);
    else {
        // create the complete routing table
        for (int i = 0; i < size-1; ++i) {
            // add new link to each router's table
            routers.at(0).updateTable(i, size-1, INFINITY); 
        }
        // add new router's table to end
        routers.at(0).updateTable(size-1, table);
    }

    // copy routing table to all routers
    for (int i = 1; i < size; ++i) {
        routers.at(i).setTable(routers.at(0).getTable());
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: addLink
    DESCRIPTION: Creates a new undirected link between two routers. Does 
    nothing if one or both routers don't exist on the network.
    PARAMETERS:
        from string : name or ip of first router
        to string : name or ip of second router
        cost int : cost/distance between the routers
    RETURNS: VOID

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
void Network::addLink(string from, string to, int cost) {
    int f, t;
    // make sure routers exist and get their IDs
    if ((f = getRouterID(from)) > -1 && (t = getRouterID(to)) > -1) {
        // add the new link to each router's table
        routers.at(f).updateTable(f, t, cost);
        routers.at(f).updateTable(t, f, cost);
        routers.at(t).updateTable(f, t, cost);
        routers.at(t).updateTable(t, f, cost);
        // call DV algorithm to update everyone
        for (int i = 0; i < size; ++i)
            distanceVector(routers.at(i));
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: printRoutingTables
    DESCRIPTION: Outputs all of the routing tables. Used for debugging so far.
    RETURNS: VOID

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
void Network::printRoutingTables() {
    for (int i = 0; i < routers.size(); ++i) {
        routers.at(i).printTable();
        cout << "\n";
    }
}

void Network::distanceVector(Router source) {

}