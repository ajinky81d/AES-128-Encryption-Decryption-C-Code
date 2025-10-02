#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

const unsigned char Rcon[11] = {
    0x00,
    0x01, 0x02, 0x04, 0x08,
    0x10, 0x20, 0x40, 0x80,
    0x1B, 0x36
};

const unsigned char inv_sbox[16][16] = {
    {0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb},
    {0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb},
    {0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e},
    {0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25},
    {0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92},
    {0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84},
    {0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06},
    {0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b},
    {0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73},
    {0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e},
    {0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b},
    {0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4},
    {0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f},
    {0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef},
    {0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61},
    {0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d}
};

const unsigned char sbox[16][16] = {
    {0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76},
    {0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0},
    {0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15},
    {0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75},
    {0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84},
    {0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf},
    {0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8},
    {0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2},
    {0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73},
    {0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb},
    {0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79},
    {0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08},
    {0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a},
    {0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e},
    {0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf},
    {0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16}
};

unsigned char allKeysMatrix[4][44];

void hexStringToMatrix(const std::string &hexStr, unsigned char matrix[4][4]) {
    for (int i = 0; i < 16; ++i) {
        std::string byteStr = hexStr.substr(2 * i, 2);
        int row = i % 4;
        int col = i / 4;
        matrix[row][col] = static_cast<unsigned char>(std::stoul(byteStr, nullptr, 16));
    }
}

std::string matrixToString(const unsigned char matrix[4][4]) {
    std::string result;
    char hex[3];
    for (int i = 0; i < 16; ++i) {
        int row = i % 4;
        int col = i / 4;
        sprintf(hex, "%02x", matrix[row][col]);
        result += hex;
    }
    return result;
}

void printMatrix(const unsigned char matrix[4][4]) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            std::cout << std::setw(2) << std::setfill('0')
                      << std::hex << static_cast<int>(matrix[row][col]) << " ";
        }
        std::cout << std::endl;
    }
}

void copyState(unsigned char dest[4][4], const unsigned char src[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

std::string hexToAscii(const std::string& hex) {
    std::string ascii;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byte = hex.substr(i, 2);
        char chr = static_cast<char>(std::stoi(byte, nullptr, 16));
        ascii += chr;
    }
    return ascii;
}



void generateAllRoundKeys(const unsigned char initialKey[4][4], unsigned char allKeyMatrix[4][44]) {
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            allKeyMatrix[r][c] = initialKey[r][c];
        }
    }

    auto subWord = [](unsigned char w[4]) {
        for (int i = 0; i < 4; ++i) {
            unsigned char b = w[i];
            unsigned char hi = (b >> 4) & 0x0F;
            unsigned char lo = b & 0x0F;
            w[i] = sbox[hi][lo];
        }
    };

    for (int round = 1; round <= 10; ++round) {
        unsigned char temp[4] = {
            allKeyMatrix[0][4 * round - 1],
            allKeyMatrix[1][4 * round - 1],
            allKeyMatrix[2][4 * round - 1],
            allKeyMatrix[3][4 * round - 1]
        };

        unsigned char rotTemp[4] = {temp[1], temp[2], temp[3], temp[0]};
        subWord(rotTemp);
        rotTemp[0] ^= Rcon[round];

        for (int r = 0; r < 4; ++r) {
            allKeyMatrix[r][4 * round] = allKeyMatrix[r][4 * (round - 1)] ^ rotTemp[r];
        }

        for (int c = 1; c < 4; ++c) {
            for (int r = 0; r < 4; ++r) {
                allKeyMatrix[r][4 * round + c] = allKeyMatrix[r][4 * (round - 1) + c] ^ allKeyMatrix[r][4 * round + c - 1];
            }
        }
    }
}

void getRoundKey(const unsigned char allKeyMatrix[4][44], unsigned char roundKey[4][4], int roundNumber) {
    int startCol = 4 * roundNumber;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            roundKey[r][c] = allKeyMatrix[r][startCol + c];
        }
    }
}

void inv_addRoundKey(const unsigned char state[4][4], const unsigned char roundKey[4][4], unsigned char newState[4][4]) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            newState[row][col] = state[row][col] ^ roundKey[row][col];
        }
    }
}

void inv_subBytes(const unsigned char state[4][4], unsigned char newState[4][4]) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            unsigned char byte = state[row][col];
            unsigned char highNibble = (byte >> 4) & 0x0F;
            unsigned char lowNibble = byte & 0x0F;
            newState[row][col] = inv_sbox[highNibble][lowNibble];
        }
    }
}

void inv_shiftRows(const unsigned char state[4][4], unsigned char newState[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            newState[i][j] = state[i][j];
        }
    }

    unsigned char temp;

    // Row 1: shift right 1
    temp = newState[1][3];
    for (int col = 3; col > 0; col--)
        newState[1][col] = newState[1][col - 1];
    newState[1][0] = temp;

    // Row 2: shift right 2
    temp = newState[2][0];
    newState[2][0] = newState[2][2];
    newState[2][2] = temp;
    temp = newState[2][1];
    newState[2][1] = newState[2][3];
    newState[2][3] = temp;

    // Row 3: shift right 3 (left shift by 1)
    temp = newState[3][0];
    newState[3][0] = newState[3][1];
    newState[3][1] = newState[3][2];
    newState[3][2] = newState[3][3];
    newState[3][3] = temp;
}

void inv_MixColumns(const unsigned char state[4][4], unsigned char newState[4][4]) {
    auto xtime = [](unsigned char x) -> unsigned char {
        unsigned char res = static_cast<unsigned char>(x << 1);
        if (x & 0x80) res ^= 0x1B;
        return res;
    };

    auto gfmul = [&](unsigned char a, unsigned char factor) -> unsigned char {
        switch (factor) {
            case 0x01: return a;
            case 0x02: return xtime(a);
            case 0x03: return static_cast<unsigned char>(xtime(a) ^ a);
            case 0x09: return static_cast<unsigned char>(xtime(xtime(xtime(a))) ^ a);
            case 0x0b: return static_cast<unsigned char>(xtime(xtime(xtime(a))) ^ xtime(a) ^ a);
            case 0x0d: return static_cast<unsigned char>(xtime(xtime(xtime(a))) ^ xtime(xtime(a)) ^ a);
            case 0x0e: return static_cast<unsigned char>(xtime(xtime(xtime(a))) ^ xtime(xtime(a)) ^ xtime(a));
            default: return 0x00;
        }
    };

    for (int c = 0; c < 4; ++c) {
        unsigned char a0 = state[0][c];
        unsigned char a1 = state[1][c];
        unsigned char a2 = state[2][c];
        unsigned char a3 = state[3][c];
        newState[0][c] = static_cast<unsigned char>(
            gfmul(a0, 0x0e) ^ gfmul(a1, 0x0b) ^ gfmul(a2, 0x0d) ^ gfmul(a3, 0x09)
        );
        newState[1][c] = static_cast<unsigned char>(
            gfmul(a0, 0x09) ^ gfmul(a1, 0x0e) ^ gfmul(a2, 0x0b) ^ gfmul(a3, 0x0d)
        );
        newState[2][c] = static_cast<unsigned char>(
            gfmul(a0, 0x0d) ^ gfmul(a1, 0x09) ^ gfmul(a2, 0x0e) ^ gfmul(a3, 0x0b)
        );
        newState[3][c] = static_cast<unsigned char>(
            gfmul(a0, 0x0b) ^ gfmul(a1, 0x0d) ^ gfmul(a2, 0x09) ^ gfmul(a3, 0x0e)
        );
    }
}


//-------------------------------------------test cases check-----------------------------------------//



int main() {
    //runTests();
    while (1) {
        std::string cipherText, key;
        const int requiredSize = 32;

        std::cout << "Enter cipher text: ";
        std::getline(std::cin, cipherText);
        if (cipherText.size() != requiredSize) {
            std::cout << "Invalid cipher text size. Required size is " << requiredSize << " characters.\n";
            continue;
        }

        std::cout << "Enter key: ";
        std::getline(std::cin, key);
        if (key.size() != requiredSize) {
            std::cout << "Invalid key size. Required size is " << requiredSize << " characters.\n";
            continue;
        }

        unsigned char keyMatrix[4][4], cipherMatrix[4][4];
        hexStringToMatrix(cipherText, cipherMatrix);
        hexStringToMatrix(key, keyMatrix);

        generateAllRoundKeys(keyMatrix, allKeysMatrix);

        unsigned char state[4][4];
        copyState(state, cipherMatrix);

        // FIPS 197 INVCIPHER Algorithm:

        // Step 1: Initial AddRoundKey with round key 10
        unsigned char temp[4][4], roundKey[4][4];
        getRoundKey(allKeysMatrix, roundKey, 10);
        inv_addRoundKey(state, roundKey, temp);
        copyState(state, temp);

        // Step 2: Rounds 9 down to 1
        for (int round = 9; round >= 1; round--) {
            unsigned char t1[4][4], t2[4][4], t3[4][4];

            // InvShiftRows
            inv_shiftRows(state, t1);

            // InvSubBytes
            inv_subBytes(t1, t2);

            // AddRoundKey
            getRoundKey(allKeysMatrix, roundKey, round);
            inv_addRoundKey(t2, roundKey, t3);

            // InvMixColumns
            inv_MixColumns(t3, state);
        }

        // Step 3: Final round (round 0)
        unsigned char t1[4][4], t2[4][4];

        // InvShiftRows
        inv_shiftRows(state, t1);

        // InvSubBytes
        inv_subBytes(t1, t2);

        // AddRoundKey with round key 0
        getRoundKey(allKeysMatrix, roundKey, 0);
        inv_addRoundKey(t2, roundKey, state);


        std::string plainHex = matrixToString(state);
        std::cout << "final output HEX  : " << plainHex << std::endl;
        std::cout << "final output ASCII: " << hexToAscii(plainHex) << std::endl;

    }
    return 0;
}
