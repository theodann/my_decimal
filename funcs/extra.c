#include "../decimal.h"

int dec_count_mant(float x) {
  int ret = 0;
  x = fmod(x, 1) * 10;
  while (x != 0.0) {
    x = fmod(x, 1) * 10;
    ret++;
    if (ret > 28) {
      ret = 28;
      break;
    }
  }
  return ret;
}

int dec_get_digit(int index) { return index >> 5; }

int dec_get_index(int index) { return index % 32; }

int dec_get_bit(decimal number, int index) {
  int i = dec_get_digit(index);
  int ind = dec_get_index(index);
  return (number.bits[i] >> ind) & 1;
}

void dec_inverse_bit(decimal *number, int index) {
  int i = dec_get_digit(index);
  int ind = dec_get_index(index);
  if (i >= 0 && i <= 3) number->bits[i] ^= 1 << ind;
}

void dec_change_bit(decimal *number, int index, int bit) {
  int i = dec_get_digit(index);
  int ind = dec_get_index(index);
  int mask = 1 << ind;
  if (number) {
    number->bits[i] &= ~mask;
    if (bit) {
      number->bits[i] |= mask;
    }
  }
}

int dec_is_negative(decimal number) { return dec_get_bit(number, 127); }

void dec_copy(decimal number, decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = number.bits[i];
  }
}

int dec_calc_scale(decimal number) {
  int scale = 1, result = 0;
  for (int i = 112; i < 117; i++) {
    if (dec_get_bit(number, i)) {
      result += scale;
    }
    scale <<= 1;
  }
  return result;
}

void dec_init_decimal(decimal *result) {
  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;
}

void dec_set_scale(decimal *value, int scale) {
  int scale_index = 112;
  if (scale > 28) scale = 28;
  if (dec_is_negative(*value))
    value->bits[3] = 0b10000000000000000000000000000000;
  else
    value->bits[3] = 0;
  if (scale == 0) {
    int p = 0;
    if (dec_is_negative(*value)) p++;
    value->bits[3] = 0;
    if (p == 1) dec_negate(*value, value);
  }
  while (scale != 0) {
    if ((scale & 0x1) == 0x1) dec_change_bit(value, scale_index, 1);
    scale >>= 1;
    scale_index++;
  }
}

int dec_get_n(decimal a) { return dec_calc_scale(a); }

void dec_div_by_ten(decimal value, decimal *result) {
  result->bits[3] = value.bits[3];
  result->bits[2] = (int)(value.bits[2] / 10);
  int x = 10 * (value.bits[2] - result->bits[2]);
  result->bits[1] = (int)((value.bits[1] + x * INT_MAX) / 10);
  x = 10 * (value.bits[1] - result->bits[1]);
  result->bits[0] = (int)((value.bits[0] + x * INT_MAX) / 10);
  x = (int)fmod(value.bits[0], 10);
  if (x != 0) {
    result->bits[0] = (10 * result->bits[0]) + x;
  }
}

int dec_is_zero(decimal value) {
  int code = TRUE;
  for (int row = 0; row < 3; row++) {
    if (value.bits[row] != 0) {
      code = FALSE;
      break;
    }
  }
  return code;
}

void dec_shift_left(decimal *value) {
  for (int i = 0; i < 96; i++) {
    dec_change_bit(value, i, dec_get_bit(*value, i + 1));
  }
}

int dec_shift_right(decimal *value) {
  int code = CONVERT_ERR;
  if (!dec_get_bit(*value, 95)) {
    for (int i = 95; i > 0; i--) {
      dec_change_bit(value, i, dec_get_bit(*value, i - 1));
    }
    code = DONE;
    dec_change_bit(value, 0, 0);
  }
  return code;
}

int dec_check_valid_decimal(decimal value) {
  int code = 0;
  if ((value.bits[3] & 0b01111111000000001111111111111111) == 0) code = 1;
  return code;
}

int dec_normalisation(decimal *value1, decimal *value2) {
  int n1 = dec_calc_scale(*value1);
  int n2 = dec_calc_scale(*value2);
  int n3 = n1 > n2 ? n1 : n2;
  int p = 0;
  if (n1 > n2) {
    for (int i = 0; i < n1 - n2; i++) p += dec_mul_by_ten(*value2, value2);
  } else {
    for (int i = 0; i < n2 - n1; i++) p += dec_mul_by_ten(*value1, value1);
  }
  if (p != 0) {
    n3 = -1;
  } else {
    dec_set_scale(value1, n3);
    dec_set_scale(value2, n3);
  }
  return n3;  // возвращаем степень десятки, на которую необходимо разделить
              // результат бинарной операции, если n3 отрицательное, то
              // произошло переполнение децимала
}

int dec_mul_by_ten(decimal value, decimal *result) {
  int ret = 0;
  int scale = dec_get_n(value);
  float x = dec_seg(value.bits[0]) + LONG_MAX * dec_seg(value.bits[1]) +
            LONG_LONG_MAX * dec_seg(value.bits[2]);
  x = x * 10;
  scale = scale - 1;
  if (scale < 0) scale = 0;
  if (fabs(x / pow(10, scale)) > 79228162514264337593543950335.0) {
    ret++;
  } else {
    if (x >= LONG_LONG_MAX) {
      result->bits[0] = dec_dec(x - LONG_LONG_MAX);
      result->bits[2] = dec_dec(fmod(x, LONG_LONG_MAX));
      result->bits[1] = dec_dec(
          fmod((x - dec_seg(dec_dec(fmod(x, LONG_LONG_MAX)))), LONG_MAX));
    } else if (x >= LONG_MAX) {
      result->bits[0] = dec_dec(x - LONG_MAX);
      result->bits[1] = dec_dec(fmod(x, LONG_MAX));
    } else {
      result->bits[0] = dec_dec(x);
    }
    result->bits[3] = 0;
    dec_set_scale(result, scale);
    if (dec_is_negative(value)) dec_negate(*result, result);
  }
  return ret;
}

int dec_dec(float a) {
  if (a == D) a = -D;
  if (a > INT_MAX) a = a - D;
  return (int)a;
}

long int dec_seg(int a) {
  long int ret = a;
  if (a < 0) {
    ret = 2 * D - abs(a);
  }
  return ret;
}

void dec_magic(decimal *val) {
  int n = dec_get_n(*val);
  if (n != 0 && val->bits[0] % 10 == 0) {
    while (n) {
      if ((val->bits[0] % 10 == 0 && n <= 10) ||
          (val->bits[1] % 10 == 0 && n >= 10 && n < 20) ||
          (val->bits[1] % 10 == 0 && n >= 20)) {
        dec_div_by_ten(*val, val);
        n--;
      } else {
        break;
      }
    }
  }
  dec_set_scale(val, n);
}
