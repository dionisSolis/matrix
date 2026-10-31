#include <stdbool.h>

// Для вычислительных операций матриц
#define OK 0
#define WRONG_MATRIX 1
#define ERROR_CALCULATIONS 2

// Для сравнительных операций матриц
#define SUCCESS 1
#define FAILURE 0

#define DIFFERENCE_TO_EQ 0.000001

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} s21_matrix;

// main funcs
int s21_create_matrix(int rows, int columns, s21_matrix* result);
void s21_remove_matrix(s21_matrix* A);

int s21_eq_matrix(s21_matrix* A, s21_matrix* B);

int s21_sum_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result);
int s21_sub_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result);

int s21_mult_number(s21_matrix* A, double number, s21_matrix* result);
int s21_mult_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result);

int s21_transpose(s21_matrix* A, s21_matrix* result);

int s21_determinant(s21_matrix* A, double* result);
int s21_calc_complements(s21_matrix* A, s21_matrix* result);

int s21_inverse_matrix(s21_matrix* A, s21_matrix* result);

// help func
bool s21_is_correct_matrix(s21_matrix* A);
bool s21_is_eq_matrix_size(s21_matrix* A, s21_matrix* B);
void s21_negate_matrix(s21_matrix* src, s21_matrix* dst);
bool s21_is_square(s21_matrix* A);
int s21_get_minor_matrix(s21_matrix* A, int skip_row, int skip_column,
                         s21_matrix* minor);