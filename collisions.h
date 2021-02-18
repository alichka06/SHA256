#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED
#include "sha2.h"

class ProofOfWork {
public:
    ProofOfWork(int k_): k(k_) {}

    vector<uint8_t> BruteForce() {
        vector<uint8_t> v;
        int i = 1;
        bool isColision = false;
        while (!isColision) {

            if (SHA_gen(i, v)) {
                isColision = true;
            }
            i++;
        }
        return v;
    }
private:
    int k;
    bool collision(vector<uint8_t> ar) {
        bool isCollision = true;
        int i = 0, j = 0;
        while (i*8 + j < k && isCollision){
            if (((ar[i] >> (7-j)) & 1) == 1) {
                isCollision = false;
            }
            j++;
            if (j > 7) {
                i++;
                j = 0;
            }
        }
        return isCollision;
    };

    bool SHA_gen(int length,vector<uint8_t>& t ) {
        SHA sha;
        vector<uint8_t> ar(length);
        for (int i = 0; i < length; i++) {
            ar[i] = '0x80';
        }
        vector<uint8_t> out = sha.compressedVector(ar);
        t =  out;
        return collision(out);
    }

};
#endif // COLLISIONS_H_INCLUDED
