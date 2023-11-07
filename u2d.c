/* U2D.COM is a simplfied UNIX2DOS clone for CP/M */
/* Written by Dean Jenkins */
/* 23rd October 2023 */

/* License: MIT */

#include <stdio.h>

char *filename_in = NULL;
char *filename_out = "NEWDOS.TXT";

int main(int argc, char *argv[])
{
    FILE *fp_in;
    FILE *fp_out;
    char buf[2];
    int mode = 1;    /* Insert CR */

    printf("U2D.COM is a UNIX2DOS clone\n");
    printf("Written by Dean Jenkins v0.3 07/11/2023\n");
    printf("U2D <input filename>\n");

    if (argc != 2)
    {
         printf("Please specify an input filename\n");   
         exit(1);
    }

    filename_in = argv[1];

    fp_in = fopen(filename_in, "rb");

    if (!fp_in)
    {
         printf("File '%s' failed to open\n", filename_in);   
         exit(1);
    }

    printf("Input file '%s' was opened\n", filename_in);

    fp_out = fopen(filename_out, "wb");

    if (!fp_out)
    {
         printf("File '%s' failed to open\n", filename_out);   
         close(fp_in);
         exit(1);
    }

    printf("Output file '%s' was opened\n", filename_out);

    while (fgets(buf, 2, fp_in) != NULL)
    {
        if (buf[0] == 0x1A) /* EOF marker padding */
            break;

        if (buf[0] == 0x00) /* NUL marker padding */
            break;

        if (buf[0] == 0x0D) /* CR */
            mode = 0;       /* File is already in DOS format */

        if (buf[0] == 0x0A && mode == 1) /* LF */
            fputc(0x0D, fp_out); /* Inject a CR */

        fputc(buf[0], fp_out); /* Write to new file */
    }

    fclose(fp_in);
    fclose(fp_out);
}
