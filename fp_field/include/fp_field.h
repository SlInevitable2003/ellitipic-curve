#include "../../large_int/include/large_int.h"

uint512_t inject(uint256_t);

class FpField {
    uint256_t pri, val;
public:
    FpField(uint256_t prime);
    FpField(uint256_t prime, uint256_t val);

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
};