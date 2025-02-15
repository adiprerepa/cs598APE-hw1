#ifndef __BIT_HACK_H__
#define __BIT_HACK_H__

#include <cstdint>
#include <cstring>  // For memcpy

inline bool xor_sign_bit(const double a, const double b) {
    uint64_t a_bits, b_bits;

    // Convert double to uint64_t
    std::memcpy(&a_bits, &a, sizeof(double));
    std::memcpy(&b_bits, &b, sizeof(double));

    // Extract sign bits (MSB is bit 63)
    uint64_t sign_a = (a_bits >> 63) & 1;
    uint64_t sign_b = (b_bits >> 63) & 1;

    // Return XOR of sign bits
    return sign_a ^ sign_b;
}

#endif
