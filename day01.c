#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rotation_result {
    int new_position;
    int times_on_zero;
};

[[gnu::const]]
static struct rotation_result rotate_dial_left(
    int dial_position,
    int distance
) [[unsequenced]] {
    if (dial_position > distance) {
        return (struct rotation_result) {dial_position - distance, 0};
    }

    distance -= dial_position;
    int new_position = 100 + (-distance % 100);
    return (struct rotation_result) {new_position == 100 ? 0 : new_position, (dial_position == 0 ? 0 : 1) + (distance / 100)};
}

[[gnu::const]]
static struct rotation_result rotate_dial_right(
    int dial_position,
    int distance
) [[unsequenced]] {
    if (dial_position < 100 - distance) {
        return (struct rotation_result) {dial_position + distance, 0};
    }

    distance -= 100 - dial_position;
    return (struct rotation_result) {distance % 100, 1 + (distance / 100)};
}

int main() {
    FILE *input_file = fopen("input-01.txt", "r");
    if (input_file == nullptr) {
        fprintf(stderr, "Failed to open input file: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    FILE *check_file = fopen("check.txt", "w");

    char direction;
    int distance;
    int dial_position = 50;
    int zero_count = 0;
    int times_on_zero = 0;
    struct rotation_result result;

    if (check_file != nullptr) {
        fprintf(check_file, "Initial position: %d\n", dial_position);
    }
    while (fscanf(input_file, " %c%d", &direction, &distance) == 2) {
        if (direction == 'L') {
            result = rotate_dial_left(dial_position, distance);
        }
        else if (direction == 'R') {
            result = rotate_dial_right(dial_position, distance);
        }
        else {
            // Fallback just in case we get an invalid input.
            result = (struct rotation_result) {dial_position, 0};
        }

        if (check_file != nullptr) {
            fprintf(check_file, "Dir: %c, Dist: %d (%d -> %d), Passed zero %d times (Total: %d)\n", direction, distance, dial_position, result.new_position, result.times_on_zero, times_on_zero + result.times_on_zero);
        }

        dial_position = result.new_position;
        times_on_zero += result.times_on_zero;

        if (dial_position == 0) {
            zero_count++;
        }
    }

    printf("Part 1: The password is %d\n", zero_count);
    printf("Part 2: The password is %d\n", times_on_zero);
}
