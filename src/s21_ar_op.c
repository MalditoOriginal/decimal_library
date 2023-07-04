#include "s21_decimal.h"
#include <math.h>
#include <stdint.h>

// Arithmetic Operators

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
     int carry;
     uint64_t sum = 0;

     for (int i = 0; i < 4; i++) {
         sum = value_1.bits[i] + value_2.bits[i] + carry;
         result->bits[i] = sum % 10;
         carry = sum / 10;
     }

     // Handle carry beyond the bits array
     if (carry > 0) return NUM_TOO_LARGE;

     // Check if the result exceeds the maximum value
     uint64_t max_value = 1;
     for (int i = 0; i < 4; i++) max_value *= 10;
     if (result->intPart >= max_value) return NUM_TOO_LARGE;

     // Apply bank rounding if the result does not fit into the mantissa
    if (carry > 0) {
        uint64_t rounding = 0;
        for (int i = 3; i >= 0; i--) {
            rounding = rounding * 10 + result->bits[i];
            result->bits[i] = rounding / 10;
            rounding %= 10;
        }
    }

     return OK;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    int borrow = 0;

    for (int i = 0; i < 4; i++) {
        int diff = value_1.bits[i] - value_2.bits[i] - borrow;
        borrow = (diff < 0) ? 1 : 0;
        result->bits[i] = (diff < 0) ? (diff += 10) : diff;
    }

    // Check if the borrowing is required from higher-order digits
    if (borrow > 0) return NUM_TOO_SMALL;

    return OK;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    uint64_t product = value_1.intPart * value_2.intPart;
    product += value_1.intPart * value_2.fractPart / (1 << value_2.scale);
    product += value_2.intPart * value_1.fractPart / (1 << value_1.scale);
    product += value_1.fractPart * value_2.fractPart / ((uint64_t)1 << (value_1.scale + value_2.scale));

    // Check for multiplication overflow
    uint64_t max_value = 1;
    for (int i = 0; i < 4; i++) max_value *=10;
    if (product >= max_value) return NUM_TOO_LARGE;

    // Assign the product to the result structure
    for (int i = 0; i < 4; i++) {
        result->bits[i] = product % 10;
        product /= 10;
    }
    return OK;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    // Check for division by zero
    if (value_2.intPart == 0 && value_2.fractPart == 0) return DIV_BY_ZERO;

    // Convert the dividend and divisor to a common scale
    int scale = value_1.scale - value_2.scale;
    uint64_t dividend = (value_1.intPart << scale) + (value_1.fractPart >> (21 - scale));
    uint64_t divisor = value_2.intPart;
    if (value_2.fractPart != 0) divisor += value_2.fractPart >> (21 - scale);

    // Perform the division
    uint64_t quotient = dividend / divisor;
    uint64_t remainder = dividend % divisor;

    // Check for division overflow
    uint64_t max_value = 1;
    for (int i = 0; i < 4; i++) max_value *= 10;
    if (quotient >= max_value) return NUM_TOO_LARGE;

    // Assign the quotent to the result structure
    for (int i = 0; i < 4; i++) {
        result->bits[i] = quotient % 10;
        quotient /= 10;
    }

    // Check for overflow
    if (quotient > 0) return NUM_TOO_LARGE;

    // Check for underflow
    if (remainder != 0) return NUM_TOO_SMALL;

    // Set the scale for the result
    result->scale = scale;

    return OK;
}