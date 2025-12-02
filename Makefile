CC = gcc

C_ANALYZER_FLAGS = -fanalyzer -Wanalyzer-allocation-size -Wanalyzer-deref-before-check -Wanalyzer-double-fclose -Wanalyzer-double-free -Wanalyzer-exposure-through-output-file -Wanalyzer-exposure-through-uninit-copy -Wanalyzer-fd-access-mode-mismatch -Wanalyzer-fd-double-close -Wanalyzer-fd-leak -Wanalyzer-fd-phase-mismatch -Wanalyzer-fd-type-mismatch -Wanalyzer-fd-use-after-close -Wanalyzer-fd-use-without-check -Wanalyzer-file-leak -Wanalyzer-free-of-non-heap -Wanalyzer-imprecise-fp-arithmetic -Wanalyzer-infinite-loop -Wanalyzer-infinite-recursion -Wanalyzer-jump-through-null -Wanalyzer-malloc-leak -Wanalyzer-mismatching-deallocation -Wanalyzer-null-argument -Wanalyzer-null-dereference -Wanalyzer-out-of-bounds -Wanalyzer-overlapping-buffers -Wanalyzer-possible-null-argument -Wanalyzer-possible-null-dereference -Wanalyzer-putenv-of-auto-var -Wanalyzer-shift-count-negative -Wanalyzer-shift-count-overflow -Wanalyzer-stale-setjmp-buffer -Wanalyzer-symbol-too-complex -Wanalyzer-tainted-allocation-size -Wanalyzer-tainted-array-index -Wanalyzer-tainted-assertion -Wanalyzer-tainted-divisor -Wanalyzer-tainted-offset -Wanalyzer-tainted-size -Wanalyzer-too-complex -Wanalyzer-undefined-behavior-ptrdiff -Wanalyzer-undefined-behavior-strtok -Wanalyzer-unsafe-call-within-signal-handler -Wanalyzer-use-after-free -Wanalyzer-use-of-pointer-in-stale-stack-frame -Wanalyzer-use-of-uninitialized-value -Wanalyzer-va-arg-type-mismatch -Wanalyzer-va-list-exhausted -Wanalyzer-va-list-leak -Wanalyzer-va-list-use-after-va-end -Wanalyzer-write-to-const -Wanalyzer-write-to-string-literal
C_FLAGS_BASE = -std=gnu23 -Wall -Wextra -Wpedantic -Werror -finput-charset=UTF-8 -fexec-charset=UTF-8

C_FLAGS_DEBUG = $(C_FLAGS_BASE) $(C_ANALYZER_FLAGS)
C_FLAGS_RELEASE = $(C_FLAGS_BASE) -O3 -march=native -flto

.PHONY: debug run_debug release run_release clean

all: debug release

clean:
	@rm day01_debug day01_release day02_debug day02_release

day01_debug: day01.c
	@$(CC) $(C_FLAGS_DEBUG) $^ -o $@


day02_debug: day02.c
	@$(CC) $(C_FLAGS_DEBUG) -lm $^ -o $@

debug: day01_debug day02_debug

run_debug: debug
	@echo "Day 1"
	@echo "----------"
	@./day01_debug
	@echo "----------"
	@echo "Day 2"
	@echo "----------"
	@./day02_debug
	@echo "----------"

day01_release: day01.c
	@$(CC) $(C_FLAGS_RELEASE) $^ -o $@

day02_release: day02.c
	@$(CC) $(C_FLAGS_RELEASE) -lm $^ -o $@

release: day01_release day02_release

run_release: release
	@echo "Day 1:"
	@echo "----------"
	@./day01_release
	@echo "----------"
	@echo "Day 2:"
	@echo "----------"
	@./day02_release
	@echo "----------"
