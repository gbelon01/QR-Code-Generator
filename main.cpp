#include "QRC.hpp"

int main(){
    initTables();
    string stringEncript;
    vector<char> stringsChars;
    
    cout << "String to QR Code:\n";
    getline(cin, stringEncript);
    EccLevel level = askLevel();
    

    Mode mode = setMode(stringEncript);
    printMode(mode);

    vector<CharInfo> charTable = makeCharTable(mode, stringEncript);
    printCharTable(charTable);

    int version = versionFitter(mode, makeCharTable(mode, stringEncript), level);
    cout << "Chosen version number: " << version << "\n";

    vector<int> encriptedInts = assembleStream(mode, stringEncript, charTable, level, version);
    cout << "Codewords:\n";
    for(int i = 0; i < static_cast<int>(encriptedInts.size()); i++){
        cout << encriptedInts[i] << " ";
    }
    cout << "\n";

    vector<int> eccs = getEcc(encriptedInts, version, level);
    cout << "ECC:\n";
    for(size_t j = 0; j < eccs.size(); j++){
        cout << eccs[j] << " ";
    }
}