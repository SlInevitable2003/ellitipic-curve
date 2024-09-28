#include "../include/large_int.h"

#define assert(x) if (!(x)) { std::cerr << __LINE__ << ": Assertion " << #x << " failed." << std::endl; exit(-1); }

template<int n> std::array<uint16_t, n> LargeInt<n>::get_val() const { return val; }

// constructor

template<int n> LargeInt<n>::LargeInt() : val{0} {}
template<int n> LargeInt<n>::LargeInt(uint64_t value) : val{0}
{
    val[0] = value & ((1 << 16) - 1);
    val[1] = (value >> 16) & ((1 << 16) - 1);
    val[2] = (value >> 32) & ((1 << 16) - 1);
    val[3] = (value >> 48) & ((1 << 16) - 1);
}
template<int n> LargeInt<n>::LargeInt(std::vector<uint16_t> vals) : val{0}
{
    assert(vals.size() <= n);
    for (int i = 0; i < vals.size(); i++) val[i] = vals[i];
}

// comparator

template<int n> bool LargeInt<n>::operator<(const LargeInt<n>& other) const 
{ 
    for (int i = n - 1; i >= 0; i--) {
        if (val[i] < other.val[i]) return true; 
        else if (val[i] > other.val[i]) return false;
    }
    return false;
}
template<int n> bool LargeInt<n>::operator>(const LargeInt<n>& other) const { return other < (*this); }
template<int n> bool LargeInt<n>::operator<=(const LargeInt<n>& other) const { return !((*this) > other); }
template<int n> bool LargeInt<n>::operator>=(const LargeInt<n>& other) const { return !((*this) < other); }
template<int n> bool LargeInt<n>::operator==(const LargeInt<n>& other) const
{
    for (int i = n - 1; i >= 0; i--) if (val[i] != other.val[i]) return false;
    return true;
}
template<int n> bool LargeInt<n>::operator!=(const LargeInt<n>& other) const { return !((*this) == other); }

// bit-wise operator

template<int n> LargeInt<n> LargeInt<n>::operator&(const LargeInt<n>& other) const
{
    LargeInt<n> res;
    for (int i = 0; i < n; i++) res.val[i] = val[i] & other.val[i];
    return res;
}
template<int n> LargeInt<n> LargeInt<n>::operator|(const LargeInt<n>& other) const
{
    LargeInt<n> res;
    for (int i = 0; i < n; i++) res.val[i] = val[i] | other.val[i];
    return res;
}
template<int n> LargeInt<n> LargeInt<n>::operator^(const LargeInt<n>& other) const
{
    LargeInt<n> res;
    for (int i = 0; i < n; i++) res.val[i] = val[i] ^ other.val[i];
    return res;
}
template<int n> LargeInt<n> LargeInt<n>::operator~() const
{
    LargeInt<n> res;
    for (int i = 0; i < n; i++) res.val[i] = ~val[i];
    return res;
}

template<int n> LargeInt<n> LargeInt<n>::operator<<(const int bit) const
{
    int whl = bit >> 4, sml = bit & ((1 << 4) - 1);
    LargeInt<n> res;
    for (int i = n - 1; i >= whl; i--) res.val[i] = val[i - whl];
    uint16_t trans = 0;
    for (int i = whl; i < n; i++) {
        uint16_t new_trans = (res.val[i] & ~((1 << (16 - sml)) - 1)) >> (16 - sml);
        res.val[i] = (res.val[i] << sml) | trans;
        trans = new_trans;
    }
    return res;
}
template<int n> LargeInt<n> LargeInt<n>::operator>>(const int bit) const
{
    int whl = bit >> 4, sml = bit & ((1 << 4) - 1);
    LargeInt<n> res;
    for (int i = 0; i < n - whl; i++) res.val[i] = val[i + whl];
    uint16_t trans = 0;
    for (int i = n - whl - 1; i >= 0; i--) {
        uint16_t new_trans = (res.val[i] & ((1 << sml) - 1)) << (16 - sml);
        res.val[i] = (res.val[i] >> sml) | trans;
        trans = new_trans;
    }
    return res;
}

template<int n> LargeInt<n> LargeInt<n>::operator<<=(const int bit) { return (*this) = (*this) << bit; }
template<int n> LargeInt<n> LargeInt<n>::operator>>=(const int bit) { return (*this) = (*this) >> bit; }

// ordinary operation

template<int n> LargeInt<n> LargeInt<n>::operator+(const LargeInt<n>& other) const
{
    uint16_t carry = 0;
    LargeInt<n> res;
    for (int i = 0; i < n; i++) {
        uint32_t sum = (uint32_t)val[i] + other.val[i] + carry;
        carry = sum >> 16, res.val[i] = sum;
    }
    return res;
}
template<int n> LargeInt<n> LargeInt<n>::operator+=(const LargeInt<n>& other) { return (*this) = (*this) + other; }

template<int n> LargeInt<n> LargeInt<n>::operator-(const LargeInt<n>& other) const { return (*this) + (~other) + 1; }
template<int n> LargeInt<n> LargeInt<n>::operator-=(const LargeInt<n>& other) { return (*this) = (*this) - other; }

template<int n> LargeInt<n> LargeInt<n>::operator*(const LargeInt<n>& other) const
{
    LargeInt<n> res;
    for (int i = 0; i < n; i++) {
        uint16_t carry = 0;
        for (int j = 0; i + j < n; j++) {
            uint32_t sum = res.val[i + j] + (uint32_t)val[i] * other.val[j] + carry;
            carry = sum >> 16, res.val[i + j] = sum;
        }
    }
    return res;
}
template<int n> LargeInt<n> LargeInt<n>::operator*=(const LargeInt<n>& other) { return (*this) = (*this) * other; }

template<int n> std::pair<LargeInt<n>, LargeInt<n>> LargeInt<n>::division(const LargeInt<n>& other) const 
{
    int la = n - 1, lb = n - 1;
    while (val[la] == 0) la--;
    while (other.val[lb] == 0) lb--;
    int diff = la - lb;

    LargeInt<n> quotient, remainder;
    for (int i = lb; i >= 0; i--) remainder.val[i] = val[i + diff];
    LargeInt<n> power[16]; power[0] = other;
    int powerlimit = 16;
    for (int i = 1; i < 16; i++) {
        power[i] = power[i - 1] + power[i - 1];
        if (power[i] < power[i - 1]) {
            powerlimit = i;
            break;
        }
    }
    for (int i = diff; i >= 0; i--) {
        LargeInt<n> sum;
        uint16_t quotient_bit = 0;
        for (int j = powerlimit - 1; j >= 0; j--) {
            if (sum + power[j] <= remainder) {
                sum += power[j];
                quotient_bit |= (1 << j);
            }
        }
        remainder -= sum;
        quotient.val[i] = quotient_bit;
        if (i > 0) {
            remainder <<= 16;
            remainder.val[0] = val[i - 1];
        }
    }
    return {quotient, remainder};
}

template<int n> LargeInt<n> LargeInt<n>::operator/(const LargeInt<n>& other) const
{
    assert(other != 0);
    if ((*this) < other) return 0;
    return division(other).first;
}
template<int n> LargeInt<n> LargeInt<n>::operator/=(const LargeInt<n>& other) { return (*this) = (*this) / other; }

template<int n> LargeInt<n> LargeInt<n>::operator%(const LargeInt<n>& other) const
{
    assert(other != 0);
    if ((*this) < other) return *this;
    return division(other).second;
}
template<int n> LargeInt<n> LargeInt<n>::operator%=(const LargeInt<n>& other) { return (*this) = (*this) % other; }

template<int n> void LargeInt<n>::print()
{
    for (int i = n - 1; i >= 0; i--) std::cout << std::setw(4) << std::setfill('0') << std::hex << val[i];
    std::cout << std::endl;
}

template class uint256_t;
template class uint512_t;