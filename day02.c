#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

FILE *check_file = nullptr;

struct range {
    uint64_t start;
    uint64_t end;
};

struct range get_range(char const *line) {
    struct range range;

    if (sscanf(line, "%" SCNu64 "-%" SCNu64, &range.start, &range.end) != 2) {
        fprintf(stderr, "Failed to parse range: \"%s\"", line);
        range.start = 0;
        range.end = 0;
    }

    return range;
}

[[gnu::nonnull(2), gnu::access(write_only, 2, 3)]]
static bool convert_number_to_string(
    uint64_t num,
    char *buffer,
    size_t buffer_length
) {
    size_t num_of_digits = log10(num) + 1;
    if (buffer_length < num_of_digits + 1) {
        return false;
    }

    for (int index = num_of_digits - 1; num > 0; index--) {
        uint64_t digit = num % 10;
        buffer[index] = '0' + digit;
        num /= 10;
    }
    buffer[num_of_digits] = '\0';

    return true;
}

uint64_t part2_get_subtotal_of_invalid_ids(uint64_t start, uint64_t end) {
    char num_as_str[21];
    char pattern[11];
    uint64_t subtotal = 0;

    for (auto num = start; num <= end; num++) {
        bool is_invalid = false;
        if (!convert_number_to_string(num, num_as_str, 21)) {
            fprintf(stderr, "Could not convert %" PRIu64 " into string.\n", num);
            continue;
        }

        int num_of_digits = log10(num) + 1;
        if (num_of_digits == 1) {
            continue;
        }

        for (int i = 1; i <= ceilf((float)num_of_digits / 2.0F); i++) {
            bool check_failed = false;
            if (num_of_digits % i != 0) {
                continue;
            }

            strncpy(pattern, num_as_str, i);
            pattern[i] = '\0';
            
            for (int j = 0; j < num_of_digits / i; j++) {
                if (strncmp(num_as_str + (i * j), pattern, i) != 0) {
                    check_failed = true;
                    break;
                }
            }

            if (!check_failed) {
                is_invalid = true;
                break;
            }
        }

        if (is_invalid) {
            subtotal += num;
        }
    }

    return subtotal;
}

uint64_t part1_get_subtotal_of_invalid_ids(uint64_t start, uint64_t end) {
    uint64_t subtotal = 0;

    for (uint64_t i = start; i <= end; i++) {
        uint8_t digit_count = log10(i) + 1;
        if (digit_count % 2 == 1) {
            // Since an invalid ID is made *only* of a sequence of digits repeated twice, odd-length numbers
            // are guaranteed to be valid.
            continue;
        }

        uint64_t separator = powf(10.0F, (float)digit_count / 2);
        uint64_t first_half = i / separator;
        uint64_t second_half = i % separator;

        if (first_half == second_half) {
            if (check_file != nullptr) {
                // fprintf(check_file, "Found invalid id: %" PRIu64 "\n", i);
            }
            subtotal += i;
        }
    }

    return subtotal;
}

int main() {
    FILE *input_file = fopen("input-02.txt", "r");
    if (input_file == nullptr) {
        fprintf(stderr, "Failed to open input file: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    check_file = fopen("check.txt", "w");

    // Each input takes the form of [number]-[number]. With 64-bit integers,
    // this buffer is guaranteed to be large enough for all possible inputs.
    // [number;20 characters max]-[number;20 characters max][null-terminator]
    char line[42];
    char current_character;
    uint8_t buffer_index = 0;
    uint64_t part1_invalid_id_sum = 0;
    uint64_t part2_invalid_id_sum = 0;

    while (fscanf(input_file, "%c", &current_character) == 1) {
        if (current_character == ',') {
            line[buffer_index] = '\0';

            auto range = get_range(line);
            if (check_file != nullptr) {
                fprintf(check_file, "Checking %" PRIu64 " to %" PRIu64 "\n", range.start, range.end);
            }

            part1_invalid_id_sum += part1_get_subtotal_of_invalid_ids(range.start, range.end);
            part2_invalid_id_sum += part2_get_subtotal_of_invalid_ids(range.start, range.end);
            buffer_index = 0;
        }
        else {
            line[buffer_index] = current_character;
            buffer_index++;
        }
    }

    // Process the final range
    line[buffer_index] = '\0';

    auto range = get_range(line);
    if (check_file != nullptr) {
        fprintf(check_file, "Checking %" PRIu64 " to %" PRIu64 "\n", range.start, range.end);
    }

    part1_invalid_id_sum += part1_get_subtotal_of_invalid_ids(range.start, range.end);
    part2_invalid_id_sum += part2_get_subtotal_of_invalid_ids(range.start, range.end);
    buffer_index = 0;

    printf("Part 1: The sum of all invalid IDs is %" PRIu64 "\n", part1_invalid_id_sum);
    printf("Part 2: The sum of all invalid IDs is %" PRIu64 "\n", part2_invalid_id_sum);
}