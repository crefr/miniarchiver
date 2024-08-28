#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "archiver.h"

void encode(FILE* infile, FILE* outfile)
{
    assert(infile  != NULL);
    assert(outfile != NULL);
    fputs("cre\n", outfile);

    int ch = 0;
    unsigned char count = 0;
    int lastsymbol = EOF;
    bool aresame = false;

    char base[128] = {};
    while ((ch = fgetc(infile)) != EOF)
    {
        if (ch == lastsymbol)
        {
            if (aresame)
            {
                //printf("%d\n",count);
                count+=1;
            }
            else
            {
                //printf("%d\n",count);
                aresame = true;
                base[count] = '\0';
                fputc(codedByte(0, count), outfile);
                fputs(base, outfile);
                base[0] = (char) ch;
                count = 1;
            }
        }
        else
        {
            if(aresame)
            {
                //printf("%d\n",count);
                aresame = false;
                fputc(codedByte(1, count), outfile);
                fputc(base[0], outfile);
                count = 1;
                base[count] = (char) ch;
            }
            else
            {
                //printf("%d\n",count);
                count += 1;
                base[count] =(char) ch;
                putchar (ch);
            }
        }
        lastsymbol = ch;
    }
    if(aresame)
    {
        fputc(codedByte(1, count), outfile);
        fputc(base[0], outfile);
        count = 1;
        base[count] = (char) ch;
    }
    else
    {
        base[count] = '\0';
        fputc(codedByte(0, count), outfile);
        fputs(base, outfile);
        base[0] = (char) ch;
        count = 1;
    }
    fputc(EOF, outfile);
}

unsigned char codedByte(bool isrep, unsigned char count)
{
    printf("%d\n",count);
    assert(count <= 129);
    unsigned char ans = 0;
    if (isrep)
    {
        ans += 1<<7;
        ans += count - 2;
        return count;
    }
    ans += count - 1;
    return ans;
}
