#include "../decimal.h"

int dec_is_less(decimal a, decimal b) {
  return !(dec_is_greater_or_equal(a, b));
}
