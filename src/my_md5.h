/*1.填充
在MD5算法中，首先需要对信息进行填充，使其位长对512求余的结果等于448，
并且填充必须进行，即使其位长对512求余的结果等于448。因此，信息的位长
将被扩展至N*512+448，N为一个非负整数，N可以是零。*/
/*每一个变量给出的数值是高字节存于内存低地址，低字节存于内存高地址，
即大端字节序。在程序中变量A、B、C、D的值分别为0x67452301，0xEFCDAB89，
0x98BADCFE，0x10325476*/
#include<iostream>
using namespace std;
#define A 0x67452301
#define B 0xEFCDAB89
#define C 0x98BADCFE
#define D 0x10325476
#define F_func(i, j, k) ((i & j) | (~i & k))
#define G_func(i, j, k) ((i & k) | (j & ~k))
#define H_func(i, j, k) (i ^ j ^ k)
#define I_func(i, j, k) (j ^ (i | (~k)))
#define shift_func(x, n) ((x << n) | (x >> (32-n)))  //MSB = 0 when padding
#define FF_func(a ,b ,c ,d ,Mj ,s ,ti)  a = b + ((a + F(b,c,d) + Mj + ti) << s)
#define GG_func(a ,b ,c ,d ,Mj ,s ,ti)  a = b + ((a + G(b,c,d) + Mj + ti) << s)
#define HH_func(a ,b ,c ,d ,Mj ,s ,ti)  a = b + ((a + H(b,c,d) + Mj + ti) << s)
#define II_func(a, b, c, d, Mj, s, ti)  a = b + ((a + I(b,c,d) + Mj + ti) << s)
const unsigned int Ti[4][16] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8,
	0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
	0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87,
	0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039,
	0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
	0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
const unsigned int shift_left[4][16] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9,
14, 20, 5, 9, 14, 20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
const char hex_16[] = "0123456789abcdef";