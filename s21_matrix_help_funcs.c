#include <stddef.h>

#include "s21_matrix.h"

bool s21_is_correct_matrix(s21_matrix* A) {
  bool result = true;

  if (A == NULL || A->matrix == NULL) {
    result = false;
  }

  return result;
}

bool s21_is_eq_matrix_size(s21_matrix* A, s21_matrix* B) {
  bool result = true;

  if (A->rows != B->rows || A->columns != B->columns) {
    result = false;
  }

  return result;
}

void s21_negate_matrix(s21_matrix* src, s21_matrix* dst) {
  for (int row = 0; row < src->rows; row++) {
    for (int column = 0; column < src->columns; column++) {
      dst->matrix[row][column] = -1 * src->matrix[row][column];
    }
  }
}

bool s21_is_square(s21_matrix* A) {
  if (!s21_is_correct_matrix(A)) {
    return false;
  }

  bool result = true;

  if (A->rows != A->columns) {
    result = false;
  }

  return result;
}

int s21_get_minor_matrix(s21_matrix* A, int skip_row, int skip_column,
                         s21_matrix* minor) {
  if (!s21_is_correct_matrix(A) || minor == NULL) {
    return WRONG_MATRIX;
  }

  if (s21_is_square(A) == false) {
    return ERROR_CALCULATIONS;
  }

  if (s21_create_matrix(A->rows - 1, A->columns - 1, minor) != OK) {
    return WRONG_MATRIX;
  }

  for (int row = 0; row < A->rows; row++) {
    int minor_row = row;

    if (minor_row == skip_row) {
      continue;
    } else if (minor_row > skip_row) {
      minor_row -= 1;
    }

    for (int column = 0; column < A->columns; column++) {
      int minor_column = column;

      if (minor_column == skip_column) {
        continue;
      } else if (minor_column > skip_column) {
        minor_column -= 1;
      }

      minor->matrix[minor_row][minor_column] = A->matrix[row][column];
    }
  }

  return OK;
}
