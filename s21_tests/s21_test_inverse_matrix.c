#include <check.h>

#include "../s21_matrix.h"

START_TEST(test_inverse_2x2) {
  s21_matrix A, R, expected;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  A.matrix[0][0] = 4;
  A.matrix[0][1] = 7;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 6;

  expected.matrix[0][0] = 0.6;
  expected.matrix[0][1] = -0.7;
  expected.matrix[1][0] = -0.2;
  expected.matrix[1][1] = 0.4;

  int code = s21_inverse_matrix(&A, &R);
  ck_assert_int_eq(code, OK);
  ck_assert_int_eq(R.rows, expected.rows);
  ck_assert_int_eq(R.columns, expected.columns);

  for (int i = 0; i < R.rows; i++) {
    for (int j = 0; j < R.columns; j++) {
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_non_square) {
  s21_matrix A = {0}, R = {0};
  s21_create_matrix(2, 3, &A);

  int code = s21_inverse_matrix(&A, &R);
  ck_assert_int_eq(code, ERROR_CALCULATIONS);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_singular) {
  s21_matrix A, R;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;

  int code = s21_inverse_matrix(&A, &R);
  ck_assert_int_eq(code, ERROR_CALCULATIONS);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_3x3) {
  s21_matrix A, R, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double data[3][3] = {{1, 2, 3}, {0, 1, 4}, {5, 6, 0}};
  double inv[3][3] = {{-24, 18, 5}, {20, -15, -4}, {-5, 4, 1}};

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = data[i][j];
      expected.matrix[i][j] = inv[i][j];
    }

  int code = s21_inverse_matrix(&A, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_identity_3x3) {
  s21_matrix I, R;
  s21_create_matrix(3, 3, &I);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) I.matrix[i][j] = (i == j) ? 1.0 : 0.0;

  int code = s21_inverse_matrix(&I, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], I.matrix[i][j], 1e-7);

  s21_remove_matrix(&I);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_inverse_small_values) {
  s21_matrix A, R, expected;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 4;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 5;

  expected.matrix[0][0] = -2.5;
  expected.matrix[0][1] = 2.0;
  expected.matrix[1][0] = 1.5;
  expected.matrix[1][1] = -1.0;

  int code = s21_inverse_matrix(&A, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < R.rows; i++)
    for (int j = 0; j < R.columns; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_negative_values) {
  s21_matrix A, R;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = -2;
  A.matrix[0][1] = -3;
  A.matrix[1][0] = -1;
  A.matrix[1][1] = -4;

  int code = s21_inverse_matrix(&A, &R);
  ck_assert_int_eq(code, OK);

  ck_assert_double_eq_tol(R.matrix[0][0], -0.8, 1e-7);
  ck_assert_double_eq_tol(R.matrix[0][1], 0.6, 1e-7);
  ck_assert_double_eq_tol(R.matrix[1][0], 0.2, 1e-7);
  ck_assert_double_eq_tol(R.matrix[1][1], -0.4, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_inverse_singular_3x3) {
  s21_matrix A, R;
  s21_create_matrix(3, 3, &A);

  double data[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) A.matrix[i][j] = data[i][j];

  int code = s21_inverse_matrix(&A, &R);
  ck_assert_int_eq(code, ERROR_CALCULATIONS);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_random_3x3) {
  s21_matrix A, R;
  s21_create_matrix(3, 3, &A);

  double data[3][3] = {{2, -1, 0}, {1, 2, -1}, {3, 0, 1}};
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) A.matrix[i][j] = data[i][j];

  int code = s21_inverse_matrix(&A, &R);
  ck_assert_int_eq(code, OK);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

Suite* inverse_suite(void) {
  Suite* s = suite_create("s21_inverse_matrix");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_inverse_2x2);
  tcase_add_test(tc, test_inverse_non_square);
  tcase_add_test(tc, test_inverse_singular);
  tcase_add_test(tc, test_inverse_3x3);
  tcase_add_test(tc, test_inverse_identity_3x3);
  tcase_add_test(tc, test_inverse_non_square);
  tcase_add_test(tc, test_inverse_singular);
  tcase_add_test(tc, test_inverse_singular_3x3);
  tcase_add_test(tc, test_inverse_negative_values);
  tcase_add_test(tc, test_inverse_small_values);
  tcase_add_test(tc, test_inverse_random_3x3);

  suite_add_tcase(s, tc);
  return s;
}
