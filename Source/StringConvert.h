#pragma once
#include <string>
#include <WinNls.h>

//std::string ---> std::wstring
static std::wstring ToWString(const std::string& a_str)
{
	int _len = MultiByteToWideChar(CP_UTF8, 0, a_str.c_str(), -1, nullptr, 0);
	std::wstring _wstr(_len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, a_str.c_str(), -1, &_wstr[0], _len);
	return _wstr;
}

//const wchar_t* ---> std::string
static std::string WideToUtf8(const std::wstring& wideStr)
{
	if (wideStr.empty()) return std::string();

	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(sizeNeeded - 1, 0); // 終端分を除く
	WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &result[0], sizeNeeded, nullptr, nullptr);

	return result;
}

//std::string ---> std::wstring
static std::wstring ShiftJisToWstring(const std::string& str)
{
	if (str.empty()) return std::wstring();

	int sizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	if (sizeNeeded <= 0) return std::wstring();

	std::wstring result(sizeNeeded - 1, 0); // null終端除く
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &result[0], sizeNeeded);

	return result;
}

//const wchar_t* ---> std::string
static std::string ConvertWideToUtf8(const wchar_t* wideStr)
{
	if (!wideStr) return {};

	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
	if (sizeNeeded <= 0) return {};

	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &result[0], sizeNeeded, nullptr, nullptr);

	// erase null terminator
	if (!result.empty() && result.back() == '\0')
	{
		result.pop_back();
	}

	return result;
}

static std::wstring Utf8ToWstring(const std::string& str)
{
	if (str.empty()) return std::wstring();

	int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	if (sizeNeeded <= 0) return std::wstring();

	std::wstring result(sizeNeeded - 1, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], sizeNeeded);
	return result;
}

//wchar_t ---> std::string
static std::string WideToUTF8(const wchar_t* wideStr)
{
	if (!wideStr) return "";

	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
	if (sizeNeeded <= 0) return "";

	std::string utf8Str(sizeNeeded - 1, '\0'); // -1 to exclude null terminator
	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &utf8Str[0], sizeNeeded, nullptr, nullptr);

	return utf8Str;
}