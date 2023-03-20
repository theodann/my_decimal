#include "../decimal.h"

int dec_from_decimal_to_float(decimal src, float *dst) {
  *dst = 0;
  double x = 0.0;
  int code = DONE, scale = dec_calc_scale(src);
  if (!dec_check_valid_decimal(src)) code = CONVERT_ERR;
  if (code == DONE) {
    x = dec_seg(src.bits[0]) + LONG_MAX * dec_seg(src.bits[1]) +
        LONG_LONG_MAX * dec_seg(src.bits[2]);
    x /= pow(10, scale);
    if (dec_is_negative(src)) x *= -1;
  }
  *dst = (float)x;
  return code;
}
