CC = gcc 
CFLAGS = -Wall -Werror -Wextra -std=c11 -g
LDFLAGS = -lm
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
TEST_FLAGS = -lcheck -lpthread -fprofile-arcs -ftest-coverage -lsubunit
SRC = s21_matrix.c s21_matrix_help_funcs.c
OBJ = $(SRC:.c=.o)

TARGET = s21_matrix.a
TEST_DIR = s21_tests
TEST_TARGET = test
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TARGET_REPORT = gcov_report
REPORT_DIR = report
GCOV_FILES = $(SRC_FILES:.c=.gcno) $(SRC_FILES:.c=.gcda) $(SRC_FILES:.c=.gcov)

all: $(TEST_TARGET) $(TARGET_REPORT)

$(TARGET): $(OBJ)
	ar -r $(TARGET) $(OBJ)
	ranlib $@

$(TEST_TARGET): $(TARGET)
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) $(TEST_FLAGS) -o $@ $(LDFLAGS)
	./$(TEST_TARGET)

$(TARGET_REPORT): clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC) $(TEST_SRC) $(TEST_FLAGS) -o test $(LDFLAGS)
	./$(TEST_TARGET)
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o $(REPORT_DIR) gcov_report.info

clean:
	rm -rf $(OBJ) $(TEST_OBJ) $(TEST_TARGET) $(TARGET) *.gcno *.gcda *.gcov $(TEST_DIR)/*.gcno $(TEST_DIR)/*.gcda $(TEST_DIR)/*.gcov gcov_report.info $(REPORT_DIR) valgrind_log.txt

clang-format:
	clang-format -style=Google -i *.c *.h $(TEST_DIR)/*.c

check:
	clang-format -style=Google -n *.c *.h $(TEST_DIR)/*.c

leak: $(TEST_TARGET)
	CK_FORK=no valgrind --tool=memcheck --leak-check=yes --track-origins=yes --log-file=valgrind_log.txt ./$(TEST_TARGET)

.PHONY: all clean clang-format check leak test gcov_report s21_matrix.a