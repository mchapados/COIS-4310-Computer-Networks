/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #3
    FILENAME: router.h

    Header file for Router (vertex) class.
    
    WRITTEN BY: S. Chapados - March, 2021
   ------------------------------------------------------------------------- */

#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <vector>
using namespace std;

class Router {
    private:
        string address; // ip address of the router
        string name;
        int id; // assigned when adding router to a network
        vector< vector<int> > table;
    public:
        Router(string ip, string n); // constructor
        ~Router() {} // destructor
        // getters and setters
        string getAddress() { return address; }
        void setAddress(string ip);
        string getName() { return name; }
        void setName(string n) { name = n; }
        vector< vector<int> > getTable() { return table; }
        void setTable(vector< vector<int> > newTable) { table = newTable; }
        int getID() { return id; }
        void setID(int newID) { id = newID; }

        void updateTable(int row, int col, int val);
        void updateTable(int row, vector<int> val);
        void printTable();
        string toString();
};

#endif