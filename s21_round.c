#include "../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int ret = !s21_check_valid_decimal(value);
  s21_init_decimal(result);
  s21_magic(&value);
  int scale = s21_calc_scale(value);
  if (!ret) {
    if (scale > 0) {
      s21_decimal five = {{5, 0, 0, 0}};
      s21_set_scale(&five, 1);
      if (s21_is_negative(value))
        s21_sub(value, five, result);
      else
        s21_add(value, five, result);
      ret = s21_truncate(*result, result);
    } else {
      *result = value;
    }
  }
  return ret;
}
