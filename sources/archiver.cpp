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
    char signCh = 14;
    int signIndex = 0;
    while((signCh = SIGNATURE[signIndex++]) != '\0')
        fputc(signCh, outfile);

    int *base = (int *) calloc(bytesinblock * MAXBLOCKLEN + 100, sizeof(int));
    int *bytes = (int *) calloc(bytesinblock, sizeof(int));
    int *lastbytes = (int *) calloc(bytesinblock, sizeof(int));

    for(int i = 0; i < bytesinblock; i++)
        lastbytes[i] = EOF;

    bool aresame = false;
    bool theend = false;
    unsigned char count = 0;

    while (!theend)
    {
        if (count > MAXBLOCKLEN-2){
            encodeBlock(outfile, base, aresame, count);
            aresame = false;
            count = 0;
            for (int i = 0; i < bytesinblock; i++)
                lastbytes[i] = EOF;
        }
        for (int i = 0; i < bytesinblock; i++){
            int byte = fgetc(infile);
            bytes[i] = byte;
            if (feof(infile))
                theend = true;
        }
        if (theend)
            break;

        if (memcmp(bytes, lastbytes, bytesinblock * sizeof(int)) == 0){
            if (aresame){
                count++;
            }
            else{
                if  (count > 1)
                {
                    count--;
                    encodeBlock(outfile, base, aresame, count);
                }
                aresame = true;
                memcpy(base, bytes, bytesinblock * sizeof(int));
                count = 2;
            }
        }
        else{
            if(aresame){
                encodeBlock(outfile, base, aresame, count);
                aresame = false;
                memcpy(base, bytes, bytesinblock * sizeof(int));
                count = 1;
            }
            else{
                memcpy(base + (count) * bytesinblock, bytes, bytesinblock * sizeof(int));
                count++;
            }
        }
        memcpy(lastbytes, bytes, bytesinblock * sizeof(int));
    }
    encodeBlock(outfile, base, aresame, count);
}

void encodeBlock(FILE* out, int *base, int aresame, unsigned char count)
{
    fputc(codedByte(aresame, count), out);
    if (aresame)
        for(int byteindex = 0; byteindex < bytesinblock; byteindex++)
            fputc(base[byteindex], out);
    else
    {
        for (int baseindex = 0; baseindex < count; baseindex++)
            for(int byteindex = 0; byteindex < bytesinblock; byteindex++)
                fputc(base[byteindex + baseindex * bytesinblock], out);
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
                    fputc(fgetc(infile), outfile);
            }
        }
    }
    return 0;
}

unsigned char codedByte(int isrep, unsigned char count)
{
    //printf("%d\n", count);
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

