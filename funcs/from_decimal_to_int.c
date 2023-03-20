#include "../decimal.h"

int dec_from_decimal_to_int(decimal src, int *dst) {
  float x = 0;
  int code = dec_from_decimal_to_float(src, &x);
  x = trunc(x);
  if (fabs(x) > INT_MAX) {
    code = CONVERT_ERR;
  } else {
    *dst = (int)x;
  }
  return code;
}
