#include "../decimal.h"

int dec_is_equal(decimal a, decimal b) {
  int code = TRUE;
  if ((dec_is_zero(a) ^ dec_is_zero(b))) return FALSE;
  if ((dec_is_zero(a) && dec_is_zero(b))) return TRUE;
  dec_magic(&a);
  dec_magic(&b);
  for (int i = 0; i < 4; i++)
    if (a.bits[i] != b.bits[i]) {
      code = FALSE;
      break;
    }
  return code;
}
