#include "csv.hpp"

CSVFile::CSVFile(string fileName, size_t columnsCount, string* columns) {
    this -> columnsCount = columnsCount;
    this -> columns = columns;
    this -> file = ofstream(fileName);
}

void CSVFile::writeRow(string* values) {
    string row = "";
    for (int i = 0; i < columnsCount - 1; i++)
        row += values[i] + ",";
    row += values[columnsCount - 1];

    this -> file << row;
}

void CSVFile::closeCsv() {
    this -> file.close();
}