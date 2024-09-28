#pragma once

#include <array>
#include <vector>
#include <iomanip>
#include <iostream>

template<int n>
class LargeInt {
    std::array<uint16_t, n> val;

    std::pair<LargeInt<n>, LargeInt<n>> division(const LargeInt<n>& other) const;
public:
    std::array<uint16_t, n> get_val() const;

    LargeInt();
    LargeInt(uint64_t value);
    LargeInt(std::vector<uint16_t> vals);

    LargeInt<n> operator+(const LargeInt<n>& other) const;
    LargeInt<n> operator-(const LargeInt<n>& other) const;
    LargeInt<n> operator*(const LargeInt<n>& other) const;
    LargeInt<n> operator/(const LargeInt<n>& other) const;
    LargeInt<n> operator%(const LargeInt<n>& other) const;
    
    LargeInt<n> operator+=(const LargeInt<n>& other);
    LargeInt<n> operator-=(const LargeInt<n>& other);
    LargeInt<n> operator*=(const LargeInt<n>& other);
    LargeInt<n> operator/=(const LargeInt<n>& other);
    LargeInt<n> operator%=(const LargeInt<n>& other);

    LargeInt<n> operator&(const LargeInt<n>& other) const;
    LargeInt<n> operator|(const LargeInt<n>& other) const;
    LargeInt<n> operator^(const LargeInt<n>& other) const;
    LargeInt<n> operator~() const;

    LargeInt<n> operator<<(const int bit) const;
    LargeInt<n> operator>>(const int bit) const;

    LargeInt<n> operator<<=(const int bit);
    LargeInt<n> operator>>=(const int bit);

    bool operator<(const LargeInt<n>& other) const;
    bool operator>(const LargeInt<n>& other) const;
    bool operator<=(const LargeInt<n>& other) const;
    bool operator>=(const LargeInt<n>& other) const;
    bool operator==(const LargeInt<n>& other) const;
    bool operator!=(const LargeInt<n>& other) const;

    void print() const;
};

#define uint256_t LargeInt<16>
#define uint512_t LargeInt<32>