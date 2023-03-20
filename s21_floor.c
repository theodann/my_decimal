#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  s21_init_decimal(result);
  int ret = 0;
  if (!s21_check_valid_decimal(value)) {
    ret = 1;
  } else {
    s21_magic(&value);
    float x = 0;
    int p = 0, c = 0;
    int n = s21_get_n(value);
    result->bits[2] = 0;
    result->bits[1] = 0;
    if (n >= 10) {
      ret = s21_from_decimal_to_float(value, &x);
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
    if (fmod(x, 1) != 0) p++;
    x = x - fmod(x, 1);
    if (s21_is_negative(value) && p == 1) x++;
    if (c == 0) result->bits[0] = s21_dec(x);
    if (c == 1) ret = s21_from_float_to_decimal(x, result);
    result->bits[3] = value.bits[3];
    s21_set_scale(result, 0);
  }
  return ret;
}
