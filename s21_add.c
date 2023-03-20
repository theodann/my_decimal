#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;
  int sub = 0;
  int m = 0;
  s21_init_decimal(result);
  if (s21_is_negative(value_1) && s21_is_negative(value_2)) {
    s21_negate(value_1, &value_1);
    s21_negate(value_2, &value_2);
    m++;
  } else if (s21_is_negative(value_2) && !s21_is_negative(value_1)) {
    s21_negate(value_2, &value_2);
    code = s21_sub(value_1, value_2, result);
    sub++;
  } else if (s21_is_negative(value_1) && !s21_is_negative(value_2)) {
    s21_negate(value_1, &value_1);
    code = s21_sub(value_2, value_1, result);
    sub++;
  }
  if (!sub) {
    int norm = s21_normalisation(&value_1, &value_2);
    if (norm >= 0) {
      int flag = s21_simple_add(value_1, value_2, result);
      code = s21_check_result(result, flag);
      s21_set_scale(result, norm);
    }
    if (norm < 0) {
      code = s21_big_add(value_1, value_2, result);
    }
    if (code == 0) s21_magic(result);
    if (m && !s21_is_negative(*result)) s21_negate(*result, result);
    if (m && code == 1) code = 2;
  }
  return code;
}

int s21_simple_add(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result) {
  int flag = 0;
  for (int i = 0; i < 96; i++) {
    int add = s21_get_bit(value_1, i) + s21_get_bit(value_2, i) + flag;
    flag = 0;
    if (add == 1) {
      s21_change_bit(result, i, 1);
    } else if (add == 2) {
      flag = 1;
    } else if (add == 3) {
      s21_change_bit(result, i, 1);
      flag = 1;
    }
  }
  return flag;
}

int s21_check_result(s21_decimal *result, int flag) {
  int code = OK;
  if (flag == 1 && !s21_is_negative(*result)) {
    code = TOO_LARGE;
  }
  if (flag == 1 && s21_is_negative(*result)) {
    code = TOO_SMALL;
  }
  return code;
}

int s21_big_add(s21_decimal src1, s21_decimal src2, s21_decimal *res) {
  big_decimal a, b, result;
  s21_init_decimal(res);
  s21_to_big(src1, &a);
  s21_to_big(src2, &b);
  s21_init_big(&result);
  big_plus(a, b, &result);
  return s21_from_big(result, res);
}
