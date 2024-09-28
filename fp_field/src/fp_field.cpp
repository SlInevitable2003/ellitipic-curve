#include "../include/fp_field.h"

#define assert(x) if (!(x)) { std::cerr << __LINE__ << ": Assertion " << #x << " failed." << std::endl; exit(-1); }

uint512_t inject(const uint256_t& sml)
{
    std::vector<uint16_t> buf;
    auto arr = sml.get_val();
    for (auto x : arr) buf.push_back(x);
    return buf;
}
uint256_t trunct(const uint512_t& lar)
{
    std::vector<uint16_t> buf;
    auto arr = lar.get_val();
    for (int i = 0; i < 16; i++) buf.push_back(arr[i]);
    return buf;
}

FpField::FpField(const uint256_t& prime) : pri{prime}, val{0} {}
FpField::FpField(const uint256_t& prime, const uint256_t& value) : pri{prime}, val{value % pri} {}

FpField FpField::operator+(const FpField& other) const
{
    assert(pri == other.pri);
    uint512_t remainder = (inject(val) + inject(other.val)) % inject(pri);
    return {pri, trunct(remainder)};
}
FpField FpField::operator+=(const FpField& other) { return (*this) = (*this) + other; }

FpField FpField::add_inverse() const { return {pri, pri - val}; }
FpField FpField::operator-(const FpField& other) const { return (*this) + other.add_inverse(); }
FpField FpField::operator-=(const FpField& other) { return (*this) = (*this) - other; }

FpField FpField::operator*(const FpField& other) const
{
    assert(pri == other.pri);
    uint512_t remainder = (inject(val) * inject(other.val)) % inject(pri);
    return {pri, trunct(remainder)};
}
FpField FpField::operator*=(const FpField& other) { return (*this) = (*this) * other; }

FpField FpField::mul_inverse() const 
{
    assert(val != 0);
    // For a in (Z_p)^*, a^(p-1) = 1 ==> a.mul_inverse() = a^(p-2)
    uint256_t power{pri - 2};
    uint512_t unit{inject(val)}, res{1}, mod{inject(pri)};
    while (power > 0) {
        if ((power & 1) > 0) res = (res * unit) % mod;
        unit = (unit * unit) % mod;
        power >>= 1;
    }
    return {pri, trunct(res)};
}
FpField FpField::operator/(const FpField& other) const { return (*this) * other.mul_inverse(); }
FpField FpField::operator/=(const FpField& other) { return (*this) = (*this) / other; }

void FpField::print() { val.print(); }