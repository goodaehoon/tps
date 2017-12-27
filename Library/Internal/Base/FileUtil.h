#pragma once

class FileUtil
{
public:
	static bool IsFile(wstring path);
	static bool IsDirectory(wstring path);
	static bool Exists(wstring path);
	static uintmax_t FileSize(wstring path);
	static bool CreateDirectory(wstring path);	
	static wchar_t GetSeparator();
};