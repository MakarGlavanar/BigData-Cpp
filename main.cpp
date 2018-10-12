#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const size_t chunkSize = 1024; // Предварительное оглашение чанка
const string outputFilename = "MULLIKEN_CHARGES.log"; // Название файла вывода
bool searchSuccess = false; // Булеан, проверяющий код на завершение с успехом

string bufferToString(char *buffer) // Функция, переводящая чанк в строчный формат
{
    string shit = "";
    for (int i = 0; i < chunkSize; i++)
    {
        shit += buffer[i];
    }
    return shit;
}

string getTableRowData(string buffer, int rowIndex) // Функция, получающая значение третьего ряда значений
{
    stringstream ss(buffer); // Подача буфера в поток
    string line;
    string resultString = "";
    while (getline(ss, line, '\n')) // Перебор всех строк
    {
        istringstream iss(line); // Подача текущей строки в поток
        vector<string> rows((istream_iterator<string>(iss)), istream_iterator<string>()); // Подача количества рядов в вектор для дальнейшего обращения
        int k = 0;
        for (auto i = rows.begin(); i != rows.end(); ++i, ++k) // Выбор нужного ряда и запись его в файл
        {
            if (k == rowIndex) 
                resultString += *i + "\n";
        }
    }
    return resultString;
}

void writeToFile(string fileName, string buffer) // Запись текущего буфера в файл
{
    ofstream file(fileName);
    file << buffer;
    file.close();
}

string readBufferFromFile(ifstream *file) // Чтение текущего буфера из файла
{
    char *buffer = new char[chunkSize];
    file -> read(buffer, chunkSize);
    string stringifiedBuffer = bufferToString(buffer);
    delete[] buffer;
    return stringifiedBuffer;
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

    char *buffer;
    char *previousChunk;

    while (file)
    {
        buffer = new char[chunkSize]; // Создание буфера
        file.read(buffer, chunkSize); // Чтение первого чанка из файла и добавление в буфер
        // ---Основной функционал---
        if (previousChunk != NULL) 
        {
            string concatinatedBuffer = bufferToString(previousChunk) + bufferToString(buffer);
            size_t index = concatinatedBuffer.find("Mulliken charges and spin densities:"); // Длина строки - 36
            if (index != -1) // Если найдена нужная строка
            {
                delete buffer; // Чистка памяти
                concatinatedBuffer = concatinatedBuffer.substr(index + 36); // Получение нужных данных из общего буффера

                while (file) // До конца файла или нахождения нужной строки
                {
                    string stringifiedBuffer = readBufferFromFile(&file); // чтение буфера и перевод в строку

                    size_t endOfDataIndex = stringifiedBuffer.find("Sum of Mulliken charges"); // Строка-индикатор конца таблицы
                    if (endOfDataIndex != -1)
                    {
                        concatinatedBuffer += stringifiedBuffer.substr(0, endOfDataIndex);  // Чтение таблицы
                        string rowData = getTableRowData(concatinatedBuffer, 2); // Получение третьего ряда
                        writeToFile(outputFilename, rowData); // Запись в файл
                        cout << "Search is done. File " + outputFilename + " is generated. " << endl; // Конец
                        searchSuccess = true;
                        break;
                    }
                    else
                    {
                        concatinatedBuffer += stringifiedBuffer; // Добавление строки в общий буфер
                    }
                }
                break;
            }

            delete previousChunk; // Чистка памяти
        }

        previousChunk = buffer; // Присвоение предыдущему чанку значение буфера
    }

    file.close(); // Закрытие файла

    if (previousChunk) // Если предыдущий чанк пуст, чистим память
        delete previousChunk;

    if (!searchSuccess) // Выброс ошибки в случае ошибки программы или файлы
        cout << "Search complete. Mullikens not found. Check file content manually." << endl;
    return 0;
}
