#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

void writeData () {
    ofstream fileptr;
    fileptr.open ("generated-input.txt");

    if (!fileptr) {
        cout << "file cannot be opened";
        exit(1);
    }

    srand(time(NULL));

    int tableDeckCount = rand() % 1001;
    int playerDeckCount = rand() % 1001;

    fileptr << tableDeckCount << " " << playerDeckCount << endl;

    for (int i = 0; i < tableDeckCount; i++) {
        fileptr << (rand() % 1000000) - (rand() % 1000000) << endl;
    }

    for (int i = 0; i < playerDeckCount; i++) {
        fileptr << (rand() % 1000000) - (rand() % 1000000) << endl;
    }

    for (int i = 0; i < playerDeckCount; i++) {
        fileptr << (rand() % 1000000) - (rand() % 1000000) << endl;
    }

    fileptr.close();

};
int main() {
    writeData();
    return 0;
}