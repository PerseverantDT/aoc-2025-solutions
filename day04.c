#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct roll_position {
    int line;
    int column;
};

struct roll_position_list {
    size_t count;
    size_t capacity;
    struct roll_position *data;
};

[[gnu::nonnull(1)]]
[[gnu::access(write_only, 1)]]
static bool initialize_roll_position_list(struct roll_position_list *const list) {
    struct roll_position *data = malloc(sizeof(struct roll_position) * 16);
    if (data == nullptr) {
        fprintf(stderr, "Could not allocate memory for roll position list: %s\n", strerror(errno));
        return false;
    }

    list->count = 0;
    list->capacity = 16;
    list->data = data;

    return true;
}

[[gnu::nonnull(1)]]
[[gnu::access(read_write, 1)]]
static bool expand_roll_position_list(struct roll_position_list *const list) {
    size_t new_capacity = list->capacity * 2;
    struct roll_position *new_data = realloc(list->data, sizeof(struct roll_position) * new_capacity);
    if (new_data == nullptr) {
        fprintf(stderr, "Could not reallocate memory for roll position list: %s\n", strerror(errno));
        return false;
    }

    list->capacity = new_capacity;
    list->data = new_data;

    return true;
}

[[gnu::nonnull(1)]]
[[gnu::access(read_write, 1)]]
static bool push_roll_position(struct roll_position_list *const list, const int line, const int column) {
    if (list->data == nullptr) {
        if (!initialize_roll_position_list(list)) {
            return false;
        }

        goto skip_capacity_check;
    }

    if (list->count == list->capacity) {
        if (!expand_roll_position_list(list)) {
            return false;
        }
    }

    skip_capacity_check:
    list->data[list->count].line = line;
    list->data[list->count].column = column;
    list->count++;

    return true;
}

static int compare_roll_positions(void const *const a, void const *const b) {
    struct roll_position const *const left = a;
    struct roll_position const *const right = b;

    if (left->line - right->line != 0) {
        return left->line - right->line;
    }

    return left->column - right->column;
}

int main(void) {
    FILE *input_file = fopen("input-04.txt", "r");
    if (input_file == nullptr) {
        fprintf(stderr, "Could not open input file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    struct roll_position_list positions = {0, 0, nullptr};

    int line = 0;
    int column = 0;
    bool recent_newline = false;

    int c;
    while ((c = fgetc(input_file)) != EOF) {
        switch (c) {
            case '\n':
            case '\r':
                if (!recent_newline) {
                    line++;
                    column = 0;
                    recent_newline = true;
                }
                break;
            case '@':
                if (!push_roll_position(&positions, line, column)) {
                    fprintf(stderr, "Could not push roll position: %s\n", strerror(errno));
                    return EXIT_FAILURE;
                }
                // Intentional fallthrough.
            default:
                column++;
                recent_newline = false;
                break;
        }
    }

    int accessible_rolls = 0;
    for (size_t i = 0; i < positions.count; i++) {
        struct roll_position position = positions.data[i];
        if (position.line == -1 || position.column == -1) {
            continue;
        }

        int adjacent_roll_count = 0;
        for (int line_offset = -1; line_offset <= 1; line_offset++) {
            for (int column_offset = -1; column_offset <= 1; column_offset++) {
                struct roll_position position_to_check = {position.line + line_offset, position.column + column_offset};
                struct roll_position *neighbor = bsearch(&position_to_check, positions.data, positions.count, sizeof(struct roll_position), compare_roll_positions);

                if (neighbor != nullptr) {
                    adjacent_roll_count++;
                }
            }
        }

        if (adjacent_roll_count < 5) {
            accessible_rolls++;
        }
    }

    printf("Part 1: Accessible rolls: %d\n", accessible_rolls);

    accessible_rolls = 0;
    bool roll_removed = true;

    while (roll_removed) {
        roll_removed = false;
        for (size_t i = 0; i < positions.count; i++) {
            struct roll_position position = positions.data[i];
            if (position.line == -1 || position.column == -1) {
                continue;
            }

            int adjacent_roll_count = 0;
            for (int line_offset = -1; line_offset <= 1; line_offset++) {
                for (int column_offset = -1; column_offset <= 1; column_offset++) {
                    struct roll_position position_to_check = {position.line + line_offset, position.column + column_offset};
                    struct roll_position *neighbor = bsearch(&position_to_check, positions.data, positions.count, sizeof(struct roll_position), compare_roll_positions);

                    if (neighbor != nullptr) {
                        adjacent_roll_count++;
                    }
                }
            }

            if (adjacent_roll_count < 5) {
                accessible_rolls++;
                memmove(&positions.data[i], &positions.data[i + 1], (positions.count - i - 1) * sizeof(struct roll_position));
                positions.count--;
                roll_removed = true;
            }
        }
    }

    printf("Part 2: Accessible rolls: %d\n", accessible_rolls);

    return EXIT_SUCCESS;
}
