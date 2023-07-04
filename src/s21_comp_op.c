#include "s21_decimal.h"
#include <math.h>

// Comparison Operators

int s21_is_less(s21_decimal value1, s21_decimal value2) {
    // If value1 is less return 1 else 0
    return (value1.bits[0] < value2.bits[0]) ? TRUE : FALSE;
}

int s21_is_equal(s21_decimal dec1, s21_decimal dec2)
{
    for (int i = 0; i < 4; i++) {
        if (dec1.bits[i] != dec2.bits[i]) {
            return FALSE;  // Not equal
        }
    }
    
    return TRUE;  // Equal
}
