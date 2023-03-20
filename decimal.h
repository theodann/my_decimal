#ifndef DECIMAL_H_
#define DECIMAL_H_

#include <math.h>
#include <stdlib.h>

#define min 1e-28
#define INT_MAX 2147483647
#define LONG_MAX 4294967296
#define LONG_LONG_MAX 1.8446744e+19
#define D 2147483648
#define inf 1.0 / 0.0

typedef struct {
  int bits[4];
} decimal;

typedef struct {
  int bits[8];
  int n;
} big_decimal;

typedef enum errComparison { FALSE, TRUE } errComparison;

typedef enum errArithmetic {
  OK,
  TOO_LARGE,
  TOO_SMALL,
  DIV_BY_ZERO
} errArithmetic;

typedef enum errAnother { DONE, CONVERT_ERR } errAnother;

int dec_count_mant(float x);
int dec_get_digit(int index);
int dec_get_index(int index);
int dec_get_bit(decimal number, int index);
void dec_inverse_bit(decimal *number, int index);
void dec_change_bit(decimal *number, int index, int bit);
int dec_is_negative(decimal number);
void dec_copy(decimal number, decimal *result);
int dec_calc_scale(decimal number);
void dec_init_decimal(decimal *result);
void dec_set_scale(decimal *number, int scale);
int dec_get_n(decimal a);
void dec_div_by_ten(decimal value, decimal *result);
int dec_is_zero(decimal value);
int dec_simple_add(decimal value_1, decimal value_2,
                   decimal *result);
int dec_simple_sub(decimal val1, decimal val2, decimal *res);
int dec_check_result(decimal *result, int flag);
void dec_shift_left(decimal *value);
int dec_shift_right(decimal *value);
int dec_normalisation(decimal *value1, decimal *value2);
int dec_mul_by_ten(decimal value, decimal *result);
int dec_check_valid_decimal(decimal value);
int dec_dec(float a);
long int dec_seg(int a);
void dec_magic(decimal *val);

int dec_add(decimal value_1, decimal value_2, decimal *result);
int dec_big_add(decimal src1, decimal src2, decimal *res);
int dec_sub(decimal value_1, decimal value_2, decimal *result);
int dec_big_sub(decimal src1, decimal src2, decimal *res);
int dec_mul(decimal value_1, decimal value_2, decimal *result);
int dec_div(decimal value_1, decimal value_2, decimal *result);
int dec_mod(decimal value_1, decimal value_2, decimal *result);
int dec_is_less(decimal, decimal);
int dec_is_less_or_equal(decimal, decimal);
int dec_is_greater(decimal, decimal);
int dec_is_greater_or_equal(decimal, decimal);
int dec_is_not_equal(decimal, decimal);
int dec_is_equal(decimal, decimal);
int dec_from_int_to_decimal(int src, decimal *dst);
int dec_from_float_to_decimal(float src, decimal *dst);
int dec_from_decimal_to_int(decimal src, int *dst);
int dec_from_decimal_to_float(decimal src, float *dst);
int dec_floor(decimal value, decimal *result);
int dec_round(decimal value, decimal *result);
int dec_truncate(decimal value, decimal *result);
int dec_negate(decimal value, decimal *result);

int get_big_bit(big_decimal d, int i);
void set_big_bit(big_decimal *d, int i);
void clean_big_bit(big_decimal *d, int i);
void big_shift(big_decimal *a, int flag);
void to_big(decimal a, big_decimal *b);
void init_big(big_decimal *a);
int from_big(big_decimal a, decimal *b);
void dec_scale(big_decimal *bd, int val, big_decimal *result);
int get_big_count(big_decimal a);
void big_mul_by_ten(big_decimal a, big_decimal *b);
int big_norm(big_decimal *a, big_decimal *b);
void big_plus(big_decimal a, big_decimal b, big_decimal *result);
void big_minus(big_decimal a, big_decimal b, big_decimal *res);
void big_mult(big_decimal a, big_decimal b, big_decimal *result);
void big_div(big_decimal a, big_decimal b, big_decimal *result);
void big_inverse(big_decimal b, big_decimal *d);
int cringe(big_decimal a, big_decimal b);

#endif  // DECIMAL_H_
