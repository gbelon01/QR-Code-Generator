#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

/*
*  Block so we can understand what type of data we're dealing with. This is important
*  because wen converting it into data for the qr, different types can be arranged
*  with more or less bytes per character. So finding the most efficient type can make
*  a more efficient QR as well.
*
*     MODE    |    PACKING    |    BITS P/ CHAR
* Numeric      3 digits -> 10 bits          3.33
* Alphanumeric 2 chars -> 11 bits           5.50
* Byte         1 char -> 8 bits             8.00
*/

enum Mode{
    Numeric = 0x1, 
    Alphanumeric = 0x2, 
    Byte = 0x4
};
/*An enum class type allows us to display values as strings
* So in this case Num = 0001, Alphnum = 0010 and Byte = 0100
* Mode is the class that we'll use, so we use Mode::Numeric for eg
*/

const string ALPHNUM = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";
//All possible chars in alphnum

bool isNumeric(char c);
//checks in their ASCII range '0' = 48/'9' = 57
bool isAlphanumeric(char c);
//looks for the char in the string alphnum. If its there, great, if not the bool has no value
//which is the npos (No position) string, because if it was 0 we would have it always being true

Mode setMode(const string& s);

void printMode(Mode mode);

struct CharInfo{
    int index;
    string characters;
    vector<int> bits;
};

vector<int> toBits(int value, int size);

vector<CharInfo> makeCharTable(Mode mode, const string& s);

void printCharTable(vector<CharInfo> table);

enum EccLevel{L, M, Q, H}; //used for the codeoworks array

EccLevel askLevel();

int versionFitter(Mode mode, const vector<CharInfo>& table, EccLevel level);

vector<int> assembleStream(Mode mode, const string& s, const vector<CharInfo>& table, EccLevel level, int version);