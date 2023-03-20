#include "../s21_decimal.h"

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int code = TRUE;
  if ((s21_is_zero(a) ^ s21_is_zero(b))) return FALSE;
  if ((s21_is_zero(a) && s21_is_zero(b))) return TRUE;
  s21_magic(&a);
  s21_magic(&b);
  for (int i = 0; i < 4; i++)
    if (a.bits[i] != b.bits[i]) {
      code = FALSE;
      break;
    }
  return code;
}
