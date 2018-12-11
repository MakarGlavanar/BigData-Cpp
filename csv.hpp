#ifndef CSV
#define CSV

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

class CSVFile {

public: 
    CSVFile(string, size_t, string *);

    void writeRow(string *);

    void closeCsv();

private:
    size_t columnsCount;
    string* columns;
    ofstream file;
}; 

#endif