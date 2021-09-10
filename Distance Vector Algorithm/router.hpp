/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3 - DISTANCE VECTOR ALGORITHM
    FILENAME: router.hpp

    Header file for Router (vertex) class.
    
    WRITTEN BY: S. Chapados - March, 2021
---------------------------------------------------------------------------- */

#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <vector>

class Router {
    private:
        std::string address; // ip address of the router
        std::string name;
        int id; // assigned when adding router to a network
        std::vector< std::vector<int> > table; // complete routing table
        std::vector<int> dv; // shortest paths
        std::vector<int> origins; // origins for shortest paths
    public:
        Router(std::string ip, std::string n); // constructor
        ~Router() {} // destructor
        // getters and setters
        std::string getAddress() { return address; }
        void setAddress(std::string ip); // see implementation file
        std::string getName() { return name; }
        void setName(std::string n) { name = n; }
        std::vector< std::vector<int> > getTable() { return table; }
        void setTable(std::vector< std::vector<int> > newTable) { table = newTable; }
        int getID() { return id; }
        void setID(int newID) { id = newID; }
        std::vector<int> getDV() { return dv; }
        std::vector<int> getOrigins() { return origins; }
        void setOrigins() { origins = std::vector<int>(); } // resets defaults
        // PUBLIC FUNCTIONS ---------------------------------------------------
        void updateTable(int row, int col, int val);
        void updateTable(int row, std::vector<int> val);
        std::vector<int> distanceVector();
        void printTable();
        void printDistanceVector();
        std::string toString();
};

#endif