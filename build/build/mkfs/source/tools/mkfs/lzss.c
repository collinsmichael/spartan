#define _CRT_SECURE_NO_WARNINGS
/* LZSS encoder-decoder (Haruhiko Okumura; public domain) */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t *decoded;
uint8_t *encoded;
int d, dcount;
int e, ecount;

uint8_t EI = 15; /* typically 10..13 */
uint8_t EJ =  9; /* typically 4..5 */
uint8_t P  =  3; /* If match length <= P then output one character */
#define N (1 << EI)  /* buffer size */
#define F ((1 << EJ) + 1)  /* lookahead buffer size */

int bit_buffer = 0, bit_mask = 128;
unsigned long codecount = 0, textcount = 0;
unsigned char buffer[65536 * 2];
FILE *infile, *outfile;

int lzgetc(void *stream) {
    if (d >= dcount) return EOF;
    return decoded[d++];
}

int lzputc(int ch, void *stream) {
    if (e >= ecount) return EOF;
    encoded[e++] = ch;
    return ch;
}

void error(void) {
    printf("Output error\n");  exit(1);
}

void putbit1(void) {
    bit_buffer |= bit_mask;
    if ((bit_mask >>= 1) == 0) {
        if (lzputc(bit_buffer, outfile) == EOF) error();
        bit_buffer = 0;  bit_mask = 128;  codecount++;
    }
}

void putbit0(void) {
    if ((bit_mask >>= 1) == 0) {
        if (lzputc(bit_buffer, outfile) == EOF) error();
        bit_buffer = 0;  bit_mask = 128;  codecount++;
    }
}

void flush_bit_buffer(void) {
    if (bit_mask != 128) {
        if (lzputc(bit_buffer, outfile) == EOF) error();
        codecount++;
    }
}

void output1(int c) {
    int mask;
    putbit1();
    mask = 256;
    while (mask >>= 1) {
        if (c & mask) putbit1();
        else putbit0();
    }
}

void output2(int x, int y) {
    int mask;
    putbit0();
    mask = N;
    while (mask >>= 1) {
        if (x & mask) putbit1();
        else putbit0();
    }
    mask = (1 << EJ);
    while (mask >>= 1) {
        if (y & mask) putbit1();
        else putbit0();
    }
}

int oldpc, newpc, total;
int encode(char *dec, int dsize, char *enc, int esize, uint8_t ei, uint8_t ej, uint8_t p) {
    decoded = dec;
    encoded = enc;
    dcount = dsize;
    ecount = esize;
    e = 0;
    d = 0;

    EI = ei;
    EJ = ej;
    P  = p;

    int i, j, f1, x, y, r, s, bufferend, c;
    int xx = N - F;
    for (i = 0; i < N - F; i++) {
        buffer[i] = ' ';
    }
    for (i = N - F; i < N * 2; i++) {
        if ((c = lzgetc(infile)) == EOF) break;
        buffer[i] = c;
        textcount++;
    }
    bufferend = i;  r = N - F;  s = 0;
    while (r < bufferend) {
        f1 = (F <= bufferend - r) ? F : bufferend - r;
        x = 0;
        y = 1;
        c = buffer[r];
        for (i = r - 1; i >= s; i--)
            if (buffer[i] == c) {
                for (j = 1; j < f1; j++)
                    if (buffer[i + j] != buffer[r + j]) {
                        break;
                    }
                if (j > y) {
                    x = i;  y = j;
                }
            }
        if (y <= P) {  y = 1;  output1(c);  }
        else output2(x & (N - 1), y - 2);
        r += y;  s += y;
        if (r >= N * 2 - F) {
            for (i = 0; i < N; i++) buffer[i] = buffer[i + N];
            bufferend -= N;
            r -= N;
            s -= N;
            while (bufferend < N * 2) {
                if ((c = lzgetc(infile)) == EOF) break;
                buffer[bufferend++] = c;
                textcount++;
            }
        }
    }
    flush_bit_buffer();
    return e;
}