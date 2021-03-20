/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3 - DISTANCE VECTOR ALGORITHM
    FILENAME: router.cpp

    Method implementations for Router class
    
    WRITTEN BY: S. Chapados - March, 2021
---------------------------------------------------------------------------- */

#include "router.hpp"
//#include <regex> // to check if a string is an IP address
#include <iostream>
#define INFTY 541196290 // very large number to represent non-existent links

// Constructor
Router::Router(std::string ip, std::string n) {
    address = "NULL";
    setAddress(ip);
    name = n;
 }

/*  ---------------------------------------------------------------------------
    FUNCTION: setAddress
    DESCRIPTION: Checks that the input is a valid IP; if so, sets it to the
    address field. Otherwise, address does not change.

    Validation commented out because regex doesn't work on Loki :(
    This is more of a nice-to-have anyway.

    RETURNS: VOID

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
void Router::setAddress(std::string ip) {
    // check that ip address is correct format
    //if (std::regex_match(ip, 
    //std::regex("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
        address = ip;
    //else {
    //    std::cout << "Invalid IP Address\n";
        // address does not change if invalid
    //}
}

/*  ---------------------------------------------------------------------------
    FUNCTION: updateTable (v1)
    DESCRIPTION: Adds or changes a value in the routing table. Does nothing if
    the row is not in the table, but can add new columns.
    PARAMETERS:
        row int : row index
        col int : column index
        val int : new value to be added or updated
    RETURNS: VOID

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
void Router::updateTable(int row, int col, int val) {
    if (row < table.size()) {
        if (col < table[row].size()) {
            table[row][col] = val;
            // check if DV needs updating 
            // (if this is a faster route than what we already have)
            if (row == id && dv.size() > col && 
            table[row][col] <= dv[col]) {
                dv[col] = val;
                origins[col] = id;
            }
        }
        else {
            table[row].push_back(val);
        }
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: updateTable (v2)
    DESCRIPTION: Adds or changes a specified row in the routing table.
    PARAMETERS:
        row int : index of the row to be updated or added
        val vector<int> : contents of the row
    RETURNS: VOID

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
void Router::updateTable(int row, std::vector<int> val) {
    if (row < table.size())
        table[row] = val;
    else
        table.push_back(val);
}

/*  ---------------------------------------------------------------------------
    FUNCTION: distanceVector
    DESCRIPTION: Calculates (or re-calculates) a router's distance vector
    (shortest paths to other routers).
    RETURNS: vector<int>

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
std::vector<int> Router::distanceVector() {
    dv = table[id]; // initial estimates
    // initialize origins vector (default origin should be self)
    if (origins.size() < dv.size()) {
        for (size_t i = origins.size(); i < dv.size(); ++i)
            origins.push_back(id);
    }
    int min_distance; // minimum distance from source to a destination
    for (size_t i = 0; i < dv.size(); ++i) {
        if (i != id) { // don't need to update distance to self
            for (int j = 0; j < dv.size(); ++j) {
                min_distance = std::min(dv[i], dv[j] + table[j][i]);
                if (min_distance != dv[i]) // origin needs updating
                    origins[i] = j;
                dv[i] = min_distance;
            }
        }
    }
    return dv;
}

/*  ---------------------------------------------------------------------------
    FUNCTION: printTable
    DESCRIPTION: Outputs the routing table to the screen.
    RETURNS: VOID

    Last Updated: Mar 10, 2021
---------------------------------------------------------------------------  */
void Router::printTable() {
    std::cout << "\n";
    for (size_t i = 0; i < table.size(); ++i) {
        for (size_t j = 0; j < table[i].size(); ++j) { 
            if (table[i][j] == INFTY)
                std::cout << "- ";
            else
                std::cout << table[i][j] << " ";
        }
        std::cout << "\n";
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: toString
    DESCRIPTION: Returns the router's name and address as a string.
    RETURNS: string

    Last Updated: Mar 19, 2021
---------------------------------------------------------------------------  */
std::string Router::toString() {
    return name + " (" + address + ")";
}