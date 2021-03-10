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