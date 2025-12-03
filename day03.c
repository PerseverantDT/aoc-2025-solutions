#include <alloca.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *check_file = nullptr;
constexpr long BUFFER_LENGTH = 2048;

[[gnu::access(read_only, 1, 2)]]
long get_highest_joltage(
    char const *line,
    int line_length,
    int number_of_characters_to_fetch
) {
    long joltage = 0;
    int start_index = -1;

    for (int i = 0; i < number_of_characters_to_fetch; i++) {
        long highest_joltage = 0;
        int highest_joltage_index = -1;

        int search_limit = line_length - (number_of_characters_to_fetch - i) + 1;
        for (int j = start_index + 1; j < search_limit; j++) {
            long value = line[j] - '0';
            if (value > highest_joltage) {
                highest_joltage = value;
                highest_joltage_index = j;
            }

            if (value == 9) {
                // We can't have digits higher than 9, so we stop early.
                break;
            }
        }

        assert(highest_joltage_index != -1);

        start_index = highest_joltage_index;
        joltage *= 10;
        joltage += highest_joltage;
    }

    return joltage;
}

int main(void) {
    FILE *input_file = fopen("input-03.txt", "r");
    if (input_file == nullptr) {
        fprintf(stderr, "Failed to open inputs: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    check_file = fopen("check.txt", "w");
    char buffer[BUFFER_LENGTH];
    long part1_total_joltage = 0;
    long part2_total_joltage = 0;

    while (fgets(buffer, BUFFER_LENGTH, input_file) != nullptr) {
        long line_length = strlen(buffer);

        // Remove trailing new line
        if (buffer[line_length - 1] == '\n' || buffer[line_length - 1] == '\r') {
            buffer[line_length - 1] = '\0';
            line_length--;
        }
        if (buffer[line_length - 1] == '\n' || buffer[line_length - 1] == '\r') {
            buffer[line_length - 1] = '\0';
            line_length--;
        }

        long part1_highest_joltage = get_highest_joltage(buffer, line_length, 2);
        long part2_highest_joltage = get_highest_joltage(buffer, line_length, 12);

        part1_total_joltage += part1_highest_joltage;
        part2_total_joltage += part2_highest_joltage;
        if (check_file != nullptr) {
            fprintf(
                check_file,
                "Joltage of \"%s\":\n\tPart 1: %ld (total: %ld)\n\tPart 2: %ld (total: %ld)\n",
                buffer,
                part1_highest_joltage,
                part1_total_joltage,
                part2_highest_joltage,
                part2_total_joltage
            );
        }
    }

    printf("Part 1: Total joltage: %ld\n", part1_total_joltage);
    printf("Part 2: Total joltage: %ld\n", part2_total_joltage);
}
