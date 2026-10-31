#include <check.h>

#include "../s21_matrix.h"

Suite* eq_matrix_suite(void);
Suite* sum_matrix_suite(void);
Suite* sub_matrix_suite(void);
Suite* mult_number_suite(void);
Suite* mult_matrix_suite(void);
Suite* transpose_suite(void);
Suite* calc_complements_suite(void);
Suite* determinant_suite(void);
Suite* inverse_suite(void);

int main() {
  int failed = 0;
  SRunner* sr = srunner_create(eq_matrix_suite());
  srunner_add_suite(sr, sum_matrix_suite());
  srunner_add_suite(sr, sub_matrix_suite());
  srunner_add_suite(sr, mult_number_suite());
  srunner_add_suite(sr, mult_matrix_suite());
  srunner_add_suite(sr, transpose_suite());
  srunner_add_suite(sr, calc_complements_suite());
  srunner_add_suite(sr, determinant_suite());
  srunner_add_suite(sr, inverse_suite());
  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? 0 : 1;
}
