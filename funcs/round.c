#include "../decimal.h"

int dec_round(decimal value, decimal *result) {
  int ret = !dec_check_valid_decimal(value);
  dec_init_decimal(result);
  dec_magic(&value);
  int scale = dec_calc_scale(value);
  if (!ret) {
    if (scale > 0) {
      decimal five = {{5, 0, 0, 0}};
      dec_set_scale(&five, 1);
      if (dec_is_negative(value))
        dec_sub(value, five, result);
      else
        dec_add(value, five, result);
      ret = dec_truncate(*result, result);
    } else {
      *result = value;
    }
  }
  return ret;
}
