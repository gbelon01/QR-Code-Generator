#include "QRC.hpp"

int main(){
    string stringEncript;
    vector<char> stringsChars;
    
    cout << "String to QR Code:\n";
    cin >> stringEncript;
    EccLevel level = askLevel();
    

    Mode mode = setMode(stringEncript);
    printMode(mode);

    vector<CharInfo> charTable = makeCharTable(mode, stringEncript);
    printCharTable(charTable);

    int version = versionFitter(mode, makeCharTable(mode, stringEncript), level);
    cout << "Chosen version number: " << version << "\n";

    vector<int> encriptedInts = assembleStream(mode, stringEncript, charTable, level, version);
    for(int i = 0; i < static_cast<int>(encriptedInts.size()); i++){
        cout << encriptedInts[i] << " ";
    }


}