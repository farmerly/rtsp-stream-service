#include "crypto.h"

#include <cstring>

#define S11 7
#define S12 12
#define S13 17
#define S14 22

#define S21 5
#define S22 9
#define S23 14
#define S24 20

#define S31 4
#define S32 11
#define S33 16
#define S34 23

#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define FF(a, b, c, d, x, s, ac)                            \
    {                                                       \
        (a) += F((b), (c), (d)) + (x) + (unsigned int)(ac); \
        (a) = ROTATE_LEFT((a), (s));                        \
        (a) += (b);                                         \
    }

#define GG(a, b, c, d, x, s, ac)                            \
    {                                                       \
        (a) += G((b), (c), (d)) + (x) + (unsigned int)(ac); \
        (a) = ROTATE_LEFT((a), (s));                        \
        (a) += (b);                                         \
    }

#define HH(a, b, c, d, x, s, ac)                            \
    {                                                       \
        (a) += H((b), (c), (d)) + (x) + (unsigned int)(ac); \
        (a) = ROTATE_LEFT((a), (s));                        \
        (a) += (b);                                         \
    }

#define II(a, b, c, d, x, s, ac)                            \
    {                                                       \
        (a) += I((b), (c), (d)) + (x) + (unsigned int)(ac); \
        (a) = ROTATE_LEFT((a), (s));                        \
        (a) += (b);                                         \
    }

typedef struct {
    unsigned int  state[4];
    unsigned int  count[2];
    unsigned char buffer[64];
} MD5Context;

void md5_initialize(MD5Context *context)
{
    memset(context->count, 0, sizeof(int) * 2);
    context->state[0] = 0x67452301; // 01100111 01000101 00100011 00000001
    context->state[1] = 0xefcdab89; // 11101111 11001101 10101011 10001001
    context->state[2] = 0x98badcfe; // 10011000 10111010 11011100 11111110
    context->state[3] = 0x10325476; // 00010000 00110010 01010100 01110110
}

void md5_encode(unsigned char *output, unsigned int *input, int len)
{
    unsigned int i, j;
    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = (unsigned char)(input[i] & 0xff);
        output[j + 1] = (unsigned char)((input[i] >> 8) & 0xff);
        output[j + 2] = (unsigned char)((input[i] >> 16) & 0xff);
        output[j + 3] = (unsigned char)((input[i] >> 24) & 0xff);
    }
}

void md5_decode(unsigned int *output, const unsigned char *input, int len)
{
    unsigned int i, j;
    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[i] = ((unsigned int)input[j]) | (((unsigned int)input[j + 1]) << 8) | (((unsigned int)input[j + 2]) << 16) |
                    (((unsigned int)input[j + 3]) << 24);
    }
}

void md5_transform(unsigned int state[4], const unsigned char block[64])
{
    unsigned int a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    md5_decode(x, block, 64);

    /*Round1*/
    FF(a, b, c, d, x[0], S11, 0xd76aa478);  /*1*/
    FF(d, a, b, c, x[1], S12, 0xe8c7b756);  /*2*/
    FF(c, d, a, b, x[2], S13, 0x242070db);  /*3*/
    FF(b, c, d, a, x[3], S14, 0xc1bdceee);  /*4*/
    FF(a, b, c, d, x[4], S11, 0xf57c0faf);  /*5*/
    FF(d, a, b, c, x[5], S12, 0x4787c62a);  /*6*/
    FF(c, d, a, b, x[6], S13, 0xa8304613);  /*7*/
    FF(b, c, d, a, x[7], S14, 0xfd469501);  /*8*/
    FF(a, b, c, d, x[8], S11, 0x698098d8);  /*9*/
    FF(d, a, b, c, x[9], S12, 0x8b44f7af);  /*10*/
    FF(c, d, a, b, x[10], S13, 0xffff5bb1); /*11*/
    FF(b, c, d, a, x[11], S14, 0x895cd7be); /*12*/
    FF(a, b, c, d, x[12], S11, 0x6b901122); /*13*/
    FF(d, a, b, c, x[13], S12, 0xfd987193); /*14*/
    FF(c, d, a, b, x[14], S13, 0xa679438e); /*15*/
    FF(b, c, d, a, x[15], S14, 0x49b40821); /*16*/

    /*Round2*/
    GG(a, b, c, d, x[1], S21, 0xf61e2562);  /*17*/
    GG(d, a, b, c, x[6], S22, 0xc040b340);  /*18*/
    GG(c, d, a, b, x[11], S23, 0x265e5a51); /*19*/
    GG(b, c, d, a, x[0], S24, 0xe9b6c7aa);  /*20*/
    GG(a, b, c, d, x[5], S21, 0xd62f105d);  /*21*/
    GG(d, a, b, c, x[10], S22, 0x2441453);  /*22*/
    GG(c, d, a, b, x[15], S23, 0xd8a1e681); /*23*/
    GG(b, c, d, a, x[4], S24, 0xe7d3fbc8);  /*24*/
    GG(a, b, c, d, x[9], S21, 0x21e1cde6);  /*25*/
    GG(d, a, b, c, x[14], S22, 0xc33707d6); /*26*/
    GG(c, d, a, b, x[3], S23, 0xf4d50d87);  /*27*/
    GG(b, c, d, a, x[8], S24, 0x455a14ed);  /*28*/
    GG(a, b, c, d, x[13], S21, 0xa9e3e905); /*29*/
    GG(d, a, b, c, x[2], S22, 0xfcefa3f8);  /*30*/
    GG(c, d, a, b, x[7], S23, 0x676f02d9);  /*31*/
    GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /*32*/

    /*Round3*/
    HH(a, b, c, d, x[5], S31, 0xfffa3942);  /*33*/
    HH(d, a, b, c, x[8], S32, 0x8771f681);  /*34*/
    HH(c, d, a, b, x[11], S33, 0x6d9d6122); /*35*/
    HH(b, c, d, a, x[14], S34, 0xfde5380c); /*36*/
    HH(a, b, c, d, x[1], S31, 0xa4beea44);  /*37*/
    HH(d, a, b, c, x[4], S32, 0x4bdecfa9);  /*38*/
    HH(c, d, a, b, x[7], S33, 0xf6bb4b60);  /*39*/
    HH(b, c, d, a, x[10], S34, 0xbebfbc70); /*40*/
    HH(a, b, c, d, x[13], S31, 0x289b7ec6); /*41*/
    HH(d, a, b, c, x[0], S32, 0xeaa127fa);  /*42*/
    HH(c, d, a, b, x[3], S33, 0xd4ef3085);  /*43*/
    HH(b, c, d, a, x[6], S34, 0x4881d05);   /*44*/
    HH(a, b, c, d, x[9], S31, 0xd9d4d039);  /*45*/
    HH(d, a, b, c, x[12], S32, 0xe6db99e5); /*46*/
    HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /*47*/
    HH(b, c, d, a, x[2], S34, 0xc4ac5665);  /*48*/

    /*Round4*/
    II(a, b, c, d, x[0], S41, 0xf4292244);  /*49*/
    II(d, a, b, c, x[7], S42, 0x432aff97);  /*50*/
    II(c, d, a, b, x[14], S43, 0xab9423a7); /*51*/
    II(b, c, d, a, x[5], S44, 0xfc93a039);  /*52*/
    II(a, b, c, d, x[12], S41, 0x655b59c3); /*53*/
    II(d, a, b, c, x[3], S42, 0x8f0ccc92);  /*54*/
    II(c, d, a, b, x[10], S43, 0xffeff47d); /*55*/
    II(b, c, d, a, x[1], S44, 0x85845dd1);  /*56*/
    II(a, b, c, d, x[8], S41, 0x6fa87e4f);  /*57*/
    II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /*58*/
    II(c, d, a, b, x[6], S43, 0xa3014314);  /*59*/
    II(b, c, d, a, x[13], S44, 0x4e0811a1); /*60*/
    II(a, b, c, d, x[4], S41, 0xf7537e82);  /*61*/
    II(d, a, b, c, x[11], S42, 0xbd3af235); /*62*/
    II(c, d, a, b, x[2], S43, 0x2ad7d2bb);  /*63*/
    II(b, c, d, a, x[9], S44, 0xeb86d391);  /*64*/

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    memset((char *)x, 0, sizeof(x));
}

void md5_update(MD5Context *context, const unsigned char *buf, int len)
{
    unsigned int i, index, partLen;
    index = (unsigned int)((context->count[0] >> 3) & 0x3F);
    if ((context->count[0] += ((unsigned int)len << 3)) < ((unsigned int)len << 3)) {
        context->count[1]++;
    }
    context->count[1] += ((unsigned int)len >> 29);

    partLen = 64 - index;
    if (len >= partLen) {
        memcpy((char *)&context->buffer[index], (char *)buf, partLen);
        md5_transform(context->state, context->buffer);

        for (i = partLen; i + 63 < len; i += 64) {
            md5_transform(context->state, &buf[i]);
        }
        index = 0;
    } else {
        i = 0;
    }
    memcpy((char *)&context->buffer[index], (char *)&buf[i], len - i);
}

void md5_final(MD5Context *context, unsigned char digest[16])
{
    static unsigned char PADDING[64] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                        0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    unsigned char bits[8];
    unsigned int  index, padLen;

    md5_encode(bits, context->count, 8);
    index = (unsigned int)((context->count[0] >> 3) & 0x3F);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    md5_update(context, PADDING, padLen);
    md5_update(context, bits, 8);
    md5_encode(digest, context->state, 16);
    memset((char *)context, 0, sizeof(*context));
}

bool md5_encrypt(unsigned char *dst, const unsigned char *src, int len)
{
    if (len < strlen((char *)src)) {
        return false;
    }

    MD5Context context;
    md5_initialize(&context);
    md5_update(&context, src, len);
    md5_final(&context, dst);
    return true;
}

bool md5_encrypt_hex(unsigned char *dst, const unsigned char *src, int len)
{
    static const char HEXMAP[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    unsigned char ret[16] = {0};
    if (!md5_encrypt(ret, src, len)) {
        return false;
    }

    for (int i = 0; i < 16; i++) {
        dst[i * 2] = HEXMAP[((ret[i] & 0xF0) >> 4)];
        dst[i * 2 + 1] = HEXMAP[ret[i] & 0x0F];
    }
    return true;
}

int base64_encode(const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen)
{
    static const unsigned char BASE64_MAP_ASC[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

    if (srcLen <= 0 || ((srcLen + 2) / 3) * 4 > dstMaxLen) {
        return -1;
    }
    memset(dstBuf, 0, dstMaxLen);
    int dstLen = 0;
    for (int i = 0; i < srcLen; i += 3) {
        if (i + 1 == srcLen) {
            dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i] & 0xFC) >> 2];
            dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i] & 0x03) << 4];
            dstBuf[dstLen++] = '=';
            dstBuf[dstLen++] = '=';
        } else if (i + 2 == srcLen) {
            dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i] & 0xFC) >> 2];
            dstBuf[dstLen++] = BASE64_MAP_ASC[((srcBuf[i] & 0x03) << 4) | ((srcBuf[i + 1] & 0xF0) >> 4)];
            dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i + 1] & 0x0F) << 2];
            dstBuf[dstLen++] = '=';
        } else {
            dstBuf[dstLen++] = BASE64_MAP_ASC[(srcBuf[i] & 0xFC) >> 2];
            dstBuf[dstLen++] = BASE64_MAP_ASC[((srcBuf[i] & 0x03) << 4) | ((srcBuf[i + 1] & 0xF0) >> 4)];
            dstBuf[dstLen++] = BASE64_MAP_ASC[((srcBuf[i + 1] & 0x0F) << 2) | ((srcBuf[i + 2] & 0xC0) >> 6)];
            dstBuf[dstLen++] = BASE64_MAP_ASC[srcBuf[i + 2] & 0x3F];
        }
    }
    return dstLen;
}

int base64_decode(const char *srcBuf, int srcLen, char *dstBuf, int dstMaxLen)
{
    static const unsigned char BASE64_MAP_DESC[256] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E,
        0x00, 0x00, 0x00, 0x3F, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
        0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
        0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    if (srcLen % 4 != 0) {
        return -1;
    }
    memset(dstBuf, 0, dstMaxLen);
    int dstLen = 0;
    for (int i = 0; i < srcLen; i += 4) {
        dstBuf[dstLen] = (BASE64_MAP_DESC[(unsigned char)srcBuf[i]] & 0x3F) << 2;
        dstBuf[dstLen++] |= (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 1]] & 0x30) >> 4;

        dstBuf[dstLen] = (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 1]] & 0x0F) << 4;
        dstBuf[dstLen++] |= (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 2]] & 0x3C) >> 2;

        dstBuf[dstLen] = (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 2]] & 0x03) << 6;
        dstBuf[dstLen++] |= (BASE64_MAP_DESC[(unsigned char)srcBuf[i + 3]] & 0x3F);
    }

    for (int i = srcLen - 1; i > 0; i--) {
        if (srcBuf[i] != '=') {
            break;
        }
        dstLen -= 1;
    }
    return dstLen;
}
