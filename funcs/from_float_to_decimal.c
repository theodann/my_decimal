#include "../decimal.h"

int dec_from_float_to_decimal(float src, decimal *dst) {
  int ret = 0;
  dec_init_decimal(dst);
  if (fabs(src) == inf) {
    ret = 1;
  } else {
    if (src < 0) {
      dec_negate(*dst, dst);
      src = -src;
    }
    if (src > 0 && src <= min) {
      ret = 1;
    } else {
      int n = dec_count_mant(src);
      if (n > 6) n = 6;
      float x = (float)(src * pow(10, n));
      x = round(x);
      if (fabs(x) > 79228162514264337593543950335.0) {
        ret = 1;
      } else {
        if (x >= LONG_LONG_MAX) {
          dst->bits[0] = dec_dec(x - LONG_LONG_MAX);
          dst->bits[2] = dec_dec(fmod(x, LONG_LONG_MAX));
          dst->bits[1] = dec_dec(fmod((x - dec_seg(dst->bits[2])), LONG_MAX));
        } else if (x >= LONG_MAX) {
          dst->bits[0] = dec_dec(x - LONG_MAX);
          dst->bits[1] = dec_dec(fmod(x, LONG_MAX));
        } else {
          dst->bits[0] = dec_dec(x);
        }
        dec_set_scale(dst, n);
        dec_magic(dst);
      }
    }
  }
  return ret;
}
