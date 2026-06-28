#include "QRC.hpp"

bool isNumeric(char c) {return c >= '0' && c<='9';};
//checks in their ASCII range '0' = 48/'9' = 57
bool isAlphanumeric(char c) {return ALPHNUM.find(c) != string::npos;};
//looks for the char in the string alphnum. If its there, great, if not the bool has no value
//which is the npos (No position) string, because if it was 0 we would have it always being true

// [version-1][level]  where level L=0, M=1, Q=2, H=3
const BlockInfo BLOCK_INFO[40][4] = {
    { { 7,  1,  19,  0,   0}, {10,  1,  16,  0,   0}, {13,  1,  13,  0,   0}, {17,  1,   9,  0,   0} },  // V1
    { {10,  1,  34,  0,   0}, {16,  1,  28,  0,   0}, {22,  1,  22,  0,   0}, {28,  1,  16,  0,   0} },  // V2
    { {15,  1,  55,  0,   0}, {26,  1,  44,  0,   0}, {18,  2,  17,  0,   0}, {22,  2,  13,  0,   0} },  // V3
    { {20,  1,  80,  0,   0}, {18,  2,  32,  0,   0}, {26,  2,  24,  0,   0}, {16,  4,   9,  0,   0} },  // V4
    { {26,  1, 108,  0,   0}, {24,  2,  43,  0,   0}, {18,  2,  15,  2,  16}, {22,  2,  11,  2,  12} },  // V5
    { {18,  2,  68,  0,   0}, {16,  4,  27,  0,   0}, {24,  4,  19,  0,   0}, {28,  4,  15,  0,   0} },  // V6
    { {20,  2,  78,  0,   0}, {18,  4,  31,  0,   0}, {18,  2,  14,  4,  15}, {26,  4,  13,  1,  14} },  // V7
    { {24,  2,  97,  0,   0}, {22,  2,  38,  2,  39}, {22,  4,  18,  2,  19}, {26,  4,  14,  2,  15} },  // V8
    { {30,  2, 116,  0,   0}, {22,  3,  36,  2,  37}, {20,  4,  16,  4,  17}, {24,  4,  12,  4,  13} },  // V9
    { {18,  2,  68,  2,  69}, {26,  4,  43,  1,  44}, {24,  6,  19,  2,  20}, {28,  6,  15,  2,  16} },  // V10
    { {20,  4,  81,  0,   0}, {30,  1,  50,  4,  51}, {28,  4,  22,  4,  23}, {24,  3,  12,  8,  13} },  // V11
    { {24,  2,  92,  2,  93}, {22,  6,  36,  2,  37}, {26,  4,  20,  6,  21}, {28,  7,  14,  4,  15} },  // V12
    { {26,  4, 107,  0,   0}, {22,  8,  37,  1,  38}, {24,  8,  20,  4,  21}, {22, 12,  11,  4,  12} },  // V13
    { {30,  3, 115,  1, 116}, {24,  4,  40,  5,  41}, {20, 11,  16,  5,  17}, {24, 11,  12,  5,  13} },  // V14
    { {22,  5,  87,  1,  88}, {24,  5,  41,  5,  42}, {30,  5,  24,  7,  25}, {24, 11,  12,  7,  13} },  // V15
    { {24,  5,  98,  1,  99}, {28,  7,  45,  3,  46}, {24, 15,  19,  2,  20}, {30,  3,  15, 13,  16} },  // V16
    { {28,  1, 107,  5, 108}, {28, 10,  46,  1,  47}, {28,  1,  22, 15,  23}, {28,  2,  14, 17,  15} },  // V17
    { {30,  5, 120,  1, 121}, {26,  9,  43,  4,  44}, {28, 17,  22,  1,  23}, {28,  2,  14, 19,  15} },  // V18
    { {28,  3, 113,  4, 114}, {26,  3,  44, 11,  45}, {26, 17,  21,  4,  22}, {26,  9,  13, 16,  14} },  // V19
    { {28,  3, 107,  5, 108}, {26,  3,  41, 13,  42}, {30, 15,  24,  5,  25}, {28, 15,  15, 10,  16} },  // V20
    { {28,  4, 116,  4, 117}, {26, 17,  42,  0,   0}, {28, 17,  22,  6,  23}, {30, 19,  16,  6,  17} },  // V21
    { {28,  2, 111,  7, 112}, {28, 17,  46,  0,   0}, {30,  7,  24, 16,  25}, {24, 34,  13,  0,   0} },  // V22
    { {30,  4, 121,  5, 122}, {28,  4,  47, 14,  48}, {30, 11,  24, 14,  25}, {30, 16,  15, 14,  16} },  // V23
    { {30,  6, 117,  4, 118}, {28,  6,  45, 14,  46}, {30, 11,  24, 16,  25}, {30, 30,  16,  2,  17} },  // V24
    { {26,  8, 106,  4, 107}, {28,  8,  47, 13,  48}, {30,  7,  24, 22,  25}, {30, 22,  15, 13,  16} },  // V25
    { {28, 10, 114,  2, 115}, {28, 19,  46,  4,  47}, {28, 28,  22,  6,  23}, {30, 33,  16,  4,  17} },  // V26
    { {30,  8, 122,  4, 123}, {28, 22,  45,  3,  46}, {30,  8,  23, 26,  24}, {30, 12,  15, 28,  16} },  // V27
    { {30,  3, 117, 10, 118}, {28,  3,  45, 23,  46}, {30,  4,  24, 31,  25}, {30, 11,  15, 31,  16} },  // V28
    { {30,  7, 116,  7, 117}, {28, 21,  45,  7,  46}, {30,  1,  23, 37,  24}, {30, 19,  15, 26,  16} },  // V29
    { {30,  5, 115, 10, 116}, {28, 19,  47, 10,  48}, {30, 15,  24, 25,  25}, {30, 23,  15, 25,  16} },  // V30
    { {30, 13, 115,  3, 116}, {28,  2,  46, 29,  47}, {30, 42,  24,  1,  25}, {30, 23,  15, 28,  16} },  // V31
    { {30, 17, 115,  0,   0}, {28, 10,  46, 23,  47}, {30, 10,  24, 35,  25}, {30, 19,  15, 35,  16} },  // V32
    { {30, 17, 115,  1, 116}, {28, 14,  46, 21,  47}, {30, 29,  24, 19,  25}, {30, 11,  15, 46,  16} },  // V33
    { {30, 13, 115,  6, 116}, {28, 14,  46, 23,  47}, {30, 44,  24,  7,  25}, {30, 59,  16,  1,  17} },  // V34
    { {30, 12, 121,  7, 122}, {28, 12,  47, 26,  48}, {30, 39,  24, 14,  25}, {30, 22,  15, 41,  16} },  // V35
    { {30,  6, 121, 14, 122}, {28,  6,  47, 34,  48}, {30, 46,  24, 10,  25}, {30,  2,  15, 64,  16} },  // V36
    { {30, 17, 122,  4, 123}, {28, 29,  46, 14,  47}, {30, 49,  24, 10,  25}, {30, 24,  15, 46,  16} },  // V37
    { {30,  4, 122, 18, 123}, {28, 13,  46, 32,  47}, {30, 48,  24, 14,  25}, {30, 42,  15, 32,  16} },  // V38
    { {30, 20, 117,  4, 118}, {28, 40,  47,  7,  48}, {30, 43,  24, 22,  25}, {30, 10,  15, 67,  16} },  // V39
    { {30, 19, 118,  6, 119}, {28, 18,  47, 31,  48}, {30, 34,  24, 34,  25}, {30, 20,  15, 61,  16} },  // V40
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

int sizeDataCodewords(int version, EccLevel level){
    
    BlockInfo info = BLOCK_INFO[version][level];
    return info.g1Size * info.g1Blocks + info.g2Size * info.g2Blocks;
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

        int codewordSize = sizeDataCodewords(i, level);

        if(codewordSize >= codewords){
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
    int codewordSize = sizeDataCodewords(version - 1, level);

    int capacityBits = codewordSize * 8;
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

int xtime(int transform){ 
//this function is the start for Reed-Solomons math
//what is does, the << 1 multiplies a bit by 2 (its exponents math from here onwards)
//(in bits, when you multiply by 2 you essentially shift your number to the left (eg. 0010 << 1 = 0100))

    int result = transform << 1; 

//after that, the if checks if after multplying by 2 it exceeds 8 bits 
//if it does, it XORs the result with the number 0x11D, which is an irreducible polinomial
//that will wrap aroun the number back into fitting into 8 bits.

    if(transform & 0x80){
        result ^= 0x11D;
    }

    return result;
}

int gf_exp[256];
int gf_log[256];

void initTables(){
//creates two tables, one for exponents and one for logs (inverse of exponents)
//these tables are used to find any of these two values, which will make our life quicker whenever
//we need to do math w these two
//gf_exp[2] = 4, gf_log[4] = 2

    int x = 1;

    for(int i = 0; i < 255; i++){
        gf_exp[i] = x;
        gf_log[x] = i;
        x = xtime(x);
    }
}

int gf_mult(int a, int b){
//self explanatory, but if 0 the answer is 0
    if(a == 0 || b == 0){return 0;}

    int sum = gf_log[a] + gf_log[b];

//if the sum is bigger than 254 we need to wrap it back to fit in our bit sizes
//therefore, we use %255, and then we work with the exponent of the remainder of that operation
//e.g sum = 260 -> 260 > 254 -> 260 % 250 = 5, and then we return that exponent.

    if(sum > 254){
        return gf_exp[(sum % 255)];
    }

    return gf_exp[sum];
}

vector<int> gf_plnm(vector<int> polynome, int size){
    //1. (x + 1) * x -> [1,1] * 1 -> [1, 1, 0]
    //2. (x + 1) * c -> [1, 1] * 2 -> [0, 2, 2]
    //3. [1, 1, 0] ^ [0, 2, 2] = [1, 3, 2]

    //2
    vector<int> multiplier;
    for(int num : polynome){
        multiplier.push_back(gf_mult(num, size));
    }
    multiplier.insert(multiplier.begin(), 0);
    
    //1
    polynome.push_back(0); //this is the equivalent of * 1, as you can see in //1

    //3
    vector<int> result;
    for(size_t i = 0; i < polynome.size(); i++){
        result.push_back(polynome[i] ^ multiplier[i]);
    }

    return result;
}

vector<int> makeGenerator(int sizeCodeword){
    vector<int> generator = {1};
    for(int i = 0; i < sizeCodeword; i++){
        generator = gf_plnm(generator, gf_exp[i]);
    }

    return generator;
}

vector<int> gf_division(vector<int> codewords, const vector<int>& generator){
    //1. Add 0s to the end of codewords, so it matches the D + N size needed
    //2. scale generator by codewords[i], and then apply it to N + 1 cells in codewords
    //3. XOR the scaled generator array, and the first int in that array will cancel out
    //since n1 ^ n1 = 0.
    //Repeat this process until the end of array
    
    size_t D = codewords.size();
    size_t N = generator.size() - 1; //for generator = [1,3,2] -> N=2

    //1
    for(size_t i = 0; i < N; i++){
        codewords.push_back(0);
    }

    //2
    for(size_t j = 0; j < D; j++){
        int modifier = codewords[j];
        //modifier is important here because it saves the value of codewords[j] into an int
        //therefore, in case k=0, we wont alterate the value inside gf_mult.
        //or else, we would be iterating the numbers with (0, generator[k])

        if(modifier != 0){
            for(size_t k = 0; k < generator.size(); k++){
                //3
                codewords[j + k] ^= gf_mult(modifier, generator[k]);
            }
        }
    }

    codewords.erase(codewords.begin(), codewords.begin() + D);
    //we only care for the end of the array, which is the N, so we delete D

    return codewords;

}

vector<int> getEcc(vector<int> block,int version,  EccLevel level){
    int numCodewords = BLOCK_INFO[version - 1][level].eccPerBlock;
    vector<int> generator = makeGenerator(numCodewords);

    return gf_division(block, generator); 
}