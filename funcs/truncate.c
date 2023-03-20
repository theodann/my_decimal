#include "../decimal.h"

int dec_truncate(decimal value, decimal *result) {
  int ret = !dec_check_valid_decimal(value);
  float x = 0.0;
  dec_magic(&value);
  dec_init_decimal(result);
  if (!ret) {
    int c = 0;
    int n = dec_get_n(value);
    if (n >= 10) {
      dec_from_decimal_to_float(value, &x);
      x = fabs(x);
      c = 1;
    } else if (n != 0) {
      result->bits[1] = value.bits[1];
      result->bits[2] = value.bits[2];
      x = dec_seg(value.bits[0]) / pow(10, n);
    } else {
      result->bits[0] = value.bits[0];
      result->bits[1] = value.bits[1];
      result->bits[2] = value.bits[2];
      c = -1;
    }
    x = trunc(x);
    if (c == 0) result->bits[0] = dec_dec(x);
    if (c == 1) ret = dec_from_float_to_decimal(x, result);
    dec_set_scale(result, 0);
    if (dec_is_negative(value)) dec_negate(*result, result);
  }
  return ret;
}
