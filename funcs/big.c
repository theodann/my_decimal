#include "../decimal.h"

int get_big_bit(big_decimal d, int i) {
  int result = 0;
  int mask = 1;
  int val = i / 32;
  if (d.bits[val] & (mask << (i - (32 * val)))) {
    result = 1;
  }
  return result;
}

void set_big_bit(big_decimal *d, int i) {
  int mask = 1;
  int val = i / 32;
  d->bits[val] = d->bits[val] | (mask << (i - (32 * val)));
}

void clean_big_bit(big_decimal *d, int i) {
  int mask = 1;
  int val = i / 32;
  d->bits[val] = d->bits[val] & (~(mask << (i - (32 * val))));
}

void big_shift(big_decimal *a, int flag) {
  if (flag == 1) {
    for (int i = 255; i != 0; i--) {
      if (dec_get_big_bit(*a, i - 1)) {
        set_big_bit(a, i);
      } else {
        clean_big_bit(a, i);
      }
    }
    clean_big_bit(a, 0);
  } else if (flag == -1) {
    for (int i = 0; i < 256; i++) {
      if (dec_get_big_bit(*a, i + 1)) {
        set_big_bit(a, i);
      } else {
        clean_big_bit(a, i);
      }
    }
    clean_big_bit(a, 255);
  }
}

void to_big(decimal a, big_decimal *b) {
  b->bits[0] = a.bits[0];
  b->bits[1] = a.bits[1];
  b->bits[2] = a.bits[2];
  b->bits[3] = 0;
  b->bits[4] = 0;
  b->bits[5] = 0;
  b->bits[6] = 0;
  b->bits[7] = 0;
  b->n = get_n(a);
  if (dec_is_negative(a)) b->n *= -1;
}

void init_big(big_decimal *a) {
  a->bits[0] = 0;
  a->bits[1] = 0;
  a->bits[2] = 0;
  a->bits[3] = 0;
  a->bits[4] = 0;
  a->bits[5] = 0;
  a->bits[6] = 0;
  a->bits[7] = 0;
  a->n = 0;
}

int from_big(big_decimal a, decimal *b) {
  int ret = 0;
  big_decimal tmp = {0};
  int scale = abs(a.n);
  int sign = 0;
  if (a.n < 0) sign++;
  init_decimal(b);
  if (scale <= 28 && a.bits[3] == 0 && a.bits[4] == 0 && a.bits[5] == 0 &&
      a.bits[6] == 0 && a.bits[7] == 0) {
    b->bits[0] = a.bits[0];
    b->bits[1] = a.bits[1];
    b->bits[2] = a.bits[2];
    b->bits[3] = 0;
    set_scale(b, abs(a.n));
    if (sign) negate(*b, b);
    for (int i = 0; i < 3; i++)
      if (b->bits[i] < 0) b->bits[i] = -1;
  } else {
    int for_bank = 0;
    while (scale && (scale > 28 || a.bits[3] || a.bits[4] || a.bits[5] ||
                     a.bits[6] || a.bits[7])) {
      dec_scale(&a, 1, &tmp);
      a = tmp;
      for_bank = tmp.bits[0] % 10;
      init_big(&tmp);
      scale--;
    }
    if (for_bank > 5 || (for_bank == 5 && get_big_bit(a, 0))) {
      big_decimal tmp_bank = {0};
      big_decimal tmp_bank2 = {{1, 0, 0, 0, 0, 0, 0, 0}, 0};
      big_plus(a, tmp_bank2, &tmp_bank);
      tmp_bank.n = a.n;
      a = tmp_bank;
    }
    if (a.bits[3] == 0 && a.bits[4] == 0 && a.bits[5] == 0 && a.bits[6] == 0 &&
        a.bits[7] == 0) {
      b->bits[0] = a.bits[0];
      b->bits[1] = a.bits[1];
      b->bits[2] = a.bits[2];
      b->bits[3] = 0;
      set_scale(b, abs(a.n));
      if (sign) negate(*b, b);
      for (int i = 0; i < 3; i++)
        if (b->bits[i] < 0) b->bits[i] = -1;
    } else {
      ret = 1;
    }
  }
  return ret;
}

void dec_scale(big_decimal *bd, int val, big_decimal *result) {
  big_decimal bd_for_mult = {{10, 0, 0, 0, 0, 0, 0, 0}, 0};
  big_decimal bd_tmp = {0};
  int old_scale = abs(bd->n);
  for (int i = 0; i < val; i++) {
    big_div(*bd, bd_for_mult, &bd_tmp);
    *bd = bd_tmp;
    init_big(&bd_tmp);
  }
  *result = *bd;
  result->n = old_scale - val;
}

int big_norm(big_decimal *a, big_decimal *b) {
  int n1 = abs(a->n);
  int n2 = abs(b->n);
  int n3 = n1 > n2 ? n1 : n2;
  if (n1 > n2) {
    for (int i = 0; i < n1 - n2; i++) big_mul_by_ten(*b, b);
  } else {
    for (int i = 0; i < n2 - n1; i++) big_mul_by_ten(*a, a);
  }
  return n3;
}

void big_mul_by_ten(big_decimal a, big_decimal *b) {
  float x = 10 * a.bits[0];
  if (fabs(x) > INT_MAX) {
    x = (x -  D) * 10;
    b->bits[0] =  -1;
    x = a.bits[1] + x;
    if (fabs(x) > INT_MAX) {
      x = (x -  D) * 10;
      b->bits[1] =  -1;
      x = a.bits[2] + x;
      if (fabs(x) > INT_MAX) {
        x = (x -  D) * 10;
        b->bits[2] =  -1;
        x = a.bits[3] + x;
        if (fabs(x) > INT_MAX) {
          x = (x -  D) * 10;
          b->bits[3] =  -1;
          x = a.bits[4] + x;
          if (fabs(x) > INT_MAX) {
            x = (x -  D) * 10;
            b->bits[4] =  -1;
            x = a.bits[5] + x;
            if (fabs(x) > INT_MAX) {
              x = (x -  D) * 10;
              b->bits[4] =  -1;
              x = a.bits[5] + x;
              if (fabs(x) > INT_MAX) {
                x = (x -  D) * 10;
                b->bits[5] =  -1;
                x = a.bits[6] + x;
                if (fabs(x) > INT_MAX) {
                  x = (x -  D) * 10;
                  b->bits[6] =  -1;
                  x = a.bits[7] + x;
                  if (fabs(x) > INT_MAX) {
                    b->bits[7] =  -1;
                  } else {
                    b->bits[7] = (int)x;
                  }
                } else {
                  b->bits[6] = (int)x;
                }
              } else {
                b->bits[5] = (int)x;
              }
            } else {
              b->bits[5] = (int)x;
            }
          } else {
            b->bits[4] = (int)x;
          }
        } else {
          b->bits[3] = (int)x;
        }
      } else {
        b->bits[2] = (int)(x);
      }
    } else {
      b->bits[1] = (int)(x);
    }
  } else {
    b->bits[0] = (int)(x);
  }
}

void big_plus(big_decimal a, big_decimal b, big_decimal *result) {
  int tmp = 0;
  init_big(result);
  if (abs(a.n) != abs(b.n)) result->n = big_norm(&a, &b);
  for (int i = 0; i < 256; i++) {
    if (dec_get_big_bit(a, i) && get_big_bit(b, i) && tmp == 1) {
      tmp = 1;
      set_big_bit(result, i);
    } else if (dec_get_big_bit(a, i) && get_big_bit(b, i) && tmp == 0) {
      tmp = 1;
    } else if ((dec_get_big_bit(a, i) || get_big_bit(b, i)) && tmp == 1) {
      tmp = 1;
    } else if ((dec_get_big_bit(a, i) || get_big_bit(b, i)) && tmp == 0) {
      set_big_bit(result, i);
    } else if ((!dec_get_big_bit(a, i) && !dec_get_big_bit(b, i)) && tmp == 1) {
      set_big_bit(result, i);
      tmp = 0;
    }
  }
}

void big_minus(big_decimal a, big_decimal b, big_decimal *res) {
  init_big(res);
  res->n = big_norm(&a, &b);
  big_inverse(a, &a);
  big_plus(a, b, res);
  big_inverse(*res, res);
}

void big_inverse(big_decimal b, big_decimal *d) {
  big_decimal tmp = {0};
  tmp.bits[0] = ~(b.bits[0]);
  tmp.bits[1] = ~(b.bits[1]);
  tmp.bits[2] = ~(b.bits[2]);
  tmp.bits[3] = ~(b.bits[3]);
  tmp.bits[4] = ~(b.bits[4]);
  tmp.bits[5] = ~(b.bits[5]);
  tmp.bits[6] = ~(b.bits[6]);
  tmp.bits[7] = ~(b.bits[7]);
  big_decimal plus_one = {{1, 0, 0, 0, 0, 0, 0, 0}, 0};
  big_plus(plus_one, tmp, d);
}

void big_mult(big_decimal a, big_decimal b, big_decimal *result) {
  big_decimal tmp_a = a;
  init_big(result);
  if (abs(a.n) != abs(b.n)) result->n = big_norm(&a, &b);
  for (int i = 0; i < 256; i++) {
    if (dec_get_big_bit(b, i)) {
      big_decimal tmp = {0};
      big_plus(tmp_a, *result, &tmp);
      *result = tmp;
    }
    big_shift(&tmp_a, 1);
  }
}

void big_div(big_decimal a, big_decimal b, big_decimal *result) {
  if (cringe(a, b)) {
    big_decimal tmp_a = a;
    big_decimal tmp_b = b;
    big_decimal tmp_result = {0};
    int count_a = get_big_count(a);
    int count_b = get_big_count(b);
    int step_count = count_a - count_b;
    for (int i = 0; i < count_a - count_b; i++) {
      big_shift(&tmp_b, 1);
    }
    if (!cringe(tmp_a, tmp_b)) {
      big_shift(&tmp_b, -1);
      step_count--;
    }
    for (int hz = 0; hz != step_count + 1; hz++) {
      if (!cringe(tmp_a, tmp_b)) {
        big_shift(&tmp_b, -1);
        if (hz != 0) {
          big_shift(result, 1);
        }
        continue;
      }
      big_minus(tmp_a, tmp_b, &tmp_result);
      big_shift(result, 1);
      set_big_bit(result, 0);
      big_shift(&tmp_b, -1);
      tmp_a = tmp_result;
      init_big(&tmp_result);
    }
  }
}

int get_big_count(big_decimal a) {
  int count = 0;
  for (int i = 255; i > 0; i--) {
    if (dec_get_big_bit(a, i)) {
      count = i;
      break;
    }
  }
  return count;
}

int cringe(big_decimal a, big_decimal b) {
  int result = 1;
  for (int i = 255; i != -1; i--) {
    if (dec_get_big_bit(a, i) != get_big_bit(b, i)) {
      if (dec_get_big_bit(a, i) == 0) {
        result = 0;
      }
      break;
    }
  }
  return result;
}
