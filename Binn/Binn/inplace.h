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

// WDM驱动中的_atoi64实现
ULONG _atoi64(PCSTR str);

// WDM驱动中的atoi实现
int atoi(const char* str);

// 判断字符是否为数字
BOOLEAN IsDigit(char c);

// 计算10的幂次方
double Pow10(int power);

// WDM驱动中的atof实现
double atof(const char* str);

// WDM驱动中的strdup实现
PCHAR Rt_strdup(PCSTR src);

// 判断字符是否为大写字母
BOOLEAN IsUpperCase(char c);

// 将字符转换为小写
char ToLowerCase(char c);

// WDM驱动中的stricmp实现
int Rt_stricmp(const char* s1, const char* s2);

// WDM驱动中的strnicmp实现
int Rt_strnicmp(const char* s1, const char* s2, size_t n);



