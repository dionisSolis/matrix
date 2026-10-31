#include <check.h>

#include "../s21_matrix.h"

START_TEST(test_transpose_square) {
  s21_matrix A, R, expected;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  // A = [[1, 2],
  //      [3, 4]]
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  // expected = [[1, 3],
  //             [2, 4]]
  expected.matrix[0][0] = 1;
  expected.matrix[0][1] = 3;
  expected.matrix[1][0] = 2;
  expected.matrix[1][1] = 4;

  int code = s21_transpose(&A, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < R.rows; i++)
    for (int j = 0; j < R.columns; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_transpose_rectangular) {
  s21_matrix A, R, expected;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &expected);

  // A = [[1, 2, 3],
  //      [4, 5, 6]]
  int val = 1;
  for (int i = 0; i < A.rows; i++)
    for (int j = 0; j < A.columns; j++) A.matrix[i][j] = val++;

  // expected = [[1, 4],
  //             [2, 5],
  //             [3, 6]]
  expected.matrix[0][0] = 1;
  expected.matrix[0][1] = 4;
  expected.matrix[1][0] = 2;
  expected.matrix[1][1] = 5;
  expected.matrix[2][0] = 3;
  expected.matrix[2][1] = 6;

  int code = s21_transpose(&A, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < R.rows; i++)
    for (int j = 0; j < R.columns; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_transpose_single_element) {
  s21_matrix A, R;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 42;

  int code = s21_transpose(&A, &R);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq(R.matrix[0][0], 42);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_transpose_large) {
  int n = 30;
  s21_matrix A, R;
  s21_create_matrix(n, n, &A);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) A.matrix[i][j] = i * n + j;

  int code = s21_transpose(&A, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], A.matrix[j][i], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

Suite* transpose_suite(void) {
  Suite* s = suite_create("s21_transpose");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_transpose_square);
  tcase_add_test(tc, test_transpose_rectangular);
  tcase_add_test(tc, test_transpose_single_element);
  tcase_add_test(tc, test_transpose_large);

  suite_add_tcase(s, tc);
  return s;
}
