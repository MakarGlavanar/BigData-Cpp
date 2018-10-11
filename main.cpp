#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const size_t chunkSize = 1024;
const string outputFilename = "MULLIKEN_CHARGES.log";
bool searchSuccess = false;

string bufferToString(char *buffer) {
    string shit = "";
    for (int i = 0; i < chunkSize; i++) {
        shit += buffer[i];
    }
    return shit;
}

string getTableRowData(string buffer, int rowIndex) {
    stringstream ss(buffer);
    string line;
    string resultString = "";
    while (getline(ss, line, '\n')) {
        istringstream iss(line);
        vector<string> rows((istream_iterator<string>(iss)), istream_iterator<string>());
        int k = 0; 
        for (auto i = rows.begin(); i != rows.end(); ++i, ++k) {
            if (k == rowIndex)
                resultString += *i + "\n";
        }
    }
    return resultString;
}

void writeToFile(string fileName, string buffer) {
    ofstream file(fileName);
    file << buffer;
    file.close();
}

int main(int argc, char** argv) {
    string fileName(argv[1]);
    cout << "Opening " + fileName + "..." << endl;

    ifstream file(fileName);

    if (!file.good()) {
        cout << "Error opening file. Check if filename is correct." << endl;
        return 0;
    }
    cout << "Successfully opened file. Searching now..." << endl;

    char *buffer;
    char *previousChunk;

    while (file)
    {
        buffer = new char[chunkSize];
        file.read(buffer, chunkSize);
        if (previousChunk != NULL)
        {
            string concatinatedBuffer = bufferToString(previousChunk) + bufferToString(buffer);
            size_t index = concatinatedBuffer.find("Mulliken charges and spin densities:"); // 36 length
            if (index != -1) {
                delete buffer;
                concatinatedBuffer = concatinatedBuffer.substr(index + 36);

                while (file) {
                    buffer = new char[chunkSize];
                    file.read(buffer, chunkSize);
                    string stringifiedBuffer = bufferToString(buffer);
                    delete buffer;

                    size_t endOfDataIndex = stringifiedBuffer.find("Sum of Mulliken charges");
                    if (endOfDataIndex != -1) {
                        concatinatedBuffer += stringifiedBuffer.substr(0, endOfDataIndex);
                        string rowData = getTableRowData(concatinatedBuffer, 2);
                        writeToFile(outputFilename, rowData);
                        cout << "Search is done. File " + outputFilename + " is generated. " << endl;
                        searchSuccess = true;
                        break;
                    } else {
                        concatinatedBuffer += stringifiedBuffer;
                    }          
                }
                break;
            }

            delete previousChunk;
        }

        previousChunk = buffer;
    }

    file.close();

    if (previousChunk)
        delete previousChunk;

    if (!searchSuccess)
        cout << "Search complete. Mullikens not found. Check file content manually." << endl;
    return 0;
}
