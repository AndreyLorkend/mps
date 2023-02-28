#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

HANDLE hSerial;
#define N 16

void sendData();
void readCOM();

int main(int argc, char* argv[]) {
	LPCTSTR sPortName = "COM2";
	hSerial = CreateFile(sPortName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            cout << "serial port does not exist.\n";
        }
        cout << "some other error occurred.\n";
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        cout << "getting state error\n";
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        cout << "error setting serial port state\n";
    }


    while (1) {
        if (_getch()) {
            sendData();
        }
    }
    return 0;
}

void sendData() {
    srand(time(NULL));
    BYTE data[N];  // строка для передачи
    for (BYTE i = 0; i < N; i++) {
        data[i] = rand() % 255;
        cout << static_cast<int>(data[i]) << " ";
    }
    cout << "\n";
    DWORD dwSize = sizeof(data);   // размер этой строки
    DWORD dwBytesWritten;    // тут будет количество собственно переданных байт

    BOOL iRet = WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL);

    //for (BYTE i = 0; i < N; i++) {
    //    WriteFile(hSerial, &data[i], 1, &dwBytesWritten, NULL);
    //    Sleep(500);
    //}

    cout << dwSize << " Bytes in string. " << dwBytesWritten << " Bytes sended. " << endl;
}
