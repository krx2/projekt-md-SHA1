#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Funkcja hashuj¹ca
vector <uint32_t> sha1(string message);
//Funkcja dopasowywuj¹ca wiadomoœæ do algorytmu SHA-1
vector <uint8_t> padding(string message);
// Funkcja wykonuj¹ca kompresjê dla 512-bitowych cz¹stek wiadomoœci
void chunk_operations(const vector <uint8_t>& chunk, vector <uint32_t>& h);

int main() {
    setlocale(LC_CTYPE, ".1250");
    system("chcp 1250");
    system("cls");

    cout << "Witaj w programie hashuj¹cym!" << endl;
    cout << "Podaj wiadomoœæ do zakodowania:" << endl;
    string message;
    getline(cin, message);

    vector <uint32_t> result = sha1(message);

    cout << "Wynik kompresji przez SHA1 to:" << endl;
    for (uint32_t value : result) {
        cout << hex << value;
    }
    cout << endl;

    return 0;
}

// Obrót bitowt, circular shift, potrzebne póŸniej
uint32_t leftRotate(uint32_t value, uint32_t bits) {
    return (value << bits) | (value >> (32 - bits));
}

vector <uint8_t> padding(string message) {

    // Ka¿dy znak zapisany jako zawsze 8-bitowy integer czyli dwa znaki w hexadecimal
    vector <uint8_t> data(message.begin(), message.end());

    uint64_t message_size = data.size();

    cout << "Wiadomoœæ w reprezentacji szesnastkowej: ";
    for (int i = 0; i < message_size; i++) {
        cout << hex << int(data[i]) << " ";
    }
    cout << endl;

    // "Padding" wiadomoœci

    // Dodanie 1 na koniec, w tym przypadku bitów 1000
    // W teorii powinien byæ sam bit 1, ale operujemy na znakach, a nie na samych bitach
    data.push_back(0x80);

    // Dopisanie zer tak aby data.size mod 512 == 448
    // Tak ¿eby zosta³o miejsce na 64-bitowy integer z d³ugoœci¹ oryginalnej wiadomoœci
    while (data.size() % 64 != 56) {
        data.push_back(0x00); // Dodajemy po 4 zera
    }

    // Zapisanie d³ugoœci oryginalnej wiadomoœci na ostatnich 64 bitach
    uint64_t original_length = message_size * 8; // iloœæ znaków, po 8 bitów ka¿dy
    for (int i = 7; i >= 0; --i) {
        data.push_back((original_length >> (i * 8)) & 0xFF); // Dodawanie po 8 bitów do data
    }

    return data;

}

void chunk_operations(const vector <uint8_t>& chunk, vector <uint32_t>& h) {
    uint32_t w[80] = { 0 };

    // Podzia³ 64 uint8_t na 16 uint32_t
    for (int i = 0; i < 16; i++) {
        w[i] = (chunk[i * 4 + 0] << 24) |
            (chunk[i * 4 + 1] << 16) |
            (chunk[i * 4 + 2] << 8) |
            (chunk[i * 4 + 3]);
    }

    //Rozszerzenie do 80 uint32_t
    for (int i = 16; i < 80; i++) {
        w[i] = leftRotate((w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]), 1);
    }

    // Sczytanie wartoœci h0-h4
    uint32_t a = h[0];
    uint32_t b = h[1];
    uint32_t c = h[2];
    uint32_t d = h[3];
    uint32_t e = h[4];

    // G³ówna Pêtla
    for (int i = 0; i < 80; i++) {
        uint32_t f, k;
        if (i < 20) {
            f = (b & c) | ((~ b) & d);
            k = 0x5A827999;
        }
        else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        uint32_t temp = (leftRotate(a, 5)) + f + e + k + w[i];
        e = d;
        d = c;
        c = leftRotate(b, 30);
        b = a;
        a = temp;
    }

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}

vector <uint32_t> sha1(string message) {

    cout << "Przetwarzanie tekstu \"" << message <<"\"..." << endl;

    vector <uint8_t> data = padding(message);

    // Inicjalizacja pocz¹tkowych wartoœci
    vector <uint32_t> h = {
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    };


    for (size_t i = 0; i < data.size(); i += 64) {
        vector <uint8_t> chunk(data.begin() + i, data.begin() + i + 64);
        chunk_operations(chunk, h);
    }

    return { h[0], h[1], h[2], h[3], h[4] };
}
