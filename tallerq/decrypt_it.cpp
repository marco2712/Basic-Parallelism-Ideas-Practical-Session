#include <iostream>
#include <string>
#include <cctype>
using namespace std;

string decrypt(const string &text, int k) {
    string res;
    for (char c : text) {
        if (isalpha(static_cast<unsigned char>(c))) {
            char base = isupper(c) ? 'A' : 'a';
            res += char(((c - base - k) % 26 + 26) % 26 + base);
        } else {
            res += c;
        }
    }
    return res;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <rotacion> \"texto\"\n";
        return 1;
    }
    int k = stoi(argv[1]);
    string text = argv[2];
    cout << decrypt(text, k) << endl;
    return 0;
}
