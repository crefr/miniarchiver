#ifndef ARCHIVER_INCLUDED
#define ARCHIVER_INCLUDED

void encode(FILE* infile, FILE* outfile);
unsigned char codedByte(int isrep, unsigned char count);
int decode(FILE* infile, FILE* outfile);
int decodeByte(unsigned char byte, unsigned char *count);
void encodeBlock(FILE* out, char *base, int aresame, unsigned char count);

#endif
