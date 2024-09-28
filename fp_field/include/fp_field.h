#include "../../large_int/include/large_int.h"

uint512_t inject(const uint256_t&);
uint256_t trunct(const uint512_t&);

class FpField {
    uint256_t pri, val;
public:
    FpField(const uint256_t& prime);
    FpField(const uint256_t& prime, const uint256_t& val);

    FpField add_inverse() const;
    FpField mul_inverse() const;

    FpField operator+(const FpField& other) const;
    FpField operator-(const FpField& other) const;
    FpField operator*(const FpField& other) const;
    FpField operator/(const FpField& other) const;

    FpField operator+=(const FpField& other);
    FpField operator-=(const FpField& other);
    FpField operator*=(const FpField& other);
    FpField operator/=(const FpField& other);

    void print();
};