#include "../decimal.h"

int dec_is_less_or_equal(decimal a, decimal b) {
  return dec_is_equal(a, b) || dec_is_less(a, b);
}
