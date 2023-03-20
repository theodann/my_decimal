#include "../decimal.h"

int dec_sub(decimal val1, decimal val2, decimal *result) {
  int code = 0;
  dec_init_decimal(result);
  if (dec_is_negative(val1) || dec_is_negative(val2)) {
    int m = 0;
    if (dec_is_negative(val1) && dec_is_negative(val2)) {
      dec_negate(val2, &val2);
    } else if (dec_is_negative(val1)) {
      dec_negate(val1, &val1);
      m++;
    } else if (dec_is_negative(val2)) {
      dec_negate(val2, &val2);
    }
    code = dec_add(val1, val2, result);
    if (m && !dec_is_negative(*result)) dec_negate(*result, result);
  } else {
    if (!dec_is_equal(val1, val2)) {
      int norm = dec_normalisation(&val1, &val2);
      if (norm < 0)
        code = dec_big_sub(val1, val2, result);
      else
        code = dec_simple_sub(val1, val2, result);
      if (norm >= 0)
        dec_set_scale(result, norm);
      else
        code = 2;
      if (code == 0) dec_magic(result);
    }
  }
  return code;
}

int dec_simple_sub(decimal val1, decimal val2, decimal *res) {
  int who_is_bigger = dec_is_greater(val1, val2);
  for (int i = 0; i < 96; i++)
    dec_inverse_bit(who_is_bigger ? &val1 : &val2, i);
  int code = dec_simple_add(val1, val2, res);
  for (int i = 0; i < 96; i++) dec_inverse_bit(res, i);
  if (!who_is_bigger) dec_negate(*res, res);
  return code;
}

int dec_big_sub(decimal src1, decimal src2, decimal *res) {
  big_decimal a, b, result;
  float s1 = 0.0, s2 = 0.0;
  dec_from_decimal_to_float(src1, &s1);
  dec_from_decimal_to_float(src2, &s2);
  dec_to_big(src1, &a);
  dec_to_big(src2, &b);
  dec_init_big(&result);
  int n = big_norm(&a, &b);
  if (s1 >= s2)
    big_minus(a, b, &result);
  else
    big_minus(b, a, &result);
  int ret = dec_from_big(result, res);
  res->bits[3] = 0;
  dec_set_scale(res, n);
  if (s1 < s2) dec_negate(*res, res);
  return ret;
}
