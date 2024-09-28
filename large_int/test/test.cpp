#include "../include/large_int.h"

extern template class uint256_t;
extern template class uint512_t;

uint16_t urand16() { return rand() & ((1 << 16) - 1); }
uint256_t urand256() 
{
    uint256_t res;
    for (int i = 0; i < 8; i++) res = (res << 16) + urand16();
    return res;
}
uint512_t urand512()
{
    uint512_t res;
    for (int i = 0; i < 16; i++) res = (res << 16) + urand16();
    return res;
}

int main(int argc, char *argv[])
{
    srand(time(0));
    uint512_t a{urand512()}, b{urand512() >> 32};
    a.print(); b.print();
    (a + b).print();
    (a - b).print();
    (a * b).print();
    (a / b).print();
    (a % b).print();
    (a - (a / b) * b).print();

    return 0;
}