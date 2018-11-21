#ifndef FILE_READER
#define FILE_READER

#include <fstream>
#include <string>
using namespace std;

string readStringFromFile(ifstream *, size_t);

void writeStringToFile(ofstream *, string);

#endif