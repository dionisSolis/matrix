#include <check.h>

#include "../s21_matrix.h"

START_TEST(test_mult_matrix_simple) {
  s21_matrix A, B, R, expected;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_create_matrix(2, 2, &expected);

  int val = 1;
  for (int i = 0; i < A.rows; i++)
    for (int j = 0; j < A.columns; j++) A.matrix[i][j] = val++;

  B.matrix[0][0] = 7;
  B.matrix[0][1] = 8;
  B.matrix[1][0] = 9;
  B.matrix[1][1] = 10;
  B.matrix[2][0] = 11;
  B.matrix[2][1] = 12;

  expected.matrix[0][0] = 58;
  expected.matrix[0][1] = 64;
  expected.matrix[1][0] = 139;
  expected.matrix[1][1] = 154;

  int code = s21_mult_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < R.rows; i++)
    for (int j = 0; j < R.columns; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_identity) {
  s21_matrix A, I, R;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &I);

  A.matrix[0][0] = 5;
  A.matrix[0][1] = 7;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = -3;

  I.matrix[0][0] = 1;
  I.matrix[0][1] = 0;
  I.matrix[1][0] = 0;
  I.matrix[1][1] = 1;

  int code = s21_mult_matrix(&A, &I, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], A.matrix[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&I);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_matrix_zero) {
  s21_matrix A, Z, R;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 3, &Z);

  for (int i = 0; i < A.rows; i++)
    for (int j = 0; j < A.columns; j++) A.matrix[i][j] = i + j + 1;

  for (int i = 0; i < Z.rows; i++)
    for (int j = 0; j < Z.columns; j++) Z.matrix[i][j] = 0;

  int code = s21_mult_matrix(&A, &Z, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < R.rows; i++)
    for (int j = 0; j < R.columns; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], 0.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&Z);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_matrix_incompatible) {
  s21_matrix A, B, R;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(4, 2, &B);

  int code = s21_mult_matrix(&A, &B, &R);
  ck_assert_int_eq(code, ERROR_CALCULATIONS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_large) {
  int n = 20;
  s21_matrix A, B, R;
  s21_create_matrix(n, n, &A);
  s21_create_matrix(n, n, &B);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) A.matrix[i][j] = (i == j) ? 1.0 : 0.0;

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) B.matrix[i][j] = 2.0;

  int code = s21_mult_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], 2.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_matrix_null_matrix) {
  s21_matrix R = {0};
  int code = s21_mult_matrix(NULL, NULL, &R);
  ck_assert_int_eq(code, WRONG_MATRIX);

  s21_matrix A = {0}, B = {0};
  code = s21_mult_matrix(&A, &B, &R);
  ck_assert_int_eq(code, WRONG_MATRIX);

  code = s21_mult_matrix(&A, &B, NULL);
  ck_assert_int_eq(code, WRONG_MATRIX);
}
END_TEST

START_TEST(test_mult_matrix_one_element) {
  s21_matrix A, B, R;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  A.matrix[0][0] = 5.0;
  B.matrix[0][0] = -2.0;

  int code = s21_mult_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(R.matrix[0][0], -10.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_matrix_row_vector) {
  s21_matrix A, B, R;
  s21_create_matrix(1, 3, &A);
  s21_create_matrix(3, 1, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  B.matrix[0][0] = 4;
  B.matrix[1][0] = 5;
  B.matrix[2][0] = 6;

  int code = s21_mult_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(R.matrix[0][0], 32.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_matrix_column_vector) {
  s21_matrix A, B, R;
  s21_create_matrix(3, 1, &A);
  s21_create_matrix(1, 3, &B);

  A.matrix[0][0] = 1;
  A.matrix[1][0] = 2;
  A.matrix[2][0] = 3;
  B.matrix[0][0] = 4;
  B.matrix[0][1] = 5;
  B.matrix[0][2] = 6;

  int code = s21_mult_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);

  double expected[3][3] = {{4, 5, 6}, {8, 10, 12}, {12, 15, 18}};
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], expected[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_matrix_extreme_values) {
  s21_matrix A, B, R;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1e308;
  A.matrix[0][1] = 1e308;
  A.matrix[1][0] = -1e308;
  A.matrix[1][1] = 1e308;
  B.matrix[0][0] = 2.0;
  B.matrix[0][1] = -2.0;
  B.matrix[1][0] = 0.5;
  B.matrix[1][1] = -0.5;

  int code = s21_mult_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++) ck_assert(!isnan(R.matrix[i][j]));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

Suite* mult_matrix_suite(void) {
  Suite* s = suite_create("s21_mult_matrix");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_mult_matrix_simple);
  tcase_add_test(tc, test_mult_matrix_identity);
  tcase_add_test(tc, test_mult_matrix_zero);
  tcase_add_test(tc, test_mult_matrix_incompatible);
  tcase_add_test(tc, test_mult_matrix_large);
  tcase_add_test(tc, test_mult_matrix_null_matrix);
  tcase_add_test(tc, test_mult_matrix_one_element);
  tcase_add_test(tc, test_mult_matrix_row_vector);
  tcase_add_test(tc, test_mult_matrix_column_vector);
  tcase_add_test(tc, test_mult_matrix_extreme_values);

  suite_add_tcase(s, tc);
  return s;
}
