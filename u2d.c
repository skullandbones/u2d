/* U2D.COM is a simplfied UNIX2DOS clone for CP/M */
/* Written by Dean Jenkins */
/* 23rd October 2023 */
/* Written for z88dk (ANSI C) */
/* z88dk.zcc +cpm -O3 -create-app -o U2D.COM u2d.c */

/* License: MIT */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* Required for unlink/remove in z88dk */

const char *filename_out = "NEWDOS.TXT";

int main(int argc, char *argv[])
{
    FILE *fp_in;
    FILE *fp_out;
    char *filename_in;
    int c;
    int last_c = 0;
    int overwrite = 0;
    int arg_idx = 0;

    printf("U2D.COM is a UNIX2DOS clone\n");
    printf("Written by Dean Jenkins v0.4 10/02/2026\n");

    /* Handle Arguments */
    if (argc == 2)
        arg_idx = 1;
    
    if (argc == 3 && strcmp(argv[1], "-O") == 0) {
        printf("Overwrite selected\n");
        overwrite = 1;
        arg_idx = 2;
    }

    if (argc <= 1 || argc > arg_idx + 1) {
        printf("Usage: U2D [-O] <file>\n");
        return EXIT_FAILURE;
    }

    filename_in = argv[arg_idx];

    /* Open files in Binary mode */
    if ((fp_in = fopen(filename_in, "rb")) == NULL) {
        printf("Can't open '%s'\n", filename_in);
        return EXIT_FAILURE;
    }

    if ((fp_out = fopen(filename_out, "wb")) == NULL) {
        printf("Can't create %s\n", filename_out);
        fclose(fp_in);
        return EXIT_FAILURE;
    }
 
    /* Conversion Loop */
    while ((c = fgetc(fp_in)) != EOF) {
        /* CP/M EOF is usually 0x1A, but z88dk's fgetc handles 
           filesystem EOF. We check 0x1A specifically for text compatibility. */
        if (c == 0x1A)
            break; 

        /* If LF (\n) without preceding CR (\r), inject CR */
        if (c == 0x0A && last_c != 0x0D) {
            fputc(0x0D, fp_out);
        }
        
        fputc(c, fp_out);
        last_c = c;
    }

    /* Proper CP/M Text Termination */
    fputc(0x1A, fp_out);

    fclose(fp_in);
    fclose(fp_out);

    if (overwrite) {
        /* Replace the original file with the temporary file. */
        /* remove() is the ANSI standard, unlink() also works in z88dk */
        if (remove(filename_in) != 0) {
            printf("Error erasing original '%s'\n", filename_in);
            return EXIT_FAILURE;
        }
        
        if (rename(filename_out, filename_in) != 0) {
            printf("Error renaming temporary file '%s'\n", filename_out);
            return EXIT_FAILURE;
        }
        printf("File '%s' updated.\n", filename_in);
    } else {
        printf("Done. Output in '%s'\n", filename_out);
    }

    return EXIT_SUCCESS;
}
