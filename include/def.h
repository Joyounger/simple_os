#ifndef __DEF_H__
#define __DEF_H__

#define U32 unsigned long
#define U16 unsigned short
#define S32 int
#define S16 short int
#define U8  unsigned char
#define	S8  char

#define	BYTE	char
#define	WORD 	short
#define	DWORD	int
#define	UINT	U32
#define	LPSTR	U8 *		

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   int   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */

#define TRUE 	1   
#define FALSE 	0
#define OK		1
#define FAIL	0

#define	SIZE_1K		0x00000400
#define	SIZE_2K		0x00000800
#define	SIZE_4K		0x00001000
#define	SIZE_8K		0x00002000
#define	SIZE_16K	0x00004000
#define	SIZE_32K	0x00008000
#define	SIZE_64K	0x00010000
#define	SIZE_128K	0x00020000
#define	SIZE_256K	0x00040000
#define	SIZE_512K	0x00080000
#define	SIZE_1M		0x00100000
#define	SIZE_2M		0x00200000
#define	SIZE_4M		0x00400000
#define	SIZE_8M		0x00800000
#define	SIZE_16M	0x01000000
#define	SIZE_32M	0x02000000
#define	SIZE_64M	0x04000000
#define	SIZE_128M	0x08000000
#define	SIZE_256M	0x10000000
#define	SIZE_512M	0x20000000
#define	SIZE_1G		0x40000000
#define	SIZE_2G		0x80000000

#define	ENTER_KEY	0x0d
#define	BACK_KEY	0x08
#define BEEP_KEY	0x07
#define UP_KEY		0x41
#define DOWN_KEY	0x42
#define RIGHT_KEY	0x43
#define LEFT_KEY	0x44
#define HOME_KEY	0x48
#define END_KEY		0x4b
#define	ESC_KEY		0x1b


/* Sets the result on bPort */
#define BIT_SET(bPort,bBitMask)        (bPort |= bBitMask)
#define BIT_CLR(bPort,bBitMask)        (bPort &= ~bBitMask)

/* Returns the result */
#define GET_BIT_SET(bPort,bBitMask)    (bPort | bBitMask)
#define GET_BIT_CLR(bPort,bBitMask)    (bPort & ~bBitMask)

/* Returns 0 if the condition is False & a non-zero value if it is True */
#define TEST_BIT_SET(bPort,bBitMask)   (bPort & bBitMask)
#define TEST_BIT_CLR(bPort,bBitMask)   ((~bPort) & bBitMask)

#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0])) //求数组长度


#endif /*__DEF_H__*/

