#pragma once

#include <array>
#include <iomanip>
#include <iostream>

#define EIGHT 8

class uint256_t {
    std::array<uint16_t, EIGHT> val;
public:
    uint256_t();
    uint256_t(uint64_t value);

    uint256_t operator+(const uint256_t& other) const;
    uint256_t operator-(const uint256_t& other) const;
    uint256_t operator*(const uint256_t& other) const;
    uint256_t operator/(const uint256_t& other) const;
    uint256_t operator%(const uint256_t& other) const;
    
    uint256_t operator+=(const uint256_t& other);
    uint256_t operator-=(const uint256_t& other);
    uint256_t operator*=(const uint256_t& other);
    uint256_t operator/=(const uint256_t& other);
    uint256_t operator%=(const uint256_t& other);

    uint256_t operator&(const uint256_t& other) const;
    uint256_t operator|(const uint256_t& other) const;
    uint256_t operator^(const uint256_t& other) const;
    uint256_t operator~() const;

    uint256_t operator<<(const int bit) const;
    uint256_t operator>>(const int bit) const;

    uint256_t operator<<=(const int bit);
    uint256_t operator>>=(const int bit);

    bool operator<(const uint256_t& other) const;
    bool operator>(const uint256_t& other) const;
    bool operator<=(const uint256_t& other) const;
    bool operator>=(const uint256_t& other) const;
    bool operator==(const uint256_t& other) const;
    bool operator!=(const uint256_t& other) const;

    void print();
};