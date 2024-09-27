#include "../include/uint256_t.h"

#define assert(x) if (!(x)) { std::cerr << __LINE__ << ": Assertion " << #x << " failed." << std::endl; exit(-1); }

// constructor

uint256_t::uint256_t() : val{0} {}
uint256_t::uint256_t(uint64_t value) : val{0}
{
    val[0] = value & ((1 << 16) - 1);
    val[1] = (value >> 16) & ((1 << 16) - 1);
    val[2] = (value >> 32) & ((1 << 16) - 1);
    val[3] = (value >> 48) & ((1 << 16) - 1);
}

// comparator

bool uint256_t::operator<(const uint256_t& other) const 
{ 
    for (int i = EIGHT - 1; i >= 0; i--) {
        if (val[i] < other.val[i]) return true; 
        else if (val[i] > other.val[i]) return false;
    }
    return false;
}
bool uint256_t::operator>(const uint256_t& other) const { return other < (*this); }
bool uint256_t::operator<=(const uint256_t& other) const { return !((*this) > other); }
bool uint256_t::operator>=(const uint256_t& other) const { return !((*this) < other); }
bool uint256_t::operator==(const uint256_t& other) const
{
    for (int i = EIGHT - 1; i >= 0; i--) if (val[i] != other.val[i]) return false;
    return true;
}
bool uint256_t::operator!=(const uint256_t& other) const { return !((*this) == other); }

// bit-wise operator

uint256_t uint256_t::operator&(const uint256_t& other) const
{
    uint256_t res;
    for (int i = 0; i < EIGHT; i++) res.val[i] = val[i] & other.val[i];
    return res;
}
uint256_t uint256_t::operator|(const uint256_t& other) const
{
    uint256_t res;
    for (int i = 0; i < EIGHT; i++) res.val[i] = val[i] | other.val[i];
    return res;
}
uint256_t uint256_t::operator^(const uint256_t& other) const
{
    uint256_t res;
    for (int i = 0; i < EIGHT; i++) res.val[i] = val[i] ^ other.val[i];
    return res;
}
uint256_t uint256_t::operator~() const
{
    uint256_t res;
    for (int i = 0; i < EIGHT; i++) res.val[i] = ~val[i];
    return res;
}

uint256_t uint256_t::operator<<(const int bit) const
{
    int whl = bit >> 4, sml = bit & ((1 << 4) - 1);
    uint256_t res;
    for (int i = EIGHT - 1; i >= whl; i--) res.val[i] = val[i - whl];
    uint16_t trans = 0;
    for (int i = whl; i < EIGHT; i++) {
        uint16_t new_trans = (res.val[i] & ~((1 << (16 - sml)) - 1)) >> (16 - sml);
        res.val[i] = (res.val[i] << sml) | trans;
        trans = new_trans;
    }
    return res;
}
uint256_t uint256_t::operator>>(const int bit) const
{
    int whl = bit >> 4, sml = bit & ((1 << 4) - 1);
    uint256_t res;
    for (int i = 0; i < EIGHT - whl; i++) res.val[i] = val[i + whl];
    uint16_t trans = 0;
    for (int i = EIGHT - whl - 1; i >= 0; i--) {
        uint16_t new_trans = (res.val[i] & ((1 << sml) - 1)) << (16 - sml);
        res.val[i] = (res.val[i] << sml) | trans;
        trans = new_trans;
    }
    return res;
}

uint256_t uint256_t::operator<<=(const int bit) { return (*this) = (*this) << bit; }
uint256_t uint256_t::operator>>=(const int bit) { return (*this) = (*this) >> bit; }

// ordinary operation

uint256_t uint256_t::operator+(const uint256_t& other) const
{
    uint16_t carry = 0;
    uint256_t res;
    for (int i = 0; i < EIGHT; i++) {
        uint32_t sum = (uint32_t)val[i] + other.val[i] + carry;
        carry = sum >> 16, res.val[i] = sum;
    }
    return res;
}
uint256_t uint256_t::operator+=(const uint256_t& other) { return (*this) = (*this) + other; }

uint256_t uint256_t::operator-(const uint256_t& other) const { return (*this) + (~other) + 1; }
uint256_t uint256_t::operator-=(const uint256_t& other) { return (*this) = (*this) - other; }

uint256_t uint256_t::operator*(const uint256_t& other) const
{
    uint256_t res;
    for (int i = 0; i < EIGHT; i++) {
        uint16_t carry = 0;
        for (int j = 0; i + j < EIGHT; j++) {
            uint32_t sum = res.val[i + j] + (uint32_t)val[i] * other.val[j] + carry;
            carry = sum >> 16, res.val[i + j] = sum;
        }
    }
    return res;
}
uint256_t uint256_t::operator*=(const uint256_t& other) { return (*this) = (*this) * other; }

uint256_t uint256_t::operator/(const uint256_t& other) const
{
    assert(other != 0);
    if ((*this) < other) return 0;
    int la = 7, lb = 7;
    while (val[la] == 0) la--;
    while (other.val[lb] == 0) lb--;
    int diff = la - lb;

    uint256_t quotient, remainder;
    for (int i = lb; i >= 0; i--) remainder.val[i] = val[i + diff];
    uint256_t power[16]; power[0] = other;
    int powerlimit = 16;
    for (int i = 1; i < 16; i++) {
        power[i] = power[i - 1] + power[i - 1];
        if (power[i] < power[i - 1]) {
            powerlimit = i;
            break;
        }
    }
    for (int i = diff; i >= 0; i--) {
        uint256_t sum;
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
    return quotient;
}
uint256_t uint256_t::operator/=(const uint256_t& other) { return (*this) = (*this) / other; }

uint256_t uint256_t::operator%(const uint256_t& other) const
{
    assert(other != 0);
    if ((*this) < other) return *this;
    int la = 7, lb = 7;
    while (val[la] == 0) la--;
    while (other.val[lb] == 0) lb--;
    int diff = la - lb;

    uint256_t quotient, remainder;
    for (int i = lb; i >= 0; i--) remainder.val[i] = val[i + diff];
    uint256_t power[16]; power[0] = other;
    int powerlimit = 16;
    for (int i = 1; i < 16; i++) {
        power[i] = power[i - 1] + power[i - 1];
        if (power[i] < power[i - 1]) {
            powerlimit = i;
            break;
        }
    }
    for (int i = diff; i >= 0; i--) {
        uint256_t sum;
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
    return remainder;
}
uint256_t uint256_t::operator%=(const uint256_t& other) { return (*this) = (*this) % other; }

void uint256_t::print()
{
    for (int i = EIGHT - 1; i >= 0; i--) std::cout << std::setw(4) << std::setfill('0') << std::hex << val[i];
    std::cout << std::endl;
}