#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_init_decimal(dst);
  if (src < 0) {
    src *= -1;
    s21_negate(*dst, dst);
  }
  dst->bits[0] = src;
  return DONE;
}
