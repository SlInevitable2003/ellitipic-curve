#include "../include/fp_field.h"

#define assert(x) if (!(x)) { std::cerr << __LINE__ << ": Assertion " << #x << " failed." << std::endl; exit(-1); }

std::vector<uint16_t> arr0 = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0, 0, 0, 0, 0, 0, 1, 0, 0xffff, 0xffff };
std::vector<uint16_t> arr1 = { 0xffed, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
                               0xffff, 0xffff, 0xffff, 0x7fff };
std::vector<uint16_t> arr2 = { 0xfc2f, 0xffff, 0xfffe, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
                               0xffff, 0xffff, 0xffff, 0xffff };
const uint256_t Prime[3] = {arr0, arr1, arr2};                               

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

template<int p> FpField<p>::FpField() : val{0} {}
template<int p> FpField<p>::FpField(const uint256_t& value) : val{value % Prime[p]} {}

template<int p> FpField<p> FpField<p>::operator+(const FpField<p>& other) const
{
    uint512_t remainder = (inject(val) + inject(other.val)) % inject(Prime[p]);
    return trunct(remainder);
}
template<int p> FpField<p> FpField<p>::operator+=(const FpField<p>& other) { return (*this) = (*this) + other; }

template<int p> FpField<p> FpField<p>::add_inverse() const { return {Prime[p] - val}; }
template<int p> FpField<p> FpField<p>::operator-(const FpField<p>& other) const { return (*this) + other.add_inverse(); }
template<int p> FpField<p> FpField<p>::operator-=(const FpField<p>& other) { return (*this) = (*this) - other; }

template<int p> FpField<p> FpField<p>::operator*(const FpField<p>& other) const
{
    uint512_t remainder = (inject(val) * inject(other.val)) % inject(Prime[p]);
    return trunct(remainder);
}
template<int p> FpField<p> FpField<p>::operator*=(const FpField<p>& other) { return (*this) = (*this) * other; }

template<int p> FpField<p> FpField<p>::mul_inverse() const 
{
    assert(val != 0);
    // For a in (Z_p)^*, a^(p-1) = 1 ==> a.mul_inverse() = a^(p-2)
    uint256_t power{Prime[p] - 2};
    uint512_t unit{inject(val)}, res{1}, mod{inject(Prime[p])};
    while (power > 0) {
        if ((power & 1) > 0) res = (res * unit) % mod;
        unit = (unit * unit) % mod;
        power >>= 1;
    }
    return trunct(res);
}
template<int p> FpField<p> FpField<p>::operator/(const FpField<p>& other) const { return (*this) * other.mul_inverse(); }
template<int p> FpField<p> FpField<p>::operator/=(const FpField<p>& other) { return (*this) = (*this) / other; }

template<int p> void FpField<p>::print() const { val.print(); }

template class fp_secp256r1;
template class fp_ed25519;
template class fp_secp256k1;