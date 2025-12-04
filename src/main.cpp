#include <iostream>
#include <string>
using namespace std;

int main() {
    char buffer[256];
    cout << "Introdu comanda: ";
    cin.getline(buffer, 256);

    string vectorCuvinte[20];
    int nrCuvinte = 0;
    string currentWord = "";

    for (int i = 0; i < 256; i++) {
        char c = buffer[i];

        if (c == '\0') {
            if (!currentWord.empty()) {
                vectorCuvinte[nrCuvinte++] = currentWord;
            }
            break;
        }

        if (c == ' ') {
            if (!currentWord.empty()) {
                vectorCuvinte[nrCuvinte++] = currentWord;
                currentWord.clear();
            }
        }
        else {
            currentWord += c;
        }
    }

    for (int i = 0; i < nrCuvinte; i++) {
        cout << "[" << vectorCuvinte[i] << "]\n";
    }
}
