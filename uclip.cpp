/*
A simple Windows console utility to copy text to clipboard.

IMPORTANT NOTICE: The utility expects the input text to be in UTF-8 encoding.

It makes the utility convenient for using under Cygwin or Git Bash.
In cmd.exe, it is simpler to stick with `clip.exe` that ships with Windows.

If your text is not in UTF-8, convert it first with something like `iconv`.
*/

#include "pch.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <io.h>
#include <Windows.h>


std::vector<wchar_t> ToUnicode(UINT codepage, LPCSTR data, int cbData);
/*
void PrintLastError(const std::wstring & prefix = std::wstring());
std::wstring GetErrorMessage(DWORD lastError);
*/


int wmain()
{
	// Read input into `data`
	std::vector<char> data(1024);
	size_t size = 0;
	while (true) {
		if (data.size() < size * 2)
			data.resize(size * 2);
		size_t read = (size_t)std::cin.read(&data.front() + size, data.size() - size).gcount();
		if (read <= 0)
			break;
		size += read;
	}
	
	// Finalize `data`
	if (size == 0)
		return 0;
	data.resize(size);
	data.push_back(0);

	// Convert to unicode
	std::vector<wchar_t> unicodeData = ToUnicode(65001 /* UTF-8 */, &data.front(), data.size());
	data.clear();

	// Copy data to global memory
	HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, unicodeData.size() * sizeof(unicodeData[0]));
	memcpy(GlobalLock(mem), &unicodeData.front(), unicodeData.size() * sizeof(unicodeData[0]));
	GlobalUnlock(mem);
	unicodeData.clear();

	// Set clipboard data
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, mem);
	CloseClipboard();
}

std::vector<wchar_t> ToUnicode(UINT codepage, LPCSTR data, int cbData) {
	int size = MultiByteToWideChar(codepage, 0, data, cbData, NULL, 0);
	std::vector<wchar_t> unicodeData(size);
	MultiByteToWideChar(CP_UTF8, 0, data, cbData, &unicodeData.front(), unicodeData.size());
	return unicodeData;
}

/*
void PrintLastError(const std::wstring & prefix /\* = std::wstring() *\/) {
	DWORD lastError = ::GetLastError();
	if (lastError == 0)
		return;
	std::wstring message = GetErrorMessage(lastError);
	if (!prefix.empty())
		std::wcerr << prefix << L": ";
	std::wcerr << "Error " << lastError << ": " << message << std::endl;
}

// Returns the last Win32 error, in string format. Returns an empty string if there is no error.
// https://stackoverflow.com/a/17387176/675333
std::wstring GetErrorMessage(DWORD lastError)
{
	LPWSTR buf = nullptr;
	size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), LPWSTR(&buf), 0, NULL);
	std::wstring message(buf, size);
	LocalFree(buf);
	return message;
}
*/