#include "../decimal.h"

int dec_is_not_equal(decimal a, decimal b) {
  return !(dec_is_equal(a, b));
}
