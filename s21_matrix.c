#include "s21_matrix.h"

#include <math.h>
#include <stdlib.h>

int s21_create_matrix(int rows, int columns, s21_matrix* result) {
  if (rows < 1 || columns < 1 || result == NULL) {
    return WRONG_MATRIX;
  }

  int exit_status = OK;

  result->rows = rows;
  result->columns = columns;

  result->matrix = calloc(rows, sizeof(*result->matrix));

  if (result->matrix == NULL) {
    exit_status = WRONG_MATRIX;
  }

  for (int row = 0; row < rows && !exit_status; row++) {
    result->matrix[row] = calloc(columns, sizeof(**result->matrix));

    if (result->matrix[row] == NULL) {
      exit_status = WRONG_MATRIX;

      for (int i = 0; i < row; i++) {
        free(result->matrix[i]);
      }

      free(result->matrix);
      result->matrix = NULL;
    }
  }

  if (exit_status != OK) {
    result->rows = 0;
    result->columns = 0;
    result->matrix = NULL;
  }

  return exit_status;
}

void s21_remove_matrix(s21_matrix* A) {
  if (A == NULL || A->matrix == NULL) {
    return;
  }

  for (int row = 0; row < A->rows; row++) {
    free(A->matrix[row]);
  }

  free(A->matrix);
  A->matrix = NULL;
  A->rows = 0;
  A->columns = 0;
}

int s21_eq_matrix(s21_matrix* A, s21_matrix* B) {
  if (s21_is_correct_matrix(A) == false || s21_is_correct_matrix(B) == false) {
    return FAILURE;
  }

  if (s21_is_eq_matrix_size(A, B) == false) {
    return FAILURE;
  }

  int exit_status = SUCCESS;

  for (int row = 0; row < A->rows && exit_status; row++) {
    for (int column = 0; column < A->columns && exit_status; column++) {
      if (fabs(A->matrix[row][column] - B->matrix[row][column]) >=
          DIFFERENCE_TO_EQ) {
        exit_status = FAILURE;
      }
    }
  }

  return exit_status;
}

int s21_sum_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result) {
  if (!s21_is_correct_matrix(A) || !s21_is_correct_matrix(B)) {
    return WRONG_MATRIX;
  }

  if (!s21_is_eq_matrix_size(A, B)) {
    return ERROR_CALCULATIONS;
  }

  if (s21_create_matrix(A->rows, A->columns, result) != 0) {
    return WRONG_MATRIX;
  }

  for (int row = 0; row < result->rows; row++) {
    for (int column = 0; column < result->columns; column++) {
      result->matrix[row][column] =
          A->matrix[row][column] + B->matrix[row][column];
    }
  }

  return OK;
}

int s21_sub_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result) {
  if (!s21_is_correct_matrix(B)) {
    return WRONG_MATRIX;
  }

  s21_matrix inverse_B = {NULL, 0, 0};
  int exit_status = s21_create_matrix(B->rows, B->columns, &inverse_B);

  if (exit_status == OK) {
    s21_negate_matrix(B, &inverse_B);
    exit_status = s21_sum_matrix(A, &inverse_B, result);
    s21_remove_matrix(&inverse_B);
  }

  return exit_status;
}

int s21_mult_number(s21_matrix* A, double number, s21_matrix* result) {
  if (!s21_is_correct_matrix(A) || result == NULL) {
    return WRONG_MATRIX;
  }

  if (s21_create_matrix(A->rows, A->columns, result)) {
    return WRONG_MATRIX;
  }

  for (int row = 0; row < A->rows; row++) {
    for (int column = 0; column < A->columns; column++) {
      result->matrix[row][column] = A->matrix[row][column] * number;
    }
  }

  return OK;
}

int s21_mult_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result) {
  if (!s21_is_correct_matrix(A) || !s21_is_correct_matrix(B) ||
      result == NULL) {
    return WRONG_MATRIX;
  }

  if (A->columns != B->rows) {
    return ERROR_CALCULATIONS;
  }

  if (s21_create_matrix(A->rows, B->columns, result) != OK) {
    return WRONG_MATRIX;
  }

  for (int row = 0; row < result->rows; row++) {
    for (int column = 0; column < result->columns; column++) {
      result->matrix[row][column] = 0;

      for (int k = 0; k < A->columns; k++) {
        result->matrix[row][column] += A->matrix[row][k] * B->matrix[k][column];
      }
    }
  }

  return OK;
}

int s21_transpose(s21_matrix* A, s21_matrix* result) {
  if (!s21_is_correct_matrix(A) || result == NULL) {
    return WRONG_MATRIX;
  }

  if (s21_create_matrix(A->columns, A->rows, result) != OK) {
    return WRONG_MATRIX;
  }

  for (int row = 0; row < result->rows; row++) {
    for (int column = 0; column < result->columns; column++) {
      result->matrix[row][column] = A->matrix[column][row];
    }
  }

  return OK;
}

int s21_determinant(s21_matrix* A, double* result) {
  if (s21_is_correct_matrix(A) == false || result == NULL) {
    return WRONG_MATRIX;
  }

  if (s21_is_square(A) == false) {
    return ERROR_CALCULATIONS;
  }

  int exit_status = OK;

  if (A->rows == 1) {
    *result = A->matrix[0][0];
  } else if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    double sign = -1.0;
    *result = 0.0;

    for (int column = 0; column < A->columns && exit_status == OK; column++) {
      sign *= -1.0;
      s21_matrix minor_matrix = {NULL, 0, 0};
      exit_status = s21_get_minor_matrix(A, 0, column, &minor_matrix);

      if (exit_status != OK) {
        continue;
      }

      double minor;
      exit_status = s21_determinant(&minor_matrix, &minor);

      if (exit_status != OK) {
        s21_remove_matrix(&minor_matrix);
        continue;
      }

      *result += sign * A->matrix[0][column] * minor;
      s21_remove_matrix(&minor_matrix);
    }
  }

  return exit_status;
}

int s21_calc_complements(s21_matrix* A, s21_matrix* result) {
  if (s21_is_correct_matrix(A) == false || result == NULL) {
    return WRONG_MATRIX;
  }

  if (s21_is_square(A) == false) {
    return ERROR_CALCULATIONS;
  }

  if (s21_create_matrix(A->rows, A->columns, result) != OK) {
    return WRONG_MATRIX;
  }

  if (A->rows == 1) {
    result->matrix[0][0] = 1.0;
    return OK;
  }

  int exit_status = OK;

  for (int row = 0; row < A->rows && exit_status == OK; row++) {
    for (int column = 0; column < A->columns && exit_status == OK; column++) {
      s21_matrix minor_matrix = {NULL, 0, 0};
      exit_status = s21_get_minor_matrix(A, row, column, &minor_matrix);

      if (exit_status != OK) {
        continue;
      }

      double minor;
      exit_status = s21_determinant(&minor_matrix, &minor);

      if (exit_status != OK) {
        s21_remove_matrix(&minor_matrix);
        continue;
      }

      double sign = -1.0;

      if ((row + column + 2) % 2 == 0) {
        sign *= -1.0;
      }

      result->matrix[row][column] = minor * sign;
      s21_remove_matrix(&minor_matrix);
    }
  }

  return exit_status;
}

int s21_inverse_matrix(s21_matrix* A, s21_matrix* result) {
  if (!s21_is_correct_matrix(A) || result == NULL) {
    return WRONG_MATRIX;
  }
  if (!s21_is_square(A)) {
    return ERROR_CALCULATIONS;
  }

  int exit_status = OK;
  double det_A = 0.0;
  exit_status = s21_determinant(A, &det_A);

  if (exit_status == OK && fabs(det_A) < DIFFERENCE_TO_EQ) {
    exit_status = ERROR_CALCULATIONS;
  }

  s21_matrix matrix_alg_ext = {NULL, 0, 0};
  s21_matrix matrix_alg_ext_transpose = {NULL, 0, 0};

  if (exit_status == OK) {
    exit_status = s21_calc_complements(A, &matrix_alg_ext);
  }
  if (exit_status == OK) {
    exit_status = s21_transpose(&matrix_alg_ext, &matrix_alg_ext_transpose);
  }
  if (exit_status == OK) {
    exit_status =
        s21_mult_number(&matrix_alg_ext_transpose, 1.0 / det_A, result);
  }

  if (exit_status != ERROR_CALCULATIONS) {
    s21_remove_matrix(&matrix_alg_ext_transpose);
    s21_remove_matrix(&matrix_alg_ext);
  }

  return exit_status;
}