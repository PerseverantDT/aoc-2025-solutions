#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

FILE *check_file = nullptr;

[[gnu::nonnull(1)]]
[[gnu::access(read_only, 1)]]
uint64_t get_subtotal_of_invalid_ids(char const *line) {
    uint64_t start;
    uint64_t end;
    uint64_t subtotal = 0;

    if (sscanf(line, "%" SCNu64 "-%" SCNu64, &start, &end) != 2) {
        fprintf(stderr, "Failed to parse range: \"%s\"", line);
        return 0;
    }

    if (start > end) {
        fprintf(stderr, "Start is greater than end: %" PRIu64 " > %" PRIu64, start, end);
        return 0;
    }

    if (check_file != nullptr) {
        fprintf(check_file, "Checking %" PRIu64 " to %" PRIu64 "\n", start, end);
    }

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
    uint64_t invalid_id_sum = 0;

    while (fscanf(input_file, "%c", &current_character) == 1) {
        if (current_character == ',') {
            line[buffer_index] = '\0';
            invalid_id_sum += get_subtotal_of_invalid_ids(line);
            buffer_index = 0;
        }
        else {
            line[buffer_index] = current_character;
            buffer_index++;
        }
    }

    // Process the final range
    line[buffer_index] = '\0';
    invalid_id_sum += get_subtotal_of_invalid_ids(line);
    buffer_index = 0;

    printf("Part 1: The sum of all invalid IDs is %" PRIu64 "\n", invalid_id_sum);
}