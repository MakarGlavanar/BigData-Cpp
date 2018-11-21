#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "file_reader.hpp"

using namespace std;

const size_t chunkSize = 1024; // Предварительное оглашение чанка
const string outputFilename = "MULLIKEN_CHARGES.log"; // Название файла вывода
bool searchSuccess = false; // Булеан, проверяющий код на завершение с успехом

string getTableRowData(string buffer, int rowIndex)
{
    stringstream ss(buffer);
    string line;
    string resultString = "";
    while (getline(ss, line, '\n'))
    {
        istringstream iss(line);
        vector<string> rows((istream_iterator<string>(iss)), istream_iterator<string>());
        int k = 0;
        for (auto i = rows.begin(); i != rows.end(); ++i, ++k)
        {
            if (k == rowIndex) 
                resultString += *i + "\n";
        }
    }
    return resultString;
}

int main(int argc, char **argv)
{
    string fileName(argv[1]);
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
                        string rowData = getTableRowData(concatinatedBuffer, 2); 
                        // writeStringToFile(, rowData); 
                        cout << "Search is done. File " + outputFilename + " is generated. " << endl;
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
