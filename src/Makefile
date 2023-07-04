CC := gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
OS := $(shell uname)
CSRC := s21_comp_op.c s21_conv_pars.c s21_ar_op.c
OBJ := $(CSRC:.c=.o)
TESTS := s21_from_int_to_decimal_test s21_add_test
TESTS_RUN := ./s21_from_int_to_decimal_test ./s21_add_test

INCLUDES = -I/opt/goinfre/fletamar/homebrew/include/
LIBS = -L/opt/goinfre/fletamar/homebrew/lib/ -lcheck

ifeq ($(OS),Linux)
FLAGS = -lm -lpthread -lrt -lsubunit -fprofile-arcs -ftest-coverage
else
FLAGS = -lm -lpthread -fprofile-arcs -ftest-coverage
endif

all: clean s21_decimal.a test gcov_report

test: clean s21_decimal.a
	$(CC) $(INCLUDES) s21_add_test.c s21_decimal.a $(FLAGS) $(LIBS) -o s21_add_test
	./s21_add_test
	$(CC) $(INCLUDES) s21_from_float_to_decimal_test.c s21_decimal.a $(FLAGS) $(LIBS) -o s21_from_float_to_decimal_test
	./s21_from_float_to_decimal_test

	$(CC) $(INCLUDES) s21_is_equal_test.c s21_decimal.a $(FLAGS) $(LIBS) -o s21_is_equal_test
	./s21_is_equal_test



	$(CC) $(INCLUDES) s21_from_int_to_decimal_test.c s21_decimal.a $(FLAGS) $(LIBS) -o s21_from_int_to_decimal_test
	./s21_from_int_to_decimal_test

	$(CC) $(INCLUDES) s21_from_decimal_to_float_test.c s21_decimal.a $(FLAGS) $(LIBS) -o s21_from_decimal_to_float_test
	./s21_from_decimal_to_float_test

s21_decimal.a:
	$(CC) $(CFLAGS) -c $(CSRC)
	ar -crs s21_decimal.a $(OBJ)

.PHONY: all test clean gcov_report style style_test cppcheck

gcov_report: clean
	$(CC) $(INCLUDES) s21_math_test.c s21_decimal.c $(FLAGS) $(LIBS) -o s21_math_test
	./s21_math_test
	lcov -t "./s21_math_test"  -o report.info --no-external -c -d .
	genhtml -o report report.info
	open ./report/index.html

	rm -rf *.gcno *gcda *.gcov gcov \
	report report.info

clean:
	rm -rf *.o *.a s21_decimal_test \
	*.gcno *gcda *.gcov gcov \
	report report.info \
	s21_from_float_to_decimal_test s21_add_test

style:
	clang-format -i -style=google *.c *.h

style_test:
	clang-format -n -style=google *.c *.h

cppcheck:
	cppcheck *.c *.h
