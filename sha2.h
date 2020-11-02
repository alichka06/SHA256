#ifndef SHA2_H_INCLUDED
#define SHA2_H_INCLUDED
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <cmath>
using namespace std;

    void print(unsigned char a){
        for (int i = 7; i >= 0; i--){
            if (a >= pow(2, i)){
                cout << 1;
                a -= pow(2, i);
            }
            else{
                cout << 0;
            }
        }
        cout << endl;
    }
    void print32(uint32_t a){
        for (int i = 31; i >= 0; i--){
            if (a >= pow(2, i)){
                cout << 1;
                a -= pow(2, i);
            }
            else{
                cout << 0;
            }
        }
        cout << endl;
    }

class SHA {
public:
    SHA() {}

    string compress(string message) {
        vector<uint8_t> block = string_to_bytes(message);

        block = pad_to_512bits(block);

        for (int i = 0; i < block.size()/64; ++i) {
            vector<uint8_t> subvector(block.begin() + i*64, block.begin()+(i+1)*64);

            vector<uint32_t> words = to_words(subvector);

            hashing(words);
        }
        return words_to_string();
    }

private:
    uint32_t H[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                     0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    const uint32_t K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

    uint32_t shift(uint32_t n, int pos) {
        return (n >> pos) | (n << (32 - pos));
    }
    uint32_t s0(uint32_t n) {
        return shift(n, 7)^shift(n, 18)^(n >> 3);
    }
    uint32_t s1(uint32_t n) {
        return shift(n, 17)^shift(n, 19)^(n >> 10);
    }
    uint32_t sigma0(uint32_t n) {
        return shift(n, 2)^shift(n, 13)^shift(n, 22);
    }
    uint32_t sigma1(uint32_t n) {
        return shift(n, 6)^shift(n, 11)^shift(n, 25);
    }
    uint32_t f_func(uint32_t u, uint32_t v, uint32_t w) {
        return ((u)&(v))^((~(u))&(w));
    }
    uint32_t g_func(uint32_t u, uint32_t v, uint32_t w) {
        return ((u)&(v))^((u)&(w))^((v)&(w));
    }
    vector<uint8_t> string_to_bytes(const string input) {
        vector<uint8_t> block;
        for (int i = 0; i < input.size(); ++i) {
            bitset<8> b(input[i]);
            block.push_back((uint8_t)b.to_ulong());
        }
        return block;
    }

    vector<uint8_t> pad_to_512bits(vector<uint8_t> block) {
        int len = block.size() * 8;
        len = len % 512;
        int k = 447 - len;

        uint8_t t1 = 0x80;
        block.push_back(0x80);
        k = k - 7; // додані 7 нулів

        for(int i = 0; i < k / 8; i++)
            block.push_back(0x00000000);

        bitset<64> len_to_bits(len);
        string len_to_bits_string = len_to_bits.to_string();

        for(int i = 0; i < 63; i += 8) {
            bitset<8> temp_string_holder(len_to_bits_string.substr(i,8));
            block.push_back(temp_string_holder.to_ulong());
        }

        return block;
    }

    vector<uint32_t> to_words(vector<uint8_t> in) {
        vector<uint32_t> output(16);

        for(int i = 0; i < 16; i++) {
            uint32_t temp =   in[i * 4 + 3] +
                                ((uint16_t) in[i * 4 + 2] << 8) +
                                ((uint32_t) in[i * 4 + 1] << 16) +
                                ((uint32_t) in[i * 4] << 24);
            output[i] = temp;
        }

        return output;
    }
    void hashing(vector<uint32_t> X) {
        for (int i = 16; i < 64; ++i) {
            X.push_back(s1(X[i-2]) + X[i-7] + s0(X[i-15]) + X[i-16]);
        }
        uint32_t w[8];
        for (int i = 0; i < 8; ++i) {
            w[i] = H[i];
        }
        for (int i = 0; i < 64; ++i) {
            uint32_t t1 = w[7] + sigma1(w[4]) + f_func(w[4],w[5],w[6]) + K[i] + X[i];
            uint32_t t2 = sigma0(w[0]) + g_func(w[0],w[1],w[2]);
            w[7] = w[6];
            w[6] = w[5];
            w[5] = w[4];
            w[4] = w[3] + t1;
            w[3] = w[2];
            w[2] = w[1];
            w[1] = w[0];
            w[0] = t1 + t2;
        }
        for (int i = 0; i < 8; ++i) {
            H[i] += w[i];
        }
    }
    string words_to_string() {
        string ans = "";
        for (int i = 0; i < 8; ++i) {
            stringstream stream;
            stream << hex << H[i];
            string result( stream.str() );
            ans += result;
        }
        return ans;
    }
};


#endif // SHA2_H_INCLUDED
