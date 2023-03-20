#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  *dst = 0;
  double x = 0.0;
  int code = DONE, scale = s21_calc_scale(src);
  if (!s21_check_valid_decimal(src)) code = CONVERT_ERR;
  if (code == DONE) {
    x = s21_seg(src.bits[0]) + LONG_MAX * s21_seg(src.bits[1]) +
        LONG_LONG_MAX * s21_seg(src.bits[2]);
    x /= pow(10, scale);
    if (s21_is_negative(src)) x *= -1;
  }
  *dst = (float)x;
  return code;
}
