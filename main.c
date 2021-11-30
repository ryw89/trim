#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WS_BUF_SIZE 256

bool is_non_newline_whitespace(char s) {
    switch (s) {
        case ' ':
            return true;
        case '\f':
            return true;
        case '\r':
            return true;
        case '\t':
            return true;
        case '\v':
            return true;
        default:
            return false;
    }
}

char *realloc_whitespace_buffer(char *buf, size_t buf_size, bool larger) {
    if (larger) {
        // Make the array larger
        buf_size = buf_size + WS_BUF_SIZE * sizeof(char);
    } else {
        // Reset back to initial size
        buf_size = WS_BUF_SIZE;
        buf[0] = '\0';
    }
    buf = realloc(buf, buf_size);

    if (buf == NULL) {
        fprintf(stderr,
                "Failed to resize whitespace buffer; "
                "exiting.\n");
        exit(EXIT_FAILURE);
    }
    return buf;
}

void print_help() {
    char *help =
        "trim: Trim whitespace line-by-line.\n\
Example usage: echo \"  a string   \" | trim\n\n\
  --left-only    trim left side only\n\
  --right-only   trim right side only\n";

    printf("%s", help);
}

int main(int argc, char const **argv) {
    if (argc > 2) {
        fprintf(stderr, "trim: Too many arguments.\n");
        return EXIT_SUCCESS;
    }

    // Program-wide flags for trimming left and/or right whitespace.
    bool ltrim = true;
    bool rtrim = true;

    // Allow for trimming only left or right-side whitespace, or
    // printing help
    if (argc == 2) {
        if (strcmp(argv[1], "--left-only") == 0) {
            rtrim = false;
        } else if (strcmp(argv[1], "--right-only") == 0) {
            ltrim = false;
        } else if (strcmp(argv[1], "--help") == 0) {
            print_help();
            return EXIT_SUCCESS;
        } else {
            fprintf(stderr, "Invalid argument: %s\n.", argv[1]);
            return EXIT_SUCCESS;
        }
    }

    // Single char from stdin
    char in;

    // Boolean to indicate if we're at the beginning of a line and
    // haven't hit a non-whitespace character yet
    bool at_line_begin = true;

    // Character array for storing potentially-printed whitespace that
    // appears in the middle of a line
    char *ws_buf;
    size_t buf_size = WS_BUF_SIZE * sizeof(char);
    ws_buf = malloc(buf_size);
    ws_buf[0] = '\0';  // Better not to leave uninitialized

    while (read(STDIN_FILENO, &in, 1) > 0) {
        if (is_non_newline_whitespace(in)) {
            if (at_line_begin) {
                if (ltrim) {
                    // Do nothing -- we still haven't hit non-whitespace
                    // in this line.
                    ;
                } else {
                    // Not trimming left-side whitespace, so print
                    // anyway.
                    printf("%s", &in);
                }
            } else {
                if (rtrim) {
                    // This line has hit some non-whitespace previously,
                    // so add this whitespace to the whitespace buffer for
                    // later (possible) printing

                    // But first, we'll check the size of the buffer and
                    // realloc if need be.
                    if (strlen(ws_buf) + 1 >= buf_size) {
                        buf_size = buf_size + WS_BUF_SIZE * sizeof(char);
                        ws_buf =
                            realloc_whitespace_buffer(ws_buf, buf_size, true);
                    }

                    strcat(ws_buf, &in);

                } else {
                    // Not trimming right-side whitespace, so print
                    // anyway.
                    printf("%s", &in);
                }
            }
        } else if (in == '\n') {
            // Hit a newline -- print a newline, put a null terminator
            // at the beginning of the whitespace buffer and reset its
            // size, and also reset the at_line_begin flag to true.
            printf("\n");
            ws_buf = realloc_whitespace_buffer(ws_buf, 0, false);
            at_line_begin = true;

        } else {
            // Hit a non-whitespace character, so first print the
            // whitespace buffer and reset it
            printf("%s", ws_buf);
            ws_buf = realloc_whitespace_buffer(ws_buf, 0, false);

            // And now print the character from stdin itself.
            printf("%s", &in);

            // We've hit a non-whitespace character in this line, so
            // change this flag.
            at_line_begin = false;
        }
    }
    free(ws_buf);
    return EXIT_SUCCESS;
}
