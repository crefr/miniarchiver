#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "archiver.h"
const int MAXBLOCKLEN = 127;
void encode(FILE* infile, FILE* outfile)
{
    assert(infile  != NULL);
    assert(outfile != NULL);
    fputs("cre\n", outfile);

    unsigned char count = 0;
    int lastsymbol = EOF;
    bool aresame = false;

    char base[MAXBLOCKLEN] = {};
    int ch = 0;
    while ((ch = fgetc(infile)) != EOF)
    {
        if (count > MAXBLOCKLEN-1)
        {
            encodeBlock(outfile, base, aresame, count);
            aresame = 0;
            count = 0;
            lastsymbol = EOF;
        }
        if (ch == lastsymbol)
        {
            if (aresame)
            {
                count++;
            }
            else
            {
                encodeBlock(outfile, base, aresame, count);
                aresame = true;
                base[0] = (char) ch;
                count = 1;
            }
        }
        else
        {
            if(aresame)
            {
                encodeBlock(outfile, base, aresame, count);
                aresame = false;
                base[0] = (char) ch;
                count = 1;
            }
            else
            {
                base[count] =(char) ch;
                count++;
            }
        }
        lastsymbol = ch;
    }
    encodeBlock(outfile, base, aresame, count);
}

void encodeBlock(FILE* out, char *base, int aresame, unsigned char count)
{
    fputc(codedByte(aresame, count), out);
    if (aresame)
        fputc(base[0], out);
    else
    {
        base[count] = '\0';
        fputs(base, out);
    }
}

int decode(FILE* infile, FILE* outfile)
{
    int ch = 0;
    char name[10] = {};
    fgets(name, 10, infile);
    if (strcmp(name, "cre\n") != 0)
        return -1;
    while((ch = fgetc(infile)) != EOF)
    {
        unsigned char count = 0;
        if (decodeByte((unsigned char)ch, &count))
        {
            ch = fgetc(infile);
            for (int i = 0; i < count; i++)
                fputc(ch, outfile);
        }
        else
        {
            for (int i = 0; i < count; i++)
                fputc(ch = fgetc(infile), outfile);
        }
    }
    return 0;
}

unsigned char codedByte(int isrep, unsigned char count)
{
    assert(count <= 127);
    unsigned char ans = 0;
    if (isrep)
    {
        ans += 1<<7;
        ans += count;
        return ans;
    }
    ans += count;
    return ans;
}

int decodeByte(unsigned char byte, unsigned char *count)
{
    *count = (byte & ((1<<7) - 1));
    if ((byte & (1<<7)) == 0)
        return 0;
    return 1;
}
