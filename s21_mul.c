#include "../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = OK;
  s21_init_decimal(result);
  if (!s21_is_zero(value_1) && !s21_is_zero(value_2)) {
    int scale = s21_calc_scale(value_1) + s21_calc_scale(value_2);
    if (s21_is_negative(value_1) ^ s21_is_negative(value_2))
      s21_negate(*result, result);
    s21_decimal res;
    s21_init_decimal(&res);
    res.bits[3] = result->bits[3];
    while (!s21_is_zero(value_2)) {
      if (s21_get_bit(value_2, 0)) {
        int flag = s21_simple_add(*result, value_1, &res);
        if (flag) {
          code = s21_is_negative(*result) ? TOO_SMALL : TOO_LARGE;
          break;
        }
        s21_copy(res, result);
        res.bits[0] = res.bits[1] = res.bits[2] = 0;
      }
      s21_shift_right(&value_1);
      s21_shift_left(&value_2);
    }
    s21_set_scale(result, scale);
  }
  return code;
}
