#include "../decimal.h"

int dec_mod(decimal value_1, decimal value_2, decimal *result) {
  int code = DONE;
  dec_init_decimal(result);
  if (dec_is_zero(value_2)) {
    code = DIV_BY_ZERO;
  } else {
    decimal temp = {0};
    dec_div(value_1, value_2, &temp);
    dec_truncate(temp, &temp);
    dec_mul(temp, value_2, &temp);
    dec_sub(value_1, temp, result);
    dec_magic(result);
  }
  return code;
}
