#include <stdio.h>
#include <stdlib.h>

/* Written by Gemini AI */

/**
 * D2U - CP/M to Linux Text Converter
 * 1. Strips 0x1A (Ctrl-Z) EOF markers.
 * 2. Converts CRLF (\r\n) to LF (\n).
 * 
 * Build: gcc -O2 d2u.c -o d2u
 */

int main(int argc, char *argv[]) {
    FILE *in, *out;
    int c;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    in = fopen(argv[1], "rb");
    if (!in) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    out = fopen(argv[2], "wb");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return EXIT_FAILURE;
    }

    /* Process byte by byte */
    while ((c = fgetc(in)) != EOF) {
        /* CP/M EOF check: Stop everything at the first 0x1A */
        if (c == 0x1A) {
            break;
        }

        /* * CRLF to LF Conversion: 
         * If we see \r, we check the next character.
         * If the next is \n, we just let the next iteration handle the \n.
         * If it's NOT \n, we preserve the \r (rare in CP/M, but good practice).
         */
        if (c == '\r') {
            int next = fgetc(in);
            if (next == '\n') {
                /* Found \r\n, write only the \n */
                fputc('\n', out);
            } else {
                /* Found \r followed by something else, preserve both */
                fputc('\r', out);
                if (next != EOF && next != 0x1A) {
                    fputc(next, out);
                } else if (next == 0x1A) {
                    break;
                }
            }
        } else {
            fputc(c, out);
        }
    }

    fclose(in);
    fclose(out);
    return EXIT_SUCCESS;
}
