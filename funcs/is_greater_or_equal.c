#include "../decimal.h"

int dec_is_greater_or_equal(decimal a, decimal b) {
  return dec_is_equal(a, b) || dec_is_greater(a, b);
}
