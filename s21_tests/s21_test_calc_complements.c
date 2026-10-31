#include <check.h>

#include "../s21_matrix.h"

START_TEST(test_calc_complements_2x2) {
  s21_matrix A, R, expected;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  expected.matrix[0][0] = 4;
  expected.matrix[0][1] = -3;
  expected.matrix[1][0] = -2;
  expected.matrix[1][1] = 1;

  int code = s21_calc_complements(&A, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < R.rows; i++)
    for (int j = 0; j < R.columns; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_calc_complements_3x3) {
  s21_matrix A, R, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double data[3][3] = {{1, 2, 3}, {0, 4, 2}, {5, 2, 1}};
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) A.matrix[i][j] = data[i][j];

  double cof[3][3] = {{0, 10, -20}, {4, -14, 8}, {-8, -2, 4}};
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) expected.matrix[i][j] = cof[i][j];

  int code = s21_calc_complements(&A, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < R.rows; i++)
    for (int j = 0; j < R.columns; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], expected.matrix[i][j], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_calc_complements_not_square) {
  s21_matrix A, R;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 3, &R);

  int code = s21_calc_complements(&A, &R);
  ck_assert_int_eq(code, ERROR_CALCULATIONS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_calc_complements_1x1) {
  s21_matrix A, R, expected;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &expected);

  A.matrix[0][0] = 7;
  expected.matrix[0][0] = 1;

  int code = s21_calc_complements(&A, &R);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(R.matrix[0][0], expected.matrix[0][0], 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_calc_complements_zero_matrix) {
  s21_matrix A, R;
  s21_create_matrix(3, 3, &A);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) A.matrix[i][j] = 0;

  int code = s21_calc_complements(&A, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      ck_assert_double_eq_tol(R.matrix[i][j], 0.0, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_calc_complements_negative_numbers) {
  s21_matrix A, R;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = -1;
  A.matrix[0][1] = -2;
  A.matrix[1][0] = -3;
  A.matrix[1][1] = -4;

  int code = s21_calc_complements(&A, &R);
  ck_assert_int_eq(code, OK);

  ck_assert_double_eq_tol(R.matrix[0][0], -4, 1e-7);
  ck_assert_double_eq_tol(R.matrix[0][1], 3, 1e-7);
  ck_assert_double_eq_tol(R.matrix[1][0], 2, 1e-7);
  ck_assert_double_eq_tol(R.matrix[1][1], -1, 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_calc_complements_null_matrix) {
  s21_matrix R = {0};
  int code = s21_calc_complements(NULL, &R);
  ck_assert_int_eq(code, WRONG_MATRIX);

  s21_matrix A = {0};
  code = s21_calc_complements(&A, &R);
  ck_assert_int_eq(code, WRONG_MATRIX);

  code = s21_calc_complements(&A, NULL);
  ck_assert_int_eq(code, WRONG_MATRIX);
}
END_TEST

START_TEST(test_calc_complements_identity_3x3) {
  s21_matrix I, R;
  s21_create_matrix(3, 3, &I);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) I.matrix[i][j] = (i == j) ? 1.0 : 0.0;

  int code = s21_calc_complements(&I, &R);
  ck_assert_int_eq(code, OK);

  ck_assert_double_eq_tol(R.matrix[0][0], 1, 1e-7);
  ck_assert_double_eq_tol(R.matrix[1][1], 1, 1e-7);
  ck_assert_double_eq_tol(R.matrix[2][2], 1, 1e-7);

  s21_remove_matrix(&I);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_calc_complements_4x4) {
  s21_matrix A, R;
  s21_create_matrix(4, 4, &A);

  double val = 1.0;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) A.matrix[i][j] = val++;

  int code = s21_calc_complements(&A, &R);
  ck_assert_int_eq(code, OK);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

START_TEST(test_calc_complements_large_values) {
  s21_matrix A, R;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 1e6;
  A.matrix[0][1] = 2e6;
  A.matrix[0][2] = 3e6;
  A.matrix[1][0] = 4e6;
  A.matrix[1][1] = 5e6;
  A.matrix[1][2] = 6e6;
  A.matrix[2][0] = 7e6;
  A.matrix[2][1] = 8e6;
  A.matrix[2][2] = 10e6;

  int code = s21_calc_complements(&A, &R);
  ck_assert_int_eq(code, OK);

  for (int i = 0; i < R.rows; i++)
    for (int j = 0; j < R.columns; j++)
      ck_assert_double_ge(fabsl(R.matrix[i][j]), 1e6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&R);
}
END_TEST

Suite* calc_complements_suite(void) {
  Suite* s = suite_create("s21_calc_complements");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_calc_complements_2x2);
  tcase_add_test(tc, test_calc_complements_3x3);
  tcase_add_test(tc, test_calc_complements_not_square);
  tcase_add_test(tc, test_calc_complements_1x1);
  tcase_add_test(tc, test_calc_complements_zero_matrix);
  tcase_add_test(tc, test_calc_complements_negative_numbers);
  tcase_add_test(tc, test_calc_complements_null_matrix);
  tcase_add_test(tc, test_calc_complements_identity_3x3);
  tcase_add_test(tc, test_calc_complements_4x4);
  tcase_add_test(tc, test_calc_complements_large_values);

  suite_add_tcase(s, tc);
  return s;
}
