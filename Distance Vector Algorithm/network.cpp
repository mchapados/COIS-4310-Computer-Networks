/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3 - DISTANCE VECTOR ALGORITHM
    FILENAME: network.cpp

    Method implementations for Network (graph) class.
    
    WRITTEN BY: Morgan Chapados - March, 2021
---------------------------------------------------------------------------- */

#include "network.hpp"
#include "router.hpp"
#include <vector>
//#include <regex> // to check if a string is an IP address
#include <iostream>
#define INFTY 541196290 // very large number to represent non-existent links

// constructor
Network::Network() {
    routers = std::vector<Router>();
    links = std::vector< std::vector<int> >();
}

// Destructor
Network::~Network() {
    routers.clear(); // delete all routers on the network
}

/*  ---------------------------------------------------------------------------
    FUNCTION: getRouterID
    DESCRIPTION: Finds a router's ID on the network given name or address.

    IP check commented out because regex doesn't work on Loki :(
    This is more of a nice-to-have anyway.

    RETURNS: int

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
int Network::getRouterID(std::string name) {
    bool isIP = false;
    // check if it's an IP
    // if (std::regex_match(name, 
    //     std::regex("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
    //     isIP = true;
    // search for name or IP
    for (size_t i = 0; i < routers.size(); ++i) {
        if (isIP && routers[i].getAddress() == name)
            return i;
        else if (!isIP && routers[i].getName() == name)
            return i;
    }
    return -1; // if not found
}

/*  ---------------------------------------------------------------------------
    FUNCTION: addRouter
    DESCRIPTION: Adds a new router to the network, assigns it an ID, and
    initializes its routing table.
    RETURNS: VOID

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
void Network::addRouter(Router r) {
    std::vector<int> table; // new router's table
    r.setID(routers.size()); // set to next ID in sequence
    routers.push_back(r); // add to end of list

    // initialize routing table
    for (size_t i = 0; i < routers.size(); ++i) {
        if (i == r.getID())
            table.push_back(0); // set link to self to 0
        else
            table.push_back(INFTY); // add default link to other routers
    }

    if (routers.size() == 1) { // this is the first router in the network
        routers[0].updateTable(0, table);
        links.push_back(table); // add default links to network
    }
    else {
        // copy first router's table
        routers[routers.size()-1].setTable(routers[0].getTable());
        // for each router
        for (size_t i = 0; i < routers.size(); ++i) {
            for (size_t j = 0; j < routers.size(); ++j) {
                // add new link (column) to its routing table
                routers[i].updateTable(j, routers.size()-1, INFTY); 
            }
            // add new router's table (row) to end
            routers[i].updateTable(routers.size()-1, table);
        }
        // update network (actual) links
        for (size_t i = 0; i < routers.size()-1; ++i)
            links[i].push_back(INFTY);
        links.push_back(table);
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

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
void Network::addLink(std::string from, std::string to, int cost) {
    int f, t;
    // make sure routers exist and get their IDs
    if ((f = getRouterID(from)) > -1 && (t = getRouterID(to)) > -1) {
        // add the new link to the network link table
        links[f][t] = cost;
        links[t][f] = cost;
        // add the new link to routing tables
        routers[f].updateTable(f, t, cost);
        routers[f].updateTable(t, f, cost);
        routers[t].updateTable(f, t, cost);
        routers[t].updateTable(t, f, cost);
        // calculate new distance vector
        distanceVector(f);
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: removeLink
    DESCRIPTION: Removes an existing link between two routers (by setting cost
    to infinity). Does nothing if one or both routers don't exist on the 
    network.
    PARAMETERS:
        from string : name or ip of first router
        to string : name or ip of second router
    RETURNS: VOID

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
void Network::removeLink(std::string from, std::string to) {
    int f, t;
    // make sure routers exist and get their IDs
    if ((f = getRouterID(from)) > -1 && (t = getRouterID(to)) > -1) {
        // remove link from the network link table
        links[f][t] = INFTY;
        links[t][f] = INFTY;
        // copy back to each router
        // (necessary because DV algorithm overwrites the original links)
        for (size_t i = 0; i < routers.size(); ++i) {
            routers[i].setTable(links); // reset routing table
            routers[i].setOrigins(); // reset DV origins
        }
        // calculate new distance vector
        distanceVector(f); 
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: removeRouter
    DESCRIPTION: Removes a router from the network (given name or IP) along 
    with all associated links. Does nothing if the router doesn't exist.
    PARAMETERS:
        name string : name or ip of router to remove
    RETURNS: VOID

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
void Network::removeRouter(std::string name) {
    int id = getRouterID(name); // get router's id on the network
    if (id > -1) { // make sure router exists
        // remove associated links
        if (id != routers.size()-1) { // if it's not the last router in list
            // copy last column into column of router to delete
            for (size_t i = 0; i < links.size(); ++i)
                links[i][id] = links[i][links.size()-1];
            links[id].swap(links[links.size()-1]); // swap with last row
            routers[routers.size()-1].setID(id); // fix id of last router
            routers[id] = routers[routers.size()-1]; // fix routers list
            // now we can delete the last router
        }
        links.pop_back(); // delete last row
        for (size_t i = 0; i < links.size(); ++i) // delete last column
            links[i].pop_back(); 
        routers.pop_back(); // delete the router itself
        // copy link table back to each remaining router
        // (necessary because DV algorithm overwrites the original links)
        for (size_t i = 0; i < routers.size(); ++i) {
            routers[i].setTable(links); // reset routing table
            routers[i].setOrigins(); // reset DV origins
        }
        // calculate new distance vectors
        if (routers.size() > 0)
            distanceVector(0); 
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: distanceVector
    DESCRIPTION: Calculates the distance vector from a source router and
    updates its neighbours recursively. Called whenever a new link is added
    (addLink function) Used with distanceVector function in Router class.
    PARAMETERS:
        id int : source router's ID on the network
    RETURNS: VOID

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
void Network::distanceVector(int id) {
    std::vector<int> oldDV = routers[id].getDV(); // get old DV
    std::vector<int> dv = routers[id].distanceVector(); // calculate new DV
    if (oldDV == dv) // no change needed
        return;
   
    for (size_t i = 0; i < dv.size(); ++i) { // for each router
        routers[i].updateTable(id, dv); // update its table
        if (i != id && links[id][i] < INFTY) // if it is a neighbour
            distanceVector(i); // pass on to its neighbours
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: printRoutingTables
    DESCRIPTION: Outputs all of the distance vectors and routing tables.
    RETURNS: VOID

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
void Network::printRoutingTables() {
    for (size_t i = 0; i < routers.size(); ++i) {
        std::vector<int> dv = routers[i].getDV(); // distance vector
        std::vector<int> origins = routers[i].getOrigins(); // origins
        // print distance vectors with origins
        std::cout << i << ": " << routers[i].getName() << "\n";
        std::cout << "----------------------------------------\n";
        for (int j = 0; j < dv.size(); ++j) {
            std::cout << dv[j] << " FROM: " << routers[origins[j]].getName() 
            << " (" << origins[j] << ")\n";
        }
        // print full routing table
        routers[i].printTable();
        std::cout << "\n";
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: printRouters
    DESCRIPTION: Outputs all of the routers on the network.
    RETURNS: VOID

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
void Network::printRouters() {
    for (size_t i = 0; i < routers.size(); ++i)
        std::cout << i << ": " << routers[i].toString() << "\n";
}

/*  ---------------------------------------------------------------------------
    FUNCTION: printLinks
    DESCRIPTION: Outputs all of the actual links (edges) between routers.
    RETURNS: VOID

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
void Network::printLinks() {
    for (size_t i = 0; i < links.size(); ++i) {
        for (size_t j = 0; j < links[i].size(); ++j) {
            if (links[i][j] == INFTY)
                std::cout << "- ";
            else
                std::cout << links[i][j] << " "; 
        }
        std::cout << "\n";
    }
}