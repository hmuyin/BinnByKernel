#include "inplace.h"

int _fltused = 1;

PVOID ReallocateMemory(PVOID OriginalPtr, SIZE_T NewSize)
{
	PVOID newPtr = NULL;

	// 处理新大小为 0 的情况（等同于释放内存）
	if (NewSize == 0) {
		if (OriginalPtr) {
			ExFreePool(OriginalPtr);
		}
		return NULL;
	}

	// 如果原指针为空，等同于直接分配内存
	if (!OriginalPtr) {
		return ExAllocatePool2(POOL_FLAG_NON_PAGED, NewSize, 'BINN');
	}

	// 分配新内存块
	newPtr = ExAllocatePool2(POOL_FLAG_NON_PAGED, NewSize, 'BINN');
	if (newPtr) {
		RtlCopyMemory(newPtr, OriginalPtr, NewSize);
		ExFreePool(OriginalPtr);
	}

	return newPtr;
}


ULONG _atoi64(PCSTR str)
{
	ANSI_STRING ansiString;
	UNICODE_STRING unicodeString;
	ULONG result = 0;
	NTSTATUS status;

	// 初始化ANSI_STRING结构
	RtlInitAnsiString(&ansiString, str);

	// 将ANSI字符串转换为Unicode字符串
	status = RtlAnsiStringToUnicodeString(
		&unicodeString,
		&ansiString,
		TRUE  // 分配目标缓冲区
	);

	if (!NT_SUCCESS(status)) {
		return 0;
	}

	// 使用RtlUnicodeStringToInteger进行转换
	status = RtlUnicodeStringToInteger(
		&unicodeString,
		10,  // 基数为10
		&result
	);

	// 释放Unicode字符串缓冲区
	RtlFreeUnicodeString(&unicodeString);

	// 如果转换失败，返回0（与_atoi64行为一致）
	if (!NT_SUCCESS(status)) {
		return 0;
	}

	return result;
}

int atoi(const char* str)
{
	LONGLONG value = _atoi64(str);

	// 范围检查，确保不超出32位有符号整数范围
	if (value > INT_MAX) {
		return INT_MAX;
	}
	else if (value < INT_MIN) {
		return INT_MIN;
	}
	else {
		return (int)value;
	}
}

BOOLEAN IsDigit(char c)
{
	return (c >= '0' && c <= '9');
}

double Pow10(int power)
{
	double result = 1.0;
	int i;

	if (power > 0) {
		for (i = 0; i < power; i++) {
			result *= 10.0;
		}
	}
	else if (power < 0) {
		for (i = 0; i < -power; i++) {
			result /= 10.0;
		}
	}

	return result;
}

double atof(const char* str)
{
	const char* p = str;
	double result = 0.0;
	BOOLEAN negative = FALSE;
	int exponent = 0;
	UNREFERENCED_PARAMETER(exponent);
	BOOLEAN decimalPoint = FALSE;
	int decimalDigits = 0;

	// 跳过前导空格
	while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == '\f' || *p == '\v')
		p++;

	// 处理符号
	if (*p == '-') {
		negative = TRUE;
		p++;
	}
	else if (*p == '+') {
		p++;
	}

	// 解析数字部分
	while (*p != '\0') {
		if (*p == '.') {
			if (decimalPoint) break; // 多个小数点，终止解析
			decimalPoint = TRUE;
			p++;
			continue;
		}

		if (IsDigit(*p)) {
			int digit = *p - '0';
			result = result * 10.0 + (double)digit;

			if (decimalPoint) {
				decimalDigits++;
			}

			p++;
		}
		else {
			break; // 非数字字符，终止解析
		}
	}

	// 应用小数位
	if (decimalDigits > 0) {
		result *= Pow10(-decimalDigits);
	}

	// 处理科学计数法 (e.g., "1.23e-4")
	if (*p == 'e' || *p == 'E') {
		p++;
		BOOLEAN expNegative = FALSE;
		int expValue = 0;

		// 处理指数符号
		if (*p == '-') {
			expNegative = TRUE;
			p++;
		}
		else if (*p == '+') {
			p++;
		}

		// 解析指数值
		while (IsDigit(*p)) {
			expValue = expValue * 10 + (*p - '0');
			p++;
		}

		// 应用指数
		if (expNegative) {
			expValue = -expValue;
		}

		result *= Pow10(expValue);
	}

	// 应用符号
	if (negative) {
		result = -result;
	}

	return result;
}

PCHAR Rt_strdup(PCSTR src)
{
	SIZE_T length;
	PCHAR dest;

	if (src == NULL) {
		return NULL;
	}

	// 计算源字符串长度（包括null终止符）
	length = strlen(src) + 1;

	// 分配内存
	dest = (PCHAR)ExAllocatePool2(POOL_FLAG_NON_PAGED, length, 'DStr');

	if (dest == NULL) {
		return NULL;        // 内存分配失败
	}

	// 复制字符串
	RtlCopyMemory(dest, src, length);

	return dest;
}


BOOLEAN IsUpperCase(char c)
{
	return (c >= 'A' && c <= 'Z');
}


char ToLowerCase(char c)
{
	if (IsUpperCase(c)) {
		return c + ('a' - 'A');
	}
	return c;
}


int Rt_stricmp(const char* s1, const char* s2)
{
	const char* p1 = s1;
	const char* p2 = s2;

	while (*p1 && *p2) {
		char c1 = ToLowerCase(*p1);
		char c2 = ToLowerCase(*p2);

		if (c1 < c2) {
			return -1;
		}
		else if (c1 > c2) {
			return 1;
		}

		p1++;
		p2++;
	}

	// 检查哪个字符串先结束
	if (*p1) {
		return 1;  // s1 更长
	}
	else if (*p2) {
		return -1; // s2 更长
	}

	return 0; // 字符串相等
}


int Rt_strnicmp(const char* s1, const char* s2, size_t n)
{
	const char* p1 = s1;
	const char* p2 = s2;
	size_t i = 0;

	while (*p1 && *p2 && i < n) {
		char c1 = ToLowerCase(*p1);
		char c2 = ToLowerCase(*p2);

		if (c1 < c2) {
			return -1;
		}
		else if (c1 > c2) {
			return 1;
		}

		p1++;
		p2++;
		i++;
	}

	// 如果还没比较到n个字符，但有一个字符串已经结束
	if (i < n) {
		if (*p1) {
			return 1;  // s1 更长
		}
		else if (*p2) {
			return -1; // s2 更长
		}
	}

	return 0; // 字符串相等或已比较完n个字符
}