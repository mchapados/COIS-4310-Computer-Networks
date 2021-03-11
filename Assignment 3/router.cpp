/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3 - DISTANCE VECTOR ALGORITHM
    FILENAME: router.cpp

    Method implementations for Router class
    
    WRITTEN BY: S. Chapados - March, 2021
---------------------------------------------------------------------------- */

#include "router.h"
#include <regex>
#include <iostream>
#define INFTY 541196290
using namespace std;

// Constructor
Router::Router(string ip, string n) {
    address = "NULL";
    setAddress(ip);
    name = n;
    table = vector< vector<int> >();
 }

/*  ---------------------------------------------------------------------------
    FUNCTION: setAddress
    DESCRIPTION: Checks that the input is a valid IP; if so, sets it to the
    address field. Otherwise, address does not change.
    RETURNS: VOID

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
void Router::setAddress(string ip) {
    // check that ip address is correct format
    if (regex_match(ip, regex("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
        address = ip;
    else {
        cout << "Invalid IP Address\n";
        // address does not change if invalid
    }
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

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
void Router::updateTable(int row, int col, int val) {
    if (row < table.size()) {
        if (col < table.at(row).size())
            table.at(row).at(col) = val;
        else
            table.at(row).push_back(val);
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
void Router::updateTable(int row, vector<int> val) {
    if (row < table.size())
        table.at(row) = val;
    else
        table.push_back(val);
}

/*  ---------------------------------------------------------------------------
    FUNCTION: distanceVector
    DESCRIPTION: 
    RETURNS: vector<int>

    Last Updated: Mar 10, 2021
---------------------------------------------------------------------------  */
vector<int> Router::distanceVector() {
    dv = table.at(id); // initial estimates
    for (int i = 0; i < dv.size(); ++i) {
        if (i != id) { // don't need to update distance to self
            for (int j = 0; j < dv.size(); ++j)
                dv.at(i) = min(dv.at(i), dv.at(j) + table.at(j).at(i));
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
    cout << "\n";
    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table.at(i).size(); ++j) { 
            if (table.at(i).at(j) == INFTY)
                cout << "- ";
            else
                cout << table.at(i).at(j) << " ";
        }
        cout << "\n";
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: printDistanceVector
    DESCRIPTION: Outputs the routing table to the screen.
    RETURNS: VOID

    Last Updated: Mar 10, 2021
---------------------------------------------------------------------------  */
void Router::printDistanceVector() {
    cout << "\n\n" << name << " (ID: " << id << ")\n";
    for (int i = 0; i < dv.size(); ++i) {
        if (dv.at(i) == INFTY)
            cout << "- ";
        else
            cout << dv.at(i) << " ";
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: toString
    DESCRIPTION: Returns the router's ID, name, and address as a string.
    RETURNS: string

    Last Updated: Mar 9, 2021
---------------------------------------------------------------------------  */
string Router::toString() {
    return "ID: " + to_string(id) + "\nName: " + name + "\nAddress: " + address;
}