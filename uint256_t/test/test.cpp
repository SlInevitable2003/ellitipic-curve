#include "../include/uint256_t.h"

uint16_t urand16() { return rand() & ((1 << 16) - 1); }
uint256_t urand256() 
{
    uint256_t res;
    for (int i = 0; i < EIGHT; i++) res = (res << 16) + urand16();
    return res;
}

int main(int argc, char *argv[])
{
    srand(time(0));
    uint256_t a{urand256()}, b{urand256()};
    a.print(); b.print();
    (a + b).print();
    (a - b).print();
    (a * b).print();
    (a / b).print();
    (a % b).print();
    (a - (a / b) * b).print();

    return 0;
}