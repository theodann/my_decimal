#include "../decimal.h"

int dec_negate(decimal value, decimal *result) {
  dec_init_decimal(result);
  dec_copy(value, result);
  dec_inverse_bit(result, 127);
  return DONE;
}
