#include <check.h>

#include "../s21_matrix.h"

START_TEST(test_mult_number_simple) {
  s21_matrix A, R, expected;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  // A = [[1, 2], [3, 4]]
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  // expected = [[2, 4], [6, 8]]
  expected.matrix[0][0] = 2;
  expected.matrix[0][1] = 4;
  expected.matrix[1][0] = 6;
  expected.matrix[1][1] = 8;

  int code = s21_mult_number(&A, 2.0, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < R.rows; i++)
    for (int j = 0; j < R.columns; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_number_zero) {
  s21_matrix A, R;
  s21_create_matrix(2, 3, &A);

  A.matrix[0][0] = 5;
  A.matrix[0][1] = -3;
  A.matrix[0][2] = 2;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 7;
  A.matrix[1][2] = -1;

  int code = s21_mult_number(&A, 0.0, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 3; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], 0.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_number_negative) {
  s21_matrix A, R;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = -2;
  A.matrix[1][0] = -3;
  A.matrix[1][1] = 4;

  int code = s21_mult_number(&A, -2.0, &R);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq(R.matrix[0][0], -2);
  ck_assert_double_eq(R.matrix[0][1], 4);
  ck_assert_double_eq(R.matrix[1][0], 6);
  ck_assert_double_eq(R.matrix[1][1], -8);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_number_fraction) {
  s21_matrix A, R;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 4;
  A.matrix[1][0] = 6;
  A.matrix[1][1] = 8;

  int code = s21_mult_number(&A, 0.5, &R);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(R.matrix[0][0], 1.0, 1e-7);
  ck_assert_double_eq_tol(R.matrix[1][1], 4.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_number_large_matrix) {
  int n = 30;
  s21_matrix A, R;
  s21_create_matrix(n, n, &A);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) A.matrix[i][j] = 1.0;

  int code = s21_mult_number(&A, 10.0, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], 10.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_number_null_matrix) {
  s21_matrix R = {0};
  int code = s21_mult_number(NULL, 2.0, &R);
  ck_assert_int_eq(code, WRONG_MATRIX);

  s21_matrix A = {0};
  code = s21_mult_number(&A, 2.0, &R);
  ck_assert_int_eq(code, WRONG_MATRIX);

  code = s21_mult_number(&A, 2.0, NULL);
  ck_assert_int_eq(code, WRONG_MATRIX);
}
END_TEST

START_TEST(test_mult_number_one_element) {
  s21_matrix A, R;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 42.0;

  int code = s21_mult_number(&A, 2.5, &R);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(R.matrix[0][0], 105.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_number_row_matrix) {
  s21_matrix A, R;
  s21_create_matrix(1, 5, &A);
  for (int j = 0; j < 5; j++) A.matrix[0][j] = j + 1;

  int code = s21_mult_number(&A, 3.0, &R);
  ck_assert_int_eq(code, OK);

  for (int j = 0; j < 5; j++) ck_assert_double_eq(R.matrix[0][j], (j + 1) * 3);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_number_column_matrix) {
  s21_matrix A, R;
  s21_create_matrix(5, 1, &A);
  for (int i = 0; i < 5; i++) A.matrix[i][0] = i + 1;

  int code = s21_mult_number(&A, -2.0, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < 5; i++) ck_assert_double_eq(R.matrix[i][0], -(i + 1) * 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_mult_number_extreme_values) {
  s21_matrix A, R;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1e308;
  A.matrix[0][1] = -1e308;
  A.matrix[1][0] = 1e308;
  A.matrix[1][1] = -1e308;

  int code = s21_mult_number(&A, 2.0, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++) ck_assert(!isnan(R.matrix[i][j]));

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

Suite* mult_number_suite(void) {
  Suite* s = suite_create("s21_mult_number");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_mult_number_simple);
  tcase_add_test(tc, test_mult_number_zero);
  tcase_add_test(tc, test_mult_number_negative);
  tcase_add_test(tc, test_mult_number_fraction);
  tcase_add_test(tc, test_mult_number_large_matrix);
  tcase_add_test(tc, test_mult_number_null_matrix);
  tcase_add_test(tc, test_mult_number_one_element);
  tcase_add_test(tc, test_mult_number_row_matrix);
  tcase_add_test(tc, test_mult_number_column_matrix);
  tcase_add_test(tc, test_mult_number_extreme_values);

  suite_add_tcase(s, tc);
  return s;
}
