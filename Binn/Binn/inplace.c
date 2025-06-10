#include "inplace.h"

int _fltused = 1;

PVOID ReallocateMemory(PVOID OriginalPtr, SIZE_T NewSize)
{
	PVOID newPtr = NULL;

	// �����´�СΪ 0 ���������ͬ���ͷ��ڴ棩
	if (NewSize == 0) {
		if (OriginalPtr) {
			ExFreePool(OriginalPtr);
		}
		return NULL;
	}

	// ���ԭָ��Ϊ�գ���ͬ��ֱ�ӷ����ڴ�
	if (!OriginalPtr) {
		return ExAllocatePool2(POOL_FLAG_NON_PAGED, NewSize, 'BINN');
	}

	// �������ڴ��
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

	// ��ʼ��ANSI_STRING�ṹ
	RtlInitAnsiString(&ansiString, str);

	// ��ANSI�ַ���ת��ΪUnicode�ַ���
	status = RtlAnsiStringToUnicodeString(
		&unicodeString,
		&ansiString,
		TRUE  // ����Ŀ�껺����
	);

	if (!NT_SUCCESS(status)) {
		return 0;
	}

	// ʹ��RtlUnicodeStringToInteger����ת��
	status = RtlUnicodeStringToInteger(
		&unicodeString,
		10,  // ����Ϊ10
		&result
	);

	// �ͷ�Unicode�ַ���������
	RtlFreeUnicodeString(&unicodeString);

	// ���ת��ʧ�ܣ�����0����_atoi64��Ϊһ�£�
	if (!NT_SUCCESS(status)) {
		return 0;
	}

	return result;
}

int atoi(const char* str)
{
	LONGLONG value = _atoi64(str);

	// ��Χ��飬ȷ��������32λ�з���������Χ
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

	// ����ǰ���ո�
	while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == '\f' || *p == '\v')
		p++;

	// �������
	if (*p == '-') {
		negative = TRUE;
		p++;
	}
	else if (*p == '+') {
		p++;
	}

	// �������ֲ���
	while (*p != '\0') {
		if (*p == '.') {
			if (decimalPoint) break; // ���С���㣬��ֹ����
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
			break; // �������ַ�����ֹ����
		}
	}

	// Ӧ��С��λ
	if (decimalDigits > 0) {
		result *= Pow10(-decimalDigits);
	}

	// �����ѧ������ (e.g., "1.23e-4")
	if (*p == 'e' || *p == 'E') {
		p++;
		BOOLEAN expNegative = FALSE;
		int expValue = 0;

		// ����ָ������
		if (*p == '-') {
			expNegative = TRUE;
			p++;
		}
		else if (*p == '+') {
			p++;
		}

		// ����ָ��ֵ
		while (IsDigit(*p)) {
			expValue = expValue * 10 + (*p - '0');
			p++;
		}

		// Ӧ��ָ��
		if (expNegative) {
			expValue = -expValue;
		}

		result *= Pow10(expValue);
	}

	// Ӧ�÷���
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

	// ����Դ�ַ������ȣ�����null��ֹ����
	length = strlen(src) + 1;

	// �����ڴ�
	dest = (PCHAR)ExAllocatePool2(POOL_FLAG_NON_PAGED, length, 'DStr');

	if (dest == NULL) {
		return NULL;        // �ڴ����ʧ��
	}

	// �����ַ���
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

	// ����ĸ��ַ����Ƚ���
	if (*p1) {
		return 1;  // s1 ����
	}
	else if (*p2) {
		return -1; // s2 ����
	}

	return 0; // �ַ������
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

	// �����û�Ƚϵ�n���ַ�������һ���ַ����Ѿ�����
	if (i < n) {
		if (*p1) {
			return 1;  // s1 ����
		}
		else if (*p2) {
			return -1; // s2 ����
		}
	}

	return 0; // �ַ�����Ȼ��ѱȽ���n���ַ�
}