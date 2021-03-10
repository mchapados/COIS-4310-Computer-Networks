/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3
    FILENAME: router.cpp

    Method implementations for Router class
    
    WRITTEN BY: S. Chapados - March, 2021
   ------------------------------------------------------------------------- */

#include "router.h"
#include <regex>
#include <iostream>
using namespace std;

// Constructor
Router::Router(string ip, string n) {
    address = "NULL";
    setAddress(ip);
    name = n;
    table = vector< vector<int> >();
 }

// address setter
void Router::setAddress(string ip) {
    // check that ip address is correct format
    if (regex_match(ip, regex("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}")))
        address = ip;
    else {
        cout << "Invalid IP Address\n";
        // address does not change if invalid
    }
}

void Router::updateTable(int row, int col, int val) {
    if (row < table.size()) {
        if (col < table.at(row).size())
            table.at(row).at(col) = val;
        else
            table.at(row).push_back(val);
    }
}

void Router::updateTable(int row, vector<int> val) {
    if (row < table.size())
        table.at(row) = val;
    else
        table.push_back(val);
}

void Router::printTable() {
    cout << "\n";
    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table.at(i).size(); ++j) { 
            cout << table.at(i).at(j) << " ";
        }
        cout << "\n";
    }
}

string Router::toString() {
    return "ID: " + to_string(id) + "\nName: " + name + "\nAddress: " + address;
}