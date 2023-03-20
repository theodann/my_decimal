#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  float x = 0;
  int code = s21_from_decimal_to_float(src, &x);
  x = trunc(x);
  if (fabs(x) > INT_MAX) {
    code = CONVERT_ERR;
  } else {
    *dst = (int)x;
  }
  return code;
}
