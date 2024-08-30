#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "archiver.h"

const char *SIGNATURE = "cre";
const int   SIGNLEN = sizeof(SIGNATURE) / sizeof(char);
const int MAXBLOCKLEN = 127;

/// @brief количество байт в одном мини блоке
static int bytesinblock = 1;

void encode(FILE* infile, FILE* outfile)
{
    assert(infile  != NULL);
    assert(outfile != NULL);
    fputs(SIGNATURE, outfile);

    unsigned char count = 0;
    int *lastbytes = (int *) calloc(bytesinblock, sizeof(char));
    for(int i = 0; i < bytesinblock; i++)
        lastbytes[i] = EOF;
    bool aresame = false;

    //unsigned char base[MAXBLOCKLEN] = {};
    unsigned char *base = (unsigned char *) calloc(bytesinblock * MAXBLOCKLEN, sizeof(char));
    int *bytes = (int *) calloc(bytesinblock, sizeof(char));
    bool theend = false;

    while (!theend)
    {
        for (int i = 0; i < bytesinblock; i++){
            int byte = fgetc(infile);
            bytes[i] = byte;
            if (feof(infile))
                theend = true;
        }
        if (theend)
            break;
        if (count > MAXBLOCKLEN-1){
            encodeBlock(outfile, base, aresame, count);
            aresame = 0;
            count = 0;
            for (int i = 0; i < bytesinblock; i++)
                lastbytes[i] = EOF;
        }
        if (memcmp(bytes, lastbytes, bytesinblock) == 0){
            if (aresame){
                count++;
            }
            else{
                encodeBlock(outfile, base, aresame, count);
                aresame = true;
                for(int i = 0; i < bytesinblock; i++)
                    base[i] = (char) bytes[i];
                count = 1;
            }
        }
        else{
            if(aresame){
                encodeBlock(outfile, base, aresame, count);
                aresame = false;
                for(int i = 0; i < bytesinblock; i++)
                    base[i] = (char) bytes[i];
                count = 1;
            }
            else{
                for(int i = 0; i < bytesinblock; i++)
                    base[i + count * bytesinblock] = (char) bytes[i];
                count++;
            }
        }
        for (int i = 0; i < bytesinblock; i++)
            lastbytes[i] = bytes[i];
    }
    encodeBlock(outfile, base, aresame, count);
}

void encodeBlock(FILE* out, unsigned char *base, int aresame, unsigned char count)
{
    fputc(codedByte(aresame, count), out);
    if (aresame)
        for(int byteindex = 0; byteindex < bytesinblock; byteindex++)
            fputc(base[byteindex], out);
    else
    {
        for (int baseindex = 0; baseindex < count; baseindex++)
            for(int byteindex = 0; byteindex < bytesinblock; byteindex++)
                fputc(base[byteindex + baseindex], out);
    }
}

int decode(FILE* infile, FILE* outfile)
{
    int codedbyte = 0;
    int *bytes = (int *) calloc(bytesinblock, sizeof(char));

    char name[4] = {};
    fgets(name, 4, infile);
    if (strcmp(name, SIGNATURE) != 0)
        return -1;
    while((codedbyte = fgetc(infile)) != EOF)
    {
        unsigned char count = 0;
        if (decodeByte((unsigned char)codedbyte, &count))
        {
            for(int byteindex = 0; byteindex < bytesinblock; byteindex++)
                bytes[byteindex] = fgetc(infile);
            for (int i = 0; i < count; i++)
                for(int byteindex = 0; byteindex < bytesinblock; byteindex++)
                    fputc(bytes[byteindex], outfile);
        }
        else
        {
            for (int i = 0; i < count; i++)
            {
                for(int byteindex = 0; byteindex < bytesinblock; byteindex++)
                    bytes[byteindex] = fgetc(infile);
                for(int byteindex = 0; byteindex < bytesinblock; byteindex++)
                    fputc(bytes[byteindex], outfile);
            }
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

void setBytesInblock(int bytes)
{
    bytesinblock = bytes;
}

