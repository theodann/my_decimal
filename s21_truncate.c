#include "../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int ret = !s21_check_valid_decimal(value);
  float x = 0.0;
  s21_magic(&value);
  s21_init_decimal(result);
  if (!ret) {
    int c = 0;
    int n = s21_get_n(value);
    if (n >= 10) {
      s21_from_decimal_to_float(value, &x);
      x = fabs(x);
      c = 1;
    } else if (n != 0) {
      result->bits[1] = value.bits[1];
      result->bits[2] = value.bits[2];
      x = s21_seg(value.bits[0]) / pow(10, n);
    } else {
      result->bits[0] = value.bits[0];
      result->bits[1] = value.bits[1];
      result->bits[2] = value.bits[2];
      c = -1;
    }
    x = trunc(x);
    if (c == 0) result->bits[0] = s21_dec(x);
    if (c == 1) ret = s21_from_float_to_decimal(x, result);
    s21_set_scale(result, 0);
    if (s21_is_negative(value)) s21_negate(*result, result);
  }
  return ret;
}
