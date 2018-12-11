#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "file_reader.hpp"
#include "csv.hpp"

using namespace std;

const size_t chunkSize = 1024;
bool searchSuccess = false;

vector<vector<string>> getTableData(string buffer, int rowIndex)
{
    stringstream ss(buffer);
    string line;
    vector<vector<string>> table;
    while (getline(ss, line, '\n'))
    {
        istringstream iss(line);
        vector<string> columns((istream_iterator<string>(iss)), istream_iterator<string>());
        table.push_back(columns);
    }
    return table;
}

int main(int argc, char **argv)
{
    string fileName(argv[1]);
    string outputFileName(argv[2]);
    outputFileName += ".csv";
    cout << "Opening " + fileName + "..." << endl;

    ifstream file(fileName);

    if (!file.good())
    {
        cout << "Error opening file. Check if filename is correct." << endl;
        return 0;
    }
    cout << "Successfully opened file. Searching now..." << endl;

    string tableStartExpression = "Mulliken charges and spin densities:";
    string tableEndExpression = "Sum of Mulliken charges";
    string buffer = "";
    string previousBuffer = "";

    while (file)
    {
        buffer = readStringFromFile(&file, chunkSize);
        if (!previousBuffer.empty()) 
        {
            string concatinatedBuffer = previousBuffer + buffer;
            size_t index = concatinatedBuffer.find(tableStartExpression);
            if (index != -1)
            {
                concatinatedBuffer = concatinatedBuffer.substr(index + tableStartExpression.length());

                while (file)
                {
                    buffer = readStringFromFile(&file, chunkSize);

                    size_t endOfDataIndex = buffer.find(tableEndExpression); 
                    if (endOfDataIndex != -1)
                    {
                        concatinatedBuffer += buffer.substr(0, endOfDataIndex); 
                        auto tableData = getTableData(concatinatedBuffer, 2); 
                        string columns[] = { "Index", "Atom", "Spin1", "Spin2" };
                        CSVFile output(outputFileName, 4, columns);
                        auto row = tableData.begin(); ++(++row);
                        auto end = tableData.end(); --end;
                        for (; row != end; ++row) {
                            string *values = new string[(*row).size()];
                            int index = 0;
                            for (auto value = (*row).begin(); value != (*row).end(); ++value, ++index) {
                                values[index] = (*value);
                            }
                            output.writeRow(values);
                        }
                        output.closeCsv();
                        cout << "Search is done. File " + outputFileName + " is generated. " << endl;
                        searchSuccess = true;
                        break;
                    }
                    else
                    {
                        concatinatedBuffer += buffer;
                    }
                }
                break;
            }
        }
        previousBuffer = buffer;
    }
    file.close();
    if (!searchSuccess)
        cout << "Search complete. Mullikens not found. Check file content manually." << endl;
    return 0;
}
