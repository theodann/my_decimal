#include "../decimal.h"

int dec_mul(decimal value_1, decimal value_2, decimal *result) {
  int code = OK;
  dec_init_decimal(result);
  if (!dec_is_zero(value_1) && !dec_is_zero(value_2)) {
    int scale = dec_calc_scale(value_1) + dec_calc_scale(value_2);
    if (dec_is_negative(value_1) ^ dec_is_negative(value_2))
      dec_negate(*result, result);
    decimal res;
    dec_init_decimal(&res);
    res.bits[3] = result->bits[3];
    while (!dec_is_zero(value_2)) {
      if (dec_get_bit(value_2, 0)) {
        int flag = dec_simple_add(*result, value_1, &res);
        if (flag) {
          code = dec_is_negative(*result) ? TOO_SMALL : TOO_LARGE;
          break;
        }
        dec_copy(res, result);
        res.bits[0] = res.bits[1] = res.bits[2] = 0;
      }
      dec_shift_right(&value_1);
      dec_shift_left(&value_2);
    }
    dec_set_scale(result, scale);
  }
  return code;
}
