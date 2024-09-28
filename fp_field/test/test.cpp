#include "../include/fp_field.h"
#include <chrono>
using namespace std;

extern template class uint256_t;
extern template class uint512_t;

uint256_t zero{0}, one{1};
uint256_t pri = (zero - one) - (one << 224) + (one << 192) + (one << 96);

uint16_t urand16() { return rand() & ((1 << 16) - 1); }
uint256_t urand256() 
{
    uint256_t res;
    for (int i = 0; i < 16; i++) res = (res << 16) + urand16();
    return res;
}

chrono::high_resolution_clock::time_point st, ed;

#define prof(x) \
    st = chrono::high_resolution_clock::now(); \
    x; \
    ed = chrono::high_resolution_clock::now(); \
    cout << __LINE__ << ": " << #x << " : " << dec << chrono::duration_cast<chrono::microseconds>(ed - st).count() << " us\n";

int main(int argc, char *argv[])
{
    srand(time(0));
    pri.print();
    FpField a{pri, urand256()}, b{pri, urand256()};
    a.print(); b.print();
    
    prof((a + b));
    prof((a - b));
    prof((a * b));
    prof((a / a));
    prof((a / b));

    return 0;
}