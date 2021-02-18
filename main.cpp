#include <iostream>
#include <bitset>
#include <ctime>
#include "sha2.h"
#include "collisions.h"
using namespace std;

    string words_to_string(vector<uint8_t> v) {
        string ans = "";
        for (int i = 0; i < v.size(); ++i) {
            stringstream stream;
            stream << hex << v[i];
            string result( stream.str() );
            ans += result;
        }
        return ans;
    }

       void to_string(vector<uint8_t> v) {
        string ans = "";
        for (int i = 0; i < v.size(); ++i) {
            cout  << (int)v[i];
        }
    }





int main() {
	clock_t begin = clock();
    //SHA sha;
    // cout << sha.compress("hello world");
    ProofOfWork pw(12);
    to_string(pw.BruteForce());

    clock_t end = clock();
	double elapsed_secs = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Time " << elapsed_secs;
    return 0;
}
