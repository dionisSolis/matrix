#include <check.h>

#include "../s21_matrix.h"

START_TEST(test_sum_simple) {
  s21_matrix A, B, R, expected;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  s21_create_matrix(2, 2, &expected);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  B.matrix[0][0] = 5;
  B.matrix[0][1] = 6;
  B.matrix[1][0] = 7;
  B.matrix[1][1] = 8;

  expected.matrix[0][0] = 6;
  expected.matrix[0][1] = 8;
  expected.matrix[1][0] = 10;
  expected.matrix[1][1] = 12;

  int code = s21_sum_matrix(&A, &B, &R);

  ck_assert_int_eq(code, OK);
  for (int i = 0; i < R.rows; i++) {
    for (int j = 0; j < R.columns; j++) {
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_sum_zero_matrices) {
  s21_matrix A, B, R;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

  int code = s21_sum_matrix(&A, &B, &R);

  ck_assert_int_eq(code, OK);
  for (int i = 0; i < R.rows; i++) {
    for (int j = 0; j < R.columns; j++) {
      ck_assert_double_eq_tol(R.matrix[i][j], 0.0, 1e-7);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_sum_negative_numbers) {
  s21_matrix A, B, R;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = -1;
  A.matrix[0][1] = -2;
  A.matrix[1][0] = -3;
  A.matrix[1][1] = -4;

  B.matrix[0][0] = -5;
  B.matrix[0][1] = -6;
  B.matrix[1][0] = -7;
  B.matrix[1][1] = -8;

  int code = s21_sum_matrix(&A, &B, &R);

  ck_assert_int_eq(code, OK);
  ck_assert_double_eq(R.matrix[0][0], -6);
  ck_assert_double_eq(R.matrix[1][1], -12);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_sum_floats) {
  s21_matrix A, B, R;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 0.1;
  A.matrix[0][1] = 0.2;
  A.matrix[1][0] = 0.3;
  A.matrix[1][1] = 0.4;

  B.matrix[0][0] = 0.9;
  B.matrix[0][1] = 1.8;
  B.matrix[1][0] = 2.7;
  B.matrix[1][1] = 3.6;

  int code = s21_sum_matrix(&A, &B, &R);

  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(R.matrix[0][0], 1.0, 1e-7);
  ck_assert_double_eq_tol(R.matrix[1][1], 4.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_sum_null) {
  s21_matrix A = {0}, B = {0}, R = {0};
  int code = s21_sum_matrix(NULL, &B, &R);
  ck_assert_int_eq(code, WRONG_MATRIX);

  code = s21_sum_matrix(&A, NULL, &R);
  ck_assert_int_eq(code, WRONG_MATRIX);

  code = s21_sum_matrix(&A, &B, NULL);
  ck_assert_int_eq(code, WRONG_MATRIX);
}
END_TEST

START_TEST(test_sum_large_matrix) {
  int n = 50;
  s21_matrix A, B, R;
  s21_create_matrix(n, n, &A);
  s21_create_matrix(n, n, &B);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      A.matrix[i][j] = 1.0;
      B.matrix[i][j] = 2.0;
    }
  }

  int code = s21_sum_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      ck_assert_double_eq_tol(R.matrix[i][j], 3.0, 1e-7);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_sum_different_sizes) {
  s21_matrix A, B, R;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 2, &B);

  int code = s21_sum_matrix(&A, &B, &R);
  ck_assert_int_eq(code, ERROR_CALCULATIONS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_one_row) {
  s21_matrix A, B, R;
  s21_create_matrix(1, 4, &A);
  s21_create_matrix(1, 4, &B);

  for (int j = 0; j < 4; j++) {
    A.matrix[0][j] = j + 1;
    B.matrix[0][j] = j + 10;
  }

  int code = s21_sum_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);

  for (int j = 0; j < 4; j++) {
    ck_assert_double_eq(R.matrix[0][j], (j + 1) + (j + 10));
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_sum_one_column) {
  s21_matrix A, B, R;
  s21_create_matrix(4, 1, &A);
  s21_create_matrix(4, 1, &B);

  for (int i = 0; i < 4; i++) {
    A.matrix[i][0] = i + 1;
    B.matrix[i][0] = (i + 1) * 2;
  }

  int code = s21_sum_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < 4; i++) {
    ck_assert_double_eq(R.matrix[i][0], (i + 1) + (i + 1) * 2);
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_sum_empty_matrix) {
  s21_matrix A = {0}, B = {0}, R;

  int code = s21_sum_matrix(&A, &B, &R);
  ck_assert_int_eq(code, WRONG_MATRIX);
}
END_TEST

START_TEST(test_sum_extreme_values) {
  s21_matrix A, B, R;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1e308;
  A.matrix[0][1] = -1e308;
  A.matrix[1][0] = 1e308;
  A.matrix[1][1] = -1e308;

  B.matrix[0][0] = 1e308;
  B.matrix[0][1] = -1e308;
  B.matrix[1][0] = 1e308;
  B.matrix[1][1] = -1e308;

  int code = s21_sum_matrix(&A, &B, &R);
  ck_assert_int_eq(code, OK);

  ck_assert(!isnan(R.matrix[0][0]));
  ck_assert(!isnan(R.matrix[0][1]));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&R);
}
END_TEST

Suite* sum_matrix_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_sum_matrix");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_sum_simple);
  tcase_add_test(tc_core, test_sum_zero_matrices);
  tcase_add_test(tc_core, test_sum_negative_numbers);
  tcase_add_test(tc_core, test_sum_floats);
  tcase_add_test(tc_core, test_sum_null);
  tcase_add_test(tc_core, test_sum_large_matrix);
  tcase_add_test(tc_core, test_sum_different_sizes);
  tcase_add_test(tc_core, test_sum_one_row);
  tcase_add_test(tc_core, test_sum_one_column);
  tcase_add_test(tc_core, test_sum_empty_matrix);
  tcase_add_test(tc_core, test_sum_extreme_values);

  suite_add_tcase(s, tc_core);
  return s;
}