#pragma once
#include <ntddk.h>

// These macros must exactly match those in the Windows SDK's intsafe.h.
#define INT8_MIN         (-127i8 - 1)
#define INT16_MIN        (-32767i16 - 1)
#define INT32_MIN        (-2147483647i32 - 1)
#define INT64_MIN        (-9223372036854775807i64 - 1)
#define INT8_MAX         127i8
#define INT16_MAX        32767i16
#define INT32_MAX        2147483647i32
#define INT64_MAX        9223372036854775807i64
#define UINT8_MAX        0xffui8
#define UINT16_MAX       0xffffui16
#define UINT32_MAX       0xffffffffui32
#define UINT64_MAX       0xffffffffffffffffui64

#define INT_MAX       2147483647    /* maximum (signed) int value */
#define INT_MIN     (-2147483647 - 1) /* minimum (signed) int value */

extern int _fltused;

PVOID ReallocateMemory(PVOID OriginalPtr, SIZE_T NewSize);

// WDM�����е�_atoi64ʵ��
ULONG _atoi64(PCSTR str);

// WDM�����е�atoiʵ��
int atoi(const char* str);

// �ж��ַ��Ƿ�Ϊ����
BOOLEAN IsDigit(char c);

// ����10���ݴη�
double Pow10(int power);

// WDM�����е�atofʵ��
double atof(const char* str);

// WDM�����е�strdupʵ��
PCHAR Rt_strdup(PCSTR src);

// �ж��ַ��Ƿ�Ϊ��д��ĸ
BOOLEAN IsUpperCase(char c);

// ���ַ�ת��ΪСд
char ToLowerCase(char c);

// WDM�����е�stricmpʵ��
int Rt_stricmp(const char* s1, const char* s2);

// WDM�����е�strnicmpʵ��
int Rt_strnicmp(const char* s1, const char* s2, size_t n);



