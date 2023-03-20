#include "../decimal.h"

int dec_from_int_to_decimal(int src, decimal *dst) {
  dec_init_decimal(dst);
  if (src < 0) {
    src *= -1;
    dec_negate(*dst, dst);
  }
  dst->bits[0] = src;
  return DONE;
}
