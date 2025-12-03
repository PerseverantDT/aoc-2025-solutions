#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *check_file = nullptr;
constexpr int BUFFER_LENGTH = 2048;

[[gnu::access(read_only, 1, 2)]]
int get_highest_joltage(char const *line, int line_length) {
    int joltage = 0;
    int joltage2 = 0;
    int position = -1;

    // Find the battery with the largest joltage.
    for (int i = 0; i < line_length; i++) {
        char c = line[i];
        if (c - '0' > joltage) {
            joltage = c - '0';
            position = i;
        }
    }

    assert(position != -1);

    if (position + 1 == line_length) {
        // The battery with the largest joltage is the last one. The second one
        // needs to be the highest of the others.

        for (int i = 0; i < position; i++) {
            char c = line[i];
            if (c - '0' > joltage2) {
                joltage2 = c - '0';
            }
        }

        return joltage2 * 10 + joltage;
    }

    for (int i = position + 1; i < line_length; i++) {
        char c = line[i];
        if (c - '0' > joltage2) {
            joltage2 = c - '0';
        }
    }

    return joltage * 10 + joltage2;
}

int main(void) {
    FILE *input_file = fopen("input-03.txt", "r");
    if (input_file == nullptr) {
        fprintf(stderr, "Failed to open inputs: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    check_file = fopen("check.txt", "w");
    char buffer[BUFFER_LENGTH];
    int total_joltage = 0;

    while (fgets(buffer, BUFFER_LENGTH, input_file) != nullptr) {
        int line_length = strlen(buffer);

        // Remove trailing new line
        if (buffer[line_length - 1] == '\n' || buffer[line_length - 1] == '\r') {
            buffer[line_length - 1] = '\0';
            line_length--;
        }
        if (buffer[line_length - 1] == '\n' || buffer[line_length - 1] == '\r') {
            buffer[line_length - 1] = '\0';
            line_length--;
        }

        int highest_joltage = get_highest_joltage(buffer, line_length);

        if (check_file != nullptr) {
            fprintf(check_file, "Joltage of \"%s\": %d\n", buffer, highest_joltage);
        }
        total_joltage += highest_joltage;
    }

    printf("Total joltage: %d\n", total_joltage);
}
