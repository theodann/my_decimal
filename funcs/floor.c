#include "../decimal.h"

int dec_floor(decimal value, decimal *result) {
  dec_init_decimal(result);
  int ret = 0;
  if (!dec_check_valid_decimal(value)) {
    ret = 1;
  } else {
    dec_magic(&value);
    float x = 0;
    int p = 0, c = 0;
    int n = dec_get_n(value);
    result->bits[2] = 0;
    result->bits[1] = 0;
    if (n >= 10) {
      ret = dec_from_decimal_to_float(value, &x);
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
    if (fmod(x, 1) != 0) p++;
    x = x - fmod(x, 1);
    if (dec_is_negative(value) && p == 1) x++;
    if (c == 0) result->bits[0] = dec_dec(x);
    if (c == 1) ret = dec_from_float_to_decimal(x, result);
    result->bits[3] = value.bits[3];
    dec_set_scale(result, 0);
  }
  return ret;
}
