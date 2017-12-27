#include "Precompiled.h"
#include "BaseDefines.h"
#include "Log.h"
#include "TimeUtil.h"
#include "FileUtil.h"

#define LINE_BUFFER 32768

LogFile::LogFile(LogType type, wstring prefix, bool writeTime, bool writeFuncName, bool writePerHour)
	: fp(0)
	, logType(type)
	, prefix(prefix)
	, isWriteTime(writeTime)
	, isWriteFuncName(writeFuncName)
	, isWritePerHour(writePerHour)
{
}

LogFile::~LogFile()
{
	if (fp != NULL)
		fclose(fp);
}

void LogFile::OpenFile(const wstring& logPath)
{
	wchar_t filename[MAX_FILENAME_LEN] = { 0, };

	struct tm nowTM;
	TimeUtil::CurTimeTM(&nowTM);

	if (isWritePerHour)
		wsprintf(filename, L"%s%04d%02d%02d-%02d.log", prefix.c_str(), nowTM.tm_year + 1900, nowTM.tm_mon + 1, nowTM.tm_mday, nowTM.tm_hour);
	else
		wsprintf(filename, L"%s%04d%02d%02d.log", prefix.c_str(), nowTM.tm_year + 1900, nowTM.tm_mon + 1, nowTM.tm_mday);

	// 로그 디렉토리로 지정된 디렉토리가 없다면 생성해준다
	if (FileUtil::Exists(logPath) == false)
		FileUtil::CreateDirectory(logPath);

	wstring fullpath(logPath);
	if (fullpath.back() == L'/')
		fullpath += filename;
	else
		fullpath = fullpath + L'/' + filename;

	fp = _wfopen(fullpath.c_str(), L"ab");
}

void LogFile::Write(const wstring& logPath, const wchar_t* funcName, int lineNo, const wchar_t* message)
{
	wchar_t header[128] = { 0, };
	wchar_t buffer[LINE_BUFFER] = { 0, };

	if (fp == NULL)
		OpenFile(logPath);

	if (isWriteTime)
	{
		struct tm nowTM;
		TimeUtil::CurTimeTM(&nowTM);
		wsprintf(header, L"[%02d:%02d:%02d]", nowTM.tm_hour, nowTM.tm_min, nowTM.tm_sec);
	}

	if (isWriteFuncName)
		wsprintf(header, L"%s %s(%d)", header, funcName, lineNo);

	fwprintf(fp, L"%s: %s\r\n", header, message);
	wprintf(L"%s: %s\r\n", header, message);

	fflush(fp);
}

Log::Log()
{
	logFiles.push_back(LogFile(LogType::DEBUG	, L"Debug"	, true, true, false));
	logFiles.push_back(LogFile(LogType::CORE	, L"Core"	, true, true, false));
	logFiles.push_back(LogFile(LogType::INFO	, L"Info"	, true, true, false));
	logFiles.push_back(LogFile(LogType::CRI		, L"Cri"	, true, true, false));
}

Log& Log::GetInstance()
{
	static Log inst;
	return inst;
}

void Log::WriteToFile(LogType type, const wchar_t* funcName, int lineNo, const wchar_t* message)
{
	logFiles[type].Write(path, funcName, lineNo, message);
}

void Log::SetLogPath(wstring dir)
{
	path = dir;
}

LogHelper::LogHelper(LogType type, const wchar_t* funcName, int lineNo)
	: logType(type)
	, funcName(funcName)
	, lineNo(lineNo)
{
}

void LogHelper::operator()(const wchar_t* message, ...)
{
	wchar_t buffer[LINE_BUFFER] = { 0, };

	va_list args;
	va_start(args, message);
	wvsprintf(buffer, message, args);
	va_end(args);

	Log::GetInstance().WriteToFile(logType, funcName, lineNo, buffer);
}