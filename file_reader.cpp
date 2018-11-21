#include "file_reader.hpp";

string bufferToString(char *buffer, size_t chunkSize)
{
    string temp = "";
    for (int i = 0; i < chunkSize; i++)
    {
        temp += buffer[i];
    }
    return temp;
}

string readStringFromFile(ifstream *file, size_t chunkSize) {
    char *buffer = new char[chunkSize];
    file -> read(buffer, chunkSize);
    string stringifiedBuffer = bufferToString(buffer, chunkSize);
    delete[] buffer;
    return stringifiedBuffer;
}

void writeStringToFile(ofstream *file, string content) {
   *file << content;
}

