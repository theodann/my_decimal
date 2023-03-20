#include "../decimal.h"

int dec_div(decimal value_1, decimal value_2, decimal *result) {
  int code = OK;
  int p = 0;
  dec_init_decimal(result);
  if (dec_is_zero(value_2)) {
    code = DIV_BY_ZERO;
  } else if (!dec_is_zero(value_1)) {
    p = value_1.bits[1] + value_1.bits[2] + value_2.bits[1] + value_2.bits[2];
    if (p == 0) {
      float res = (float)value_1.bits[0] / (float)value_2.bits[0];
      int n = 0;
      while (fmod(res, 1) != 0) {
        res *= 10;
        n++;
        if (n == 28) break;
      }
      result->bits[0] = (int)res;
      dec_set_scale(result, n);
    } else {
      big_decimal a, b, res;
      dec_init_big(&res);
      dec_to_big(value_1, &a);
      dec_to_big(value_2, &b);
      big_norm(&a, &b);
      big_div(a, b, &res);
      code = dec_from_big(res, result);
      int check = 0;
      dec_from_decimal_to_int(*result, &check);
      if (dec_is_not_equal(value_1, value_2) && check == 1) {
        float x, y, f;
        dec_from_decimal_to_float(value_1, &x);
        dec_from_decimal_to_float(value_2, &y);
        f = x / y;
        dec_from_float_to_decimal(f, result);
      }
    }
  }
  p = dec_is_negative(value_1) + dec_is_negative(value_2);
  if (p == 1 && !dec_is_negative(*result)) dec_negate(*result, result);
  if (p == 1 && code == 1) code = 2;
  if (code == 0) dec_magic(result);
  return code;
}
