#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/MemoryStream.h"

#include <iostream>

using namespace std;

char out[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                   0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

int main() {
    Poco::FileInputStream fis("file.txt", ios::binary);
    Poco::MemoryInputStream sink(out, 25);

    char fisS[20], fis1S[20];
    fis.read(fisS, sizeof(uint8_t)*20);
    sink.read(fis1S, sizeof(uint8_t)*20);
    for (uint8_t i = 0; i < 20; i++) {
        cout << to_string(fisS[i]) << "   " << to_string(fis1S[i]) << endl;
    }
    // if(fisS == fis1S)
    //     cout << "hello world" << endl;
    return 0;
}