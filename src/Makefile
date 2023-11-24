CC := gcc
CFLAGS := -Wall -Werror -Wextra -std=c11
OS := $(shell uname)
CSRC := s21_comp_op.c s21_conv_pars.c s21_ar_op.c s21_util.c s21_another_func.c s21_bit_funcs.c
OBJ := $(CSRC:.c=.o)

ifeq ($(OS),Linux)
FLAGS := -lcheck -lm -lpthread -lrt -lsubunit
else
FLAGS := -lm -lpthread -fprofile-arcs -ftest-coverage -lcheck
endif

TESTS := s21_div_test s21_mul_test s21_add_test s21_sub_test \
         s21_is_less_test s21_is_equal_test s21_is_not_equal_test s21_is_less_or_equal_test s21_is_greater_test s21_is_greater_or_equal_test \
         s21_from_float_to_decimal_test s21_from_decimal_to_int_test s21_from_int_to_decimal_test s21_from_decimal_to_float_test \
         s21_floor_test s21_truncate_test s21_negate_test s21_round_test

all: clean s21_decimal.a

test: $(TESTS)

$(TESTS): %: %.c s21_decimal.a
	$(CC) $< s21_decimal.a $(FLAGS) -o $@
	./$@

s21_decimal.a: $(OBJ)
	ar -crs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all test clean gcov_report style style_test cppcheck

gcov_report: clean $(TESTS)
	$(foreach test, $(TESTS), ./$(test);)
	lcov -t "Code Coverage Report" -o report.info --no-external -c -d .
	genhtml -o report report.info
	open ./report/index.html

clean:
	rm -rf *.o *.a $(TESTS) *.gcno *gcda *.gcov gcov report report.info

style_apply:
	clang-format -i -style=google *.c *.h

style_test:
	clang-format -n -style=google *.c *.h
