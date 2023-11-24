#include "s21_decimal.h"

int get_bit(s21_decimal num, int bit) {
  if (bit < 0 || bit >= 128) {
    // Invalid bit position
    return 0;
  }

  unsigned int mask = 1u << (bit % 32);
  return (num.bits[bit / 32] & mask) != 0;
}

void set_bit(s21_decimal *num, int bit, int value) {
  if (bit < 0 || bit >= 128) {
    // Invalid bit position
    return;
  }

  unsigned int mask = 1u << (bit % 32);
  int index = bit / 32;

  if (value == TRUE) {
    num->bits[index] |= mask;
  } else if (value == FALSE) {
    num->bits[index] &= ~mask;
  }
}

int last_bit(s21_decimal number) {
  for (int last_bit = 95; last_bit >= 0; last_bit--) {
    if (get_bit(number, last_bit) != 0) {
      return last_bit;
    }
  }
  return -1;  // Indicates no set bit found
}

int offset_left(s21_decimal *num, int value_offset) {
  int lastbit = last_bit(*num);

  if (lastbit + value_offset > 95) {
    return INF;
  }

  for (int i = 0; i < value_offset; i++) {
    int value_31bit = get_bit(*num, 31);
    int value_63bit = get_bit(*num, 63);

    for (int j = 0; j < 3; j++) num->bits[j] <<= 1;

    if (value_31bit) set_bit(num, 32, 1);
    if (value_63bit) set_bit(num, 64, 1);
  }

  return OK;
}

void set_sign(s21_decimal *num, int sign) {
  if (sign != 0) {
    num->bits[3] |= 1u << 31;
  } else {
    num->bits[3] &= ~(1u << 31);
  }
}

int get_scale(const s21_decimal *num) {
  char scale = num->bits[3] >> 16;
  return scale;
}

void set_scale(s21_decimal *num, int scale) {
  // Clear the existing scale bits (bits 17 to 24) in num->bits[3]
  num->bits[3] &= ~(0xFFu << 16);

  // Set the new scale bits based on the provided scale
  num->bits[3] |= (scale & 0xFF) << 16;
}

void clear_bits(s21_decimal *num) {
  if (num != NULL) {
    memset(num->bits, 0, sizeof(num->bits));
  }
}

int get_sign(s21_decimal num) { return (num.bits[3] >> 31) & 1; }

void mul_by_ten(s21_decimal *num) {
  s21_decimal ten = {{10, 0, 0, 0}};
  bit_mul(*num, ten, num);
}

void div_by_ten(s21_decimal *num) {
  s21_decimal empty;
  s21_decimal ten = {{10, 0, 0, 0}};
  bit_div(*num, ten, num, &empty);
}

int get_float_sign(float src) { return src < 0; }

int get_float_exp(float *src) { return ((*(int *)src) >> 23) - 127; }

int is_equal_without_sign(s21_decimal value_1, s21_decimal value_2) {
  return (value_1.bits[2] == value_2.bits[2] &&
          value_1.bits[1] == value_2.bits[1] &&
          value_1.bits[0] == value_2.bits[0]);
}

int mod_comp(s21_decimal value_1, s21_decimal value_2) {
  int out;
  if (value_1.bits[2] < value_2.bits[2]) {
    out = TRUE;
  } else if (value_1.bits[2] > value_2.bits[2]) {
    out = FALSE;
  } else {
    if (value_1.bits[1] < value_2.bits[1]) {
      out = TRUE;
    } else if (value_1.bits[1] > value_2.bits[1]) {
      out = FALSE;
    } else {
      if (value_1.bits[0] < value_2.bits[0]) {
        out = TRUE;
      } else if (value_1.bits[0] > value_2.bits[0]) {
        out = FALSE;
      } else {
        out = FALSE;
      }
    }
  }
  return out;
}

void scale_equal(s21_decimal *value_1, s21_decimal *value_2) {
  // Check if the scale of value_1 is less than the scale of value_2
  if (get_scale(value_1) < get_scale(value_2)) {
    int difference = get_scale(value_2) - get_scale(value_1);

    // Check if the higher bits of value_2 are zero
    if (get_bit(*value_2, 93) == 0 && get_bit(*value_2, 94) == 0 &&
        get_bit(*value_2, 95) == 0) {
      // Multiply value_1 by 10 'difference' times
      for (int i = 0; i < difference; i++) {
        mul_by_ten(value_1);
      }
      // Set the scale of value_1 to the scale of value_2
      set_scale(value_1, get_scale(value_2));
    } else {
      // Divide value_2 by 10 'difference' times
      for (int i = 0; i < difference; i++) {
        div_by_ten(value_2);
      }
      // Set the scale of value_2 to the scale of value_1
      set_scale(value_2, get_scale(value_1));
    }
  } else {
    // If the scale of value_2 is less than the scale of value_1
    int difference = get_scale(value_1) - get_scale(value_2);

    // Check if the higher bits of value_1 are zero
    if (get_bit(*value_1, 93) == 0 && get_bit(*value_1, 94) == 0 &&
        get_bit(*value_1, 95) == 0) {
      // Multiply value_2 by 10 'difference' times
      for (int i = 0; i < difference; i++) {
        mul_by_ten(value_2);
      }
      // Set the scale of value_2 to the scale of value_1
      set_scale(value_2, get_scale(value_1));
    } else {
      // Divide value_1 by 10 'difference' times
      for (int i = 0; i < difference; i++) {
        div_by_ten(value_1);
      }
      // Set the scale of value_1 to the scale of value_2
      set_scale(value_1, get_scale(value_2));
    }
  }
}

int result_sign(s21_decimal value_1, s21_decimal value_2) {
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  int sign;
  if (sign_1 == sign_2) {
    sign = 0;
  } else {
    sign = 1;
  }
  return sign;
}