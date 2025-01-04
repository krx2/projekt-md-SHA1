#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Funkcja hashuj�ca
string sha1(string message);

int main()
{
    // Obs�uga polskich znak�w
    setlocale(LC_CTYPE, ".1250");
    system("chcp 1250");
    system("cls");

    cout << "Witaj w programie hashuj�cym!" << endl;
    cout << "Podaj wiadomo�� do zakodowania:" << endl;
    string message;
    getline(cin, message);
    string result = sha1(message);
    cout << "Wynik kompresji przez SHA1 to:" << endl;
    cout << result;

    return 0;
}

string sha1(string message) {

    cout << "Przetwarzanie tekstu \"" << message <<"...\"" << endl;

    // Ka�dy znak zapisany jako zawsze 8-bitowy integer czyli dwa znaki w hexadecimal
    vector<uint8_t> data(message.begin(), message.end());

    uint64_t message_size = data.size();

    cout << "Wiadomo�� w reprezentacji szesnastkowej: ";
    for (int i = 0; i < message_size; i++) {
        cout << hex << int(data[i]) << " ";
    }
    cout << endl;

    // "Padding" wiadomo�ci
    
    // Dodanie 1 na koniec, w tym przypadku bit�w 1000
    // W teorii powinien by� sam bit 1, ale operujemy na znakach, a nie na samych bitach
    data.push_back(0x80);

    // Dopisanie zer tak aby data.size mod 512 == 448
    // Tak �eby zosta�o miejsce na 64-bitowy integer z d�ugo�ci� oryginalnej wiadomo�ci
    while (data.size() % 64 != 56) {
        data.push_back(0x00); // Dodajemy po 4 zera
    }

    cout << dec << data.size() << endl;

    // Zapisanie d�ugo�ci oryginalnej wiadomo�ci na ostatnich 64 bitach
    uint64_t original_length = message_size * 8; // ilo�� znak�w, po 8 bit�w ka�dy
    for (int i = 7; i >= 0; --i) {
        data.push_back((original_length >> (i * 8)) & 0xFF); // Dodawanie po 8 bit�w do data
    }

    cout << data.size() << endl << hex;

    for (int i = 0; i < data.size(); i++) {
        cout << int(data[i]) << " ";
    }
    cout << endl;

    // Inicjalizacja pocz�tkowych warto�ci
    uint32_t h[5] = {
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    };


    return message;
}
