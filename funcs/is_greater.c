#include "../decimal.h"

int dec_is_greater(decimal a, decimal b) {
  int code = -1;
  if (dec_is_negative(a) && !dec_is_negative(b)) code = 0;
  if (!dec_is_negative(a) && dec_is_negative(b)) code = 1;
  if (dec_is_equal(a, b)) code = 0;
  if (code == -1) {
    dec_magic(&a);
    dec_magic(&b);
    dec_normalisation(&a, &b);
    code = 1;
    for (int i = 95; i > -1; i--) {
      if (dec_get_bit(a, i) < dec_get_bit(b, i)) {
        code = 0;
        break;
      }
      if (dec_get_bit(a, i) > dec_get_bit(b, i)) {
        break;
      }
    }
    if (dec_is_negative(a) && dec_is_negative(b)) code = !code;
  }
  return code;
}
