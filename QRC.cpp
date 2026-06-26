#include "QRC.hpp"

bool isNumeric(char c) {return c >= '0' && c<='9';};
//checks in their ASCII range '0' = 48/'9' = 57
bool isAlphanumeric(char c) {return ALPHNUM.find(c) != string::npos;};
//looks for the char in the string alphnum. If its there, great, if not the bool has no value
//which is the npos (No position) string, because if it was 0 we would have it always being true

// number of DATA codewords (bytes) available per version and ECC level.
const int DATA_CODEWORDS[40][4] = {
    {  19,   16,   13,    9 },   
    {  34,   28,   22,   16 },   
    {  55,   44,   34,   26 },   
    {  80,   64,   48,   36 },   
    { 108,   86,   62,   46 },   
    { 136,  108,   76,   60 },   
    { 156,  124,   88,   66 },   
    { 194,  154,  110,   86 },   
    { 232,  182,  132,  100 },   
    { 274,  216,  154,  122 },   
    { 324,  254,  180,  140 },   
    { 370,  290,  206,  158 },   
    { 428,  334,  244,  180 },   
    { 461,  365,  261,  197 },   
    { 523,  415,  295,  223 },   
    { 589,  453,  325,  253 },   
    { 647,  507,  367,  283 },   
    { 721,  563,  397,  313 },   
    { 795,  627,  445,  341 },   
    { 861,  669,  485,  385 },   
    { 932,  714,  512,  406 },   
    {1006,  782,  568,  442 },   
    {1094,  860,  614,  464 },   
    {1174,  914,  664,  514 },   
    {1276, 1000,  718,  538 },   
    {1370, 1062,  754,  596 },   
    {1468, 1128,  808,  628 },   
    {1531, 1193,  871,  661 },   
    {1631, 1267,  911,  701 },   
    {1735, 1373,  985,  745 },   
    {1843, 1455, 1033,  793 },   
    {1955, 1541, 1115,  845 },   
    {2071, 1631, 1171,  901 },   
    {2191, 1725, 1231,  961 },   
    {2306, 1812, 1286,  986 },   
    {2434, 1914, 1354, 1054 },   
    {2566, 1992, 1426, 1096 },   
    {2702, 2102, 1502, 1142 },   
    {2812, 2216, 1582, 1222 },   
    {2956, 2334, 1666, 1276 },   
};

Mode setMode(const string& s){
    bool allNum = true, allAlphanum = true;
    for(char c : s){ //checks c against every char in s, one at at time
        if(!isNumeric(c)) allNum = false;
        if(!isAlphanumeric(c)) allAlphanum = false;
    } //this for loop will make the check the website gave me. If its all alphanum, itll be that, so on...

    if(allNum) return Mode::Numeric; // 10bits -> 3 chars
    if(allAlphanum) return Mode::Alphanumeric; //11bits -> 2 chars

    return Mode::Byte; //8 bits p/ char
}

void printMode(Mode mode){
    cout << "Chosen segment mode to encode all characters: ";

    if(mode == Mode::Alphanumeric){cout << "Alphanumeric\n";}
    else if(mode == Mode::Numeric){cout << "Numeric\n";}
    else{cout << "Byte\n";}
}

vector<int> toBits(int dividend, int size){
    vector<int> result;
    for(int i = 0; i < size; i++){
        result.insert(result.begin(), (dividend % 2)); //array of bits, putting newest digit first
        dividend /= 2; 

    }
    return result;
}

vector<CharInfo> makeCharTable(Mode mode, const string& s){
    vector<CharInfo> table;
    int bitsSize;
    vector<int> bits;

    for(size_t i = 0; i < s.size(); i++){
        if(mode == Mode::Byte){
            int dividend = static_cast<int>(s[i]); //gets ASCII value
            bitsSize = 8;
            bits = toBits(dividend, bitsSize);

            table.push_back(CharInfo{static_cast<int>(i), s.substr(i, 1), bits});
            //static_cast is converting size_t -> int
        }
        else if(mode == Mode::Alphanumeric){
            int v1 = ALPHNUM.find(s[i]);

            if(i+1 < s.size()){
                int v2 = ALPHNUM.find(s[i+1]);
                bitsSize = 11;
                bits = toBits((v1 * 45 + v2), bitsSize);

                table.push_back(CharInfo{static_cast<int>(i), s.substr(i, 2), bits});
                i++;                
            }else{
                bitsSize = 6;
                bits = toBits(v1, bitsSize);

                table.push_back(CharInfo{static_cast<int>(i), s.substr(i, 1), bits});
            }

        }
        else{
            int remainder = s.size() - i;
            int n1 = s[i] - '0';
            
            if(remainder >= 3){
                int n2 = s[i+1] - '0';
                int n3 = s[i+2] - '0';
                bitsSize = 10;

                int operation = (n1 * 100) + (n2 * 10) + n3;
                bits = toBits(operation, bitsSize);

                table.push_back(CharInfo{static_cast<int>(i), s.substr(i, 3), bits});
                i += 2;
            }
            else if(remainder == 2){
                int n2 = s[i+1] - '0';
                bitsSize = 7;

                int operation = (n1 * 10) + n2;
                bits = toBits(operation, bitsSize);

                table.push_back(CharInfo{static_cast<int>(i), s.substr(i, 2), bits});
                i++;
            }
            else{
                bitsSize = 4;
                
                bits = toBits(n1, bitsSize);

                table.push_back(CharInfo{static_cast<int>(i), s.substr(i, 1), bits});
            }

        }

    }

    return table;
}

void printCharTable(vector<CharInfo> table){
    string bigSpace = "                   ";
    string smallSpace = "             ";

    cout << "Index     |     Character     |     Bits     \n";
    //print row
    for(const CharInfo& row : table){
        cout << row.index << bigSpace << row.characters << smallSpace;
        for(int digit : row.bits){
            cout << digit;
        }
        cout << "\n";
    }
}

EccLevel askLevel(){
    char userInput;

    while(true){ //loops untill user puts the correct input
        cout << "Error correction level (L/M/Q/H):\n";
        cin >> userInput;

        switch(userInput){
            case 'L':return EccLevel::L;
            case 'M':return EccLevel::M;
            case 'Q':return EccLevel::Q;
            case 'H':return EccLevel::H;
            default:
                cout << "Error, plese try again. Remember: Low, Medium, Quartile, High\n";
        }
    }
}   

int versionFitter(Mode mode, const vector<CharInfo>& table, EccLevel level){
    vector<int> totalBits = toBits(static_cast<int>(mode), 4);

    for(const CharInfo& row : table){
        totalBits.insert(totalBits.end(), row.bits.begin(),row.bits.end());
    }

    for(int i = 0; i < 40; i++){
        int version = i + 1;
        int countWidth;
        if(mode == Mode::Alphanumeric){countWidth = (version <= 9) ? 9 : (version <= 26) ? 11 : 13;}
        else if(mode == Mode::Numeric){countWidth = (version <= 9) ? 10 : (version <= 26) ? 12 : 14;}
        else{countWidth = (version <= 9) ? 8 : 16;}
        
        int codewords = ceil((totalBits.size() + countWidth) / 8.0);

        if(DATA_CODEWORDS[i][level] >= codewords){
            return version;
        }
    }

    return -1;
}

vector<int> assembleStream(Mode mode,const string& s, const vector<CharInfo>& table, EccLevel level, int version){
    //1. Get the 4 bits for the mode
    //2. make the variable size thingy
    //3. data
    //4. Terminator
    //5. bit padding
    //6. byte padding

    //1
    vector<int> result = toBits(static_cast<int>(mode), 4);
    int countWidth;

    if(mode == Mode::Alphanumeric){countWidth = (version <= 9) ? 9 : (version <= 26) ? 11 : 13;}
        else if(mode == Mode::Numeric){countWidth = (version <= 9) ? 10 : (version <= 26) ? 12 : 14;}
        else{countWidth = (version <= 9) ? 8 : 16;}

    //2
    vector<int> countBits = toBits(s.size(), countWidth);
    result.insert(result.end(), countBits.begin(), countBits.end());

    //3
    for(const CharInfo& row : table){
        result.insert(result.end(), row.bits.begin(),row.bits.end());
    }

    //4
    int capacityBits = DATA_CODEWORDS[version - 1][level] * 8;
    int spaceLeft = capacityBits - static_cast<int>(result.size()); 
    //used static_cast cause sizes for vectors are returned in size_t

    int terminatorSize = min(4, spaceLeft);
    for(int i = 0; i < terminatorSize; i++){
        result.push_back(0);
    }

    //5
    int bitPadding = (8 - (static_cast<int>(result.size()) %  8)) % 8;
    for(int j = 0; j < bitPadding; j++){
        result.push_back(0);
    }

    //6
    vector<int> EC = toBits(0xEC, 8);
    vector<int> II = toBits(0x11, 8);
    int bytesPadding = (capacityBits - static_cast<int>(result.size())) / 8;
    for(int k = 0; k < bytesPadding; k++){
        if(k % 2 == 0){
            result.insert(result.end(), EC.begin(),EC.end());                    
        }
        else{
            result.insert(result.end(), II.begin(),II.end());
        }
    }

    //7
    vector<int> codewords;
    for(int l = 0; l < static_cast<int>(result.size()); l += 8){
        int byte = 0;
        for(int m = 0; m < 8; m++){
            byte = byte * 2 + result[l + m];
        }
        codewords.push_back(byte);
    }

    return codewords;
}
