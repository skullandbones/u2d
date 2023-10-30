/* U2D.COM is a simplfied UNIX2DOS clone for CP/M */
/* Written by Dean Jenkins */
/* 23rd October 2023 */

/* License: MIT */

#include <stdio.h>

int main(void)
{
    FILE *fp;
    char buf[2];
    unsigned int count = 0;

    printf("U2D.COM a UNIX2DOS clone\n");

    fp = fopen("TEST.TXT","rb");
    
    if (!fp)
    {
         printf("File failed to open\n");   
         exit(1);
    }

    printf("File was opened\n");

    while (fgets(buf, 2, fp) != NULL)
    {
        count++;
        printf("%4u: %c\n", count, buf[0]);
    
    
    }

    fclose(fp);
}
