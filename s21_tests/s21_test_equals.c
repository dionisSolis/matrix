#include <check.h>

#include "../s21_matrix.h"

START_TEST(test_eq_identical_int) {
  s21_matrix A, B;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

  for (int i = 0; i < A.rows; i++) {
    for (int j = 0; j < A.columns; j++) {
      A.matrix[i][j] = B.matrix[i][j] = i * A.columns + j;
    }
  }

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_identical_float) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = B.matrix[0][0] = 0.123456789;
  A.matrix[0][1] = B.matrix[0][1] = -5.6789;
  A.matrix[1][0] = B.matrix[1][0] = 123.456;
  A.matrix[1][1] = B.matrix[1][1] = 0.000001;

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_different_sizes) {
  s21_matrix A, B;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);

  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_one_element_different) {
  s21_matrix A, B;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

  // Заполняем одинаково
  for (int i = 0; i < A.rows; i++) {
    for (int j = 0; j < A.columns; j++) {
      A.matrix[i][j] = B.matrix[i][j] = i * A.columns + j;
    }
  }

  // Меняем один элемент
  B.matrix[1][1] += 0.000001;

  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_within_epsilon) {
  s21_matrix A, B;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = 0.0000001;   // 1e-7
  B.matrix[0][0] = 0.00000009;  // 9e-8 (разница 1e-8 < EPSILON 1e-7)

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_beyond_epsilon) {
  s21_matrix A, B;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = 0.123457;
  B.matrix[0][0] = 0.123459;

  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_zero_matrices) {
  s21_matrix A, B;
  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &B);

  // Заполняем нулями
  for (int i = 0; i < A.rows; i++) {
    for (int j = 0; j < A.columns; j++) {
      A.matrix[i][j] = B.matrix[i][j] = 0.0;
    }
  }

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_large_numbers) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = B.matrix[0][0] = 1e15;
  A.matrix[0][1] = B.matrix[0][1] = -2e10;
  A.matrix[1][0] = B.matrix[1][0] = 3.1415926535;
  A.matrix[1][1] = B.matrix[1][1] = -999999.999999;

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_null_matrix) {
  s21_matrix A;
  s21_create_matrix(2, 2, &A);

  ck_assert_int_eq(s21_eq_matrix(&A, NULL), FAILURE);
  ck_assert_int_eq(s21_eq_matrix(NULL, &A), FAILURE);
  ck_assert_int_eq(s21_eq_matrix(NULL, NULL), FAILURE);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_eq_invalid_dimensions) {
  s21_matrix A = {NULL, 0, 0};
  s21_matrix B = {NULL, -1, 5};

  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
}
END_TEST

START_TEST(test_eq_large_matrices) {
  s21_matrix A, B;
  int size = 100;  // Большой размер для проверки производительности
  s21_create_matrix(size, size, &A);
  s21_create_matrix(size, size, &B);

  // Заполняем одинаковыми значениями
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      A.matrix[i][j] = B.matrix[i][j] = (i + j) * 0.001;
    }
  }

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

Suite* eq_matrix_suite(void) {
  Suite* s;
  TCase* tc_core;
  s = suite_create("s21_eq_matrix");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_eq_identical_int);
  tcase_add_test(tc_core, test_eq_identical_float);
  tcase_add_test(tc_core, test_eq_different_sizes);
  tcase_add_test(tc_core, test_eq_one_element_different);
  tcase_add_test(tc_core, test_eq_within_epsilon);
  tcase_add_test(tc_core, test_eq_beyond_epsilon);
  tcase_add_test(tc_core, test_eq_zero_matrices);
  tcase_add_test(tc_core, test_eq_null_matrix);
  tcase_add_test(tc_core, test_eq_invalid_dimensions);
  tcase_add_test(tc_core, test_eq_large_numbers);
  tcase_add_test(tc_core, test_eq_large_matrices);
  suite_add_tcase(s, tc_core);
  return s;
}