#include "../../large_int/include/large_int.h"

uint512_t inject(const uint256_t&);
uint256_t trunct(const uint512_t&);

extern const uint256_t Prime[3];

template <int p>
class FpField {
    uint256_t val;
public:
    FpField();
    FpField(const uint256_t& val);

    FpField<p> add_inverse() const;
    FpField<p> mul_inverse() const;

    FpField<p> operator+(const FpField<p>& other) const;
    FpField<p> operator-(const FpField<p>& other) const;
    FpField<p> operator*(const FpField<p>& other) const;
    FpField<p> operator/(const FpField<p>& other) const;

    FpField<p> operator+=(const FpField<p>& other);
    FpField<p> operator-=(const FpField<p>& other);
    FpField<p> operator*=(const FpField<p>& other);
    FpField<p> operator/=(const FpField<p>& other);

    void print() const;
};

#define fp_secp256r1 FpField<0>
#define fp_ed25519 FpField<1>
#define fp_secp256k1 FpField<2>