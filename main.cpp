#include <iostream>
#include <bitset>
#include "sha2.h"
using namespace std;

int main() {

    SHA sha;
    cout << sha.compress("hello world");

    return 0;
}
