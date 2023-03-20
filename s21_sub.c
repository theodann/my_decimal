#include "../s21_decimal.h"

int s21_sub(s21_decimal val1, s21_decimal val2, s21_decimal *result) {
  int code = 0;
  s21_init_decimal(result);
  if (s21_is_negative(val1) || s21_is_negative(val2)) {
    int m = 0;
    if (s21_is_negative(val1) && s21_is_negative(val2)) {
      s21_negate(val2, &val2);
    } else if (s21_is_negative(val1)) {
      s21_negate(val1, &val1);
      m++;
    } else if (s21_is_negative(val2)) {
      s21_negate(val2, &val2);
    }
    code = s21_add(val1, val2, result);
    if (m && !s21_is_negative(*result)) s21_negate(*result, result);
  } else {
    if (!s21_is_equal(val1, val2)) {
      int norm = s21_normalisation(&val1, &val2);
      if (norm < 0)
        code = s21_big_sub(val1, val2, result);
      else
        code = s21_simple_sub(val1, val2, result);
      if (norm >= 0)
        s21_set_scale(result, norm);
      else
        code = 2;
      if (code == 0) s21_magic(result);
    }
  }
  return code;
}

int s21_simple_sub(s21_decimal val1, s21_decimal val2, s21_decimal *res) {
  int who_is_bigger = s21_is_greater(val1, val2);
  for (int i = 0; i < 96; i++)
    s21_inverse_bit(who_is_bigger ? &val1 : &val2, i);
  int code = s21_simple_add(val1, val2, res);
  for (int i = 0; i < 96; i++) s21_inverse_bit(res, i);
  if (!who_is_bigger) s21_negate(*res, res);
  return code;
}

int s21_big_sub(s21_decimal src1, s21_decimal src2, s21_decimal *res) {
  big_decimal a, b, result;
  float s1 = 0.0, s2 = 0.0;
  s21_from_decimal_to_float(src1, &s1);
  s21_from_decimal_to_float(src2, &s2);
  s21_to_big(src1, &a);
  s21_to_big(src2, &b);
  s21_init_big(&result);
  int n = big_norm(&a, &b);
  if (s1 >= s2)
    big_minus(a, b, &result);
  else
    big_minus(b, a, &result);
  int ret = s21_from_big(result, res);
  res->bits[3] = 0;
  s21_set_scale(res, n);
  if (s1 < s2) s21_negate(*res, res);
  return ret;
}
