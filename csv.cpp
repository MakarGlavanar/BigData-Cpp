#include "csv.hpp"

CSVFile::CSVFile(string fileName, size_t columnsCount, string columns[]) {
    this -> columnsCount = columnsCount;
    this -> columns = columns;
    this -> file = ofstream(fileName, ios::out);

    if (this -> file.is_open()) {
        for (size_t i = 0; i < columnsCount - 1; i++) {
            this -> file << columns[i] + ",";
        }
        this -> file << columns[columnsCount - 1];
    }
}

void CSVFile::writeRow(string *values) {
    string row = "";
    for (int i = 0; i < this -> columnsCount - 1; ++i)
        row += values[i] + ",";
    row += values[this -> columnsCount - 1] + "\n";
    this -> file << row;
}

void CSVFile::closeCsv() {
    this -> file.close();
}