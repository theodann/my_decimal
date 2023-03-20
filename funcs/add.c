#include "../decimal.h"

int dec_add(decimal value_1, decimal value_2, decimal *result) {
  int code = 0;
  int sub = 0;
  int m = 0;
  dec_init_decimal(result);
  if (dec_is_negative(value_1) && dec_is_negative(value_2)) {
    dec_negate(value_1, &value_1);
    dec_negate(value_2, &value_2);
    m++;
  } else if (dec_is_negative(value_2) && !dec_is_negative(value_1)) {
    dec_negate(value_2, &value_2);
    code = dec_sub(value_1, value_2, result);
    sub++;
  } else if (dec_is_negative(value_1) && !dec_is_negative(value_2)) {
    dec_negate(value_1, &value_1);
    code = dec_sub(value_2, value_1, result);
    sub++;
  }
  if (!sub) {
    int norm = dec_normalisation(&value_1, &value_2);
    if (norm >= 0) {
      int flag = dec_simple_add(value_1, value_2, result);
      code = dec_check_result(result, flag);
      dec_set_scale(result, norm);
    }
    if (norm < 0) {
      code = dec_big_add(value_1, value_2, result);
    }
    if (code == 0) dec_magic(result);
    if (m && !dec_is_negative(*result)) dec_negate(*result, result);
    if (m && code == 1) code = 2;
  }
  return code;
}

int dec_simple_add(decimal value_1, decimal value_2,
                   decimal *result) {
  int flag = 0;
  for (int i = 0; i < 96; i++) {
    int add = dec_get_bit(value_1, i) + dec_get_bit(value_2, i) + flag;
    flag = 0;
    if (add == 1) {
      dec_change_bit(result, i, 1);
    } else if (add == 2) {
      flag = 1;
    } else if (add == 3) {
      dec_change_bit(result, i, 1);
      flag = 1;
    }
  }
  return flag;
}

int dec_check_result(decimal *result, int flag) {
  int code = OK;
  if (flag == 1 && !dec_is_negative(*result)) {
    code = TOO_LARGE;
  }
  if (flag == 1 && dec_is_negative(*result)) {
    code = TOO_SMALL;
  }
  return code;
}

int dec_big_add(decimal src1, decimal src2, decimal *res) {
  big_decimal a, b, result;
  dec_init_decimal(res);
  dec_to_big(src1, &a);
  dec_to_big(src2, &b);
  dec_init_big(&result);
  big_plus(a, b, &result);
  return dec_from_big(result, res);
}
