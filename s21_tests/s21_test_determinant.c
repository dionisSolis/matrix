#include <check.h>

#include "../s21_matrix.h"

START_TEST(test_det_1x1) {
  s21_matrix A;
  double result = 0;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 42;

  int code = s21_determinant(&A, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(result, 42, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_det_2x2) {
  s21_matrix A;
  double result = 0;
  s21_create_matrix(2, 2, &A);

  // [[1, 2],
  //  [3, 4]]
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  int code = s21_determinant(&A, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(result, -2, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_det_3x3) {
  s21_matrix A;
  double result = 0;
  s21_create_matrix(3, 3, &A);

  // [[1, 2, 3],
  //  [0, 4, 2],
  //  [5, 2, 1]]
  double data[3][3] = {{1, 2, 3}, {0, 4, 2}, {5, 2, 1}};
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) A.matrix[i][j] = data[i][j];

  // Дет = 1*(4*1 - 2*2) - 2*(0*1 - 2*5) + 3*(0*2 - 4*5)
  //     = 1*(4 - 4) - 2*(0 - 10) + 3*(0 - 20)
  //     = 0 - 2*(-10) + 3*(-20) = 20 - 60 = -40
  int code = s21_determinant(&A, &result);
  ck_assert_int_eq(code, OK);
  ck_assert_double_eq_tol(result, -40, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_det_not_square) {
  s21_matrix A;
  double result = 0;
  s21_create_matrix(2, 3, &A);

  int code = s21_determinant(&A, &result);
  ck_assert_int_eq(code, ERROR_CALCULATIONS);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_det_null) {
  s21_matrix A;
  double result = 0;
  s21_create_matrix(2, 2, &A);

  ck_assert_int_eq(s21_determinant(NULL, &result), WRONG_MATRIX);
  ck_assert_int_eq(s21_determinant(&A, NULL), WRONG_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

Suite* determinant_suite(void) {
  Suite* s = suite_create("s21_determinant");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_det_1x1);
  tcase_add_test(tc, test_det_2x2);
  tcase_add_test(tc, test_det_3x3);
  tcase_add_test(tc, test_det_not_square);
  tcase_add_test(tc, test_det_null);

  suite_add_tcase(s, tc);
  return s;
}
