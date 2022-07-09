#include "UnitTestAssert.h"


long long Assert::totalPass = 0;
long long Assert::totalFaild = 0;

__LineInfo::__LineInfo(const wchar_t* pszFileName, const char* pszFuncName, int lineNumber) : pszFile(pszFileName),
                                                                                              pszFunc(reinterpret_cast<const unsigned char*>(pszFuncName)),
                                                                                              line(lineNumber)
{
}

long long Assert::GetTotalPass()
{
	return totalPass;
}

long long Assert::GetTotalFaild()
{
	return totalFaild;
}

void Assert::AreEqual(double expected, double actual, double tolerance, const wchar_t* message, const __LineInfo* pLineInfo)
{
	double diff = expected - actual;
	FailOnCondition(fabs(diff) <= fabs(tolerance), EQUALS_MESSAGE(expected, actual, message), pLineInfo);
}

void Assert::AreEqual(float expected, float actual, float tolerance, const wchar_t* message, const __LineInfo* pLineInfo)
{
	float diff = expected - actual;
	FailOnCondition(fabs(diff) <= fabs(tolerance), EQUALS_MESSAGE(expected, actual, message), pLineInfo);
}

void Assert::AreEqual(const char* expected, const char* actual, const char* message, const __LineInfo* pLineInfo)
{
	AreEqual(expected, actual, DEFAULT_IGNORECASE, ToString(message).c_str(), pLineInfo);
}

void Assert::AreEqual(const char* expected, const char* actual, const wchar_t* message, const __LineInfo* pLineInfo)
{
	AreEqual(expected, actual, DEFAULT_IGNORECASE, message, pLineInfo);
}

void Assert::AreEqual(const char* expected, const char* actual, bool ignoreCase, const wchar_t* message, const __LineInfo* pLineInfo)
{
	FailOnCondition(CppUnitStrCmpA(expected, actual, ignoreCase), EQUALS_MESSAGE(expected, actual, message), pLineInfo);
}

void Assert::AreEqual(const wchar_t* expected, const wchar_t* actual, const char* message, const __LineInfo* pLineInfo)
{
	AreEqual(expected, actual, DEFAULT_IGNORECASE, ToString(message).c_str(), pLineInfo);
}

void Assert::AreEqual(const wchar_t* expected, const wchar_t* actual, const wchar_t* message, const __LineInfo* pLineInfo)
{
	AreEqual(expected, actual, DEFAULT_IGNORECASE, message, pLineInfo);
}

void Assert::AreEqual(const wchar_t* expected, const wchar_t* actual, bool ignoreCase, const wchar_t* message, const __LineInfo* pLineInfo)
{
	FailOnCondition(CppUnitStrCmpW(reinterpret_cast<const unsigned short*>(expected), reinterpret_cast<const unsigned short*>(actual), ignoreCase), EQUALS_MESSAGE(expected, actual, message), pLineInfo);
}

void Assert::AreNotEqual(double notExpected, double actual, double tolerance, const wchar_t* message, const __LineInfo* pLineInfo)
{
	double diff = notExpected - actual;
	FailOnCondition(fabs(diff) > fabs(tolerance), NOT_EQUALS_MESSAGE(notExpected, actual, message), pLineInfo);
}

void Assert::AreNotEqual(float notExpected, float actual, float tolerance, const wchar_t* message, const __LineInfo* pLineInfo)
{
	float diff = notExpected - actual;
	FailOnCondition(fabs(diff) > fabs(tolerance), NOT_EQUALS_MESSAGE(notExpected, actual, message), pLineInfo);
}

void Assert::AreNotEqual(const char* notExpected, const char* actual, const char* message, const __LineInfo* pLineInfo)
{
	AreNotEqual(notExpected, actual, DEFAULT_IGNORECASE, ToString(message).c_str(), pLineInfo);
}

void Assert::AreNotEqual(const char* notExpected, const char* actual, const wchar_t* message, const __LineInfo* pLineInfo)
{
	AreNotEqual(notExpected, actual, DEFAULT_IGNORECASE, message, pLineInfo);
}

void Assert::AreNotEqual(const char* notExpected, const char* actual, bool ignoreCase, const wchar_t* message, const __LineInfo* pLineInfo)
{
	FailOnCondition(!CppUnitStrCmpA(notExpected, actual, ignoreCase), NOT_EQUALS_MESSAGE(notExpected, actual, message), pLineInfo);
}

void Assert::AreNotEqual(const wchar_t* notExpected, const wchar_t* actual, const char* message, const __LineInfo* pLineInfo)
{
	AreNotEqual(notExpected, actual, DEFAULT_IGNORECASE, ToString(message).c_str(), pLineInfo);
}

void Assert::AreNotEqual(const wchar_t* notExpected, const wchar_t* actual, const wchar_t* message, const __LineInfo* pLineInfo)
{
	AreNotEqual(notExpected, actual, DEFAULT_IGNORECASE, message, pLineInfo);
}

void Assert::AreNotEqual(const wchar_t* notExpected, const wchar_t* actual, bool ignoreCase, const wchar_t* message, const __LineInfo* pLineInfo)
{
	FailOnCondition(!CppUnitStrCmpW(reinterpret_cast<const unsigned short*>(notExpected), reinterpret_cast<const unsigned short*>(actual), ignoreCase), NOT_EQUALS_MESSAGE(notExpected, actual, message), pLineInfo);
}

void Assert::IsTrue(bool condition, const wchar_t* message, const __LineInfo* pLineInfo)
{
	FailOnCondition(condition, reinterpret_cast<const unsigned short*>(message), pLineInfo);
}

void Assert::True(bool condition, const wchar_t* message, const __LineInfo* pLineInfo)
{
	FailOnCondition(condition, reinterpret_cast<const unsigned short*>(message), pLineInfo);
}

void Assert::WriteLineSpace()
{
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hstdout, ColorText::ColorText_0xF);
	std::wcout << "-----------------------------------------";
	std::wcout << MS_CPP_UNITTESTFRAMEWORK_NEW_LINE;
	std::wcout.flush();
	SetConsoleTextAttribute(hstdout, ColorText::ColorText_0xF);
}

void Assert::WriteLine(const wchar_t* message)
{
	if (message != NULL)
	{
		HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hstdout, ColorText::ColorText_0xF);
		std::wcout << message;
		std::wcout << MS_CPP_UNITTESTFRAMEWORK_NEW_LINE;
		std::wcout.flush();

		SetConsoleTextAttribute(hstdout, ColorText::ColorText_0xF);
	}
}

void Assert::IsFalse(bool condition, const wchar_t* message, const __LineInfo* pLineInfo)
{
	FailOnCondition(!condition, reinterpret_cast<const unsigned short*>(message), pLineInfo);
}

void Assert::False(bool condition, const wchar_t* message, const __LineInfo* pLineInfo)
{
	FailOnCondition(!condition, reinterpret_cast<const unsigned short*>(message), pLineInfo);
}

void Assert::ThrowsTrue()
{
	IsTrue(true, L"Throw True");
}

void Assert::ThrowsFalse()
{
	IsTrue(false, L"Throw False");
}

void Assert::Fail(const wchar_t* message, const __LineInfo* pLineInfo)
{
	FailImpl(reinterpret_cast<const unsigned short*>(message), pLineInfo);
}

void Assert::FailOnCondition(bool condition, const unsigned short* message, const __LineInfo* pLineInfo)
{
	const wchar_t* messageReal = (const wchar_t*)message;
	if (condition)
	{
		if (messageReal != NULL)
		{
			std::wcout << messageReal;
			std::wcout << L": ";
		}
		totalPass++;

		HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hstdout, ColorText::ColorText_0xA);
		std::wcout << L"PASS";
		std::wcout << MS_CPP_UNITTESTFRAMEWORK_NEW_LINE;
		SetConsoleTextAttribute(hstdout, ColorText::ColorText_0xF);
	}
	else
	{
		if (messageReal != NULL)
		{
			std::wcout << messageReal;
			std::wcout << L": ";
		}
		totalFaild++;

		HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hstdout, ColorText::ColorText_0xC);
		std::wcout << L"FAILD";
		std::wcout << MS_CPP_UNITTESTFRAMEWORK_NEW_LINE;
		SetConsoleTextAttribute(hstdout, ColorText::ColorText_0xF);
	}
}

bool Assert::CppUnitStrCmpA(const char* str1, const char* str2, bool ignoreCase)
{
	return str1 == str2;
}

bool Assert::CppUnitStrCmpW(const unsigned short* str1, const unsigned short* str2, bool ignoreCase)
{
	return str1 == str2;
}

void Assert::FailImpl(const unsigned short* message, const __LineInfo* pLineInfo)
{
}

void Assert::GetAssertMessage(bool equality, const unsigned short* expected, const unsigned short* actual, const unsigned short* userMessage, unsigned short* assertMessageBuffer, size_t bufferSize)
{
}

std::wstring Assert::GetAssertMessage(bool equality, const std::wstring& expected, const std::wstring& actual, const wchar_t* message)
{
	wchar_t assertMessage[MS_CPP_UNITTESTFRAMEWORK_MAX_BUF_LENGTH];
	memset(assertMessage, MS_CPP_UNITTESTFRAMEWORK_MAX_BUF_LENGTH * sizeof(wchar_t), 0);
	const unsigned short* expectedPtr = reinterpret_cast<const unsigned short*>(expected.c_str());
	const unsigned short* actualPtr = reinterpret_cast<const unsigned short*>(actual.c_str());
	const unsigned short* messagePtr = reinterpret_cast<const unsigned short*>(message);
	unsigned short* assertMessagePtr = reinterpret_cast<unsigned short*>(assertMessage);

	GetAssertMessage(equality, expectedPtr, actualPtr, messagePtr, assertMessagePtr, MS_CPP_UNITTESTFRAMEWORK_MAX_BUF_LENGTH);
	return std::wstring(assertMessage);
}
