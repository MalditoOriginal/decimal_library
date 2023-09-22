#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  // Сравниваем целые части
  if (value_1.intPart < value_2.intPart) {
    return TRUE;
  }
  // Если целые части равны, сравниваем дробные части
  else if (value_1.intPart == value_2.intPart) {
    if (value_1.fractPart < value_2.fractPart) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  // Сравниваем целые части
  if (value_1.intPart < value_2.intPart) {
    return TRUE;
  }
  // Если целые части равны, сравниваем дробные части
  else if (value_1.intPart == value_2.intPart) {
    if (value_1.fractPart <= value_2.fractPart) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  // Сравниваем целые части
  if (value_1.intPart > value_2.intPart) {
    return TRUE;
  }
  // Если целые части равны, сравниваем дробные части
  else if (value_1.intPart == value_2.intPart) {
    if (value_1.fractPart > value_2.fractPart) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  // Сравниваем целые части
  if (value_1.intPart > value_2.intPart) {
    return TRUE;
  }
  // Если целые части равны, сравниваем дробные части
  else if (value_1.intPart == value_2.intPart) {
    if (value_1.fractPart >= value_2.fractPart) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  // Сравниваем целые и дробные части
  if (value_1.intPart == value_2.intPart && value_1.fractPart == value_2.fractPart) {
    return TRUE;
  } else {
    return FALSE;
  }
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  // Сравниваем целые и дробные части
  if (value_1.intPart != value_2.intPart || value_1.fractPart != value_2.fractPart) {
    return TRUE;
  } else {
    return FALSE;
  }
}
