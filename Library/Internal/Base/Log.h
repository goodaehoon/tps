#pragma once

enum LogType
{
	DEBUG,		// 디버깅용 로그
	CORE,		// 시스템 코어(혹은 라이브러리단)용 로그
	INFO,		// 기본 로그
	CRI,		// 발생하면 안되는 심각한 로그
	LOG_TYPE_MAX,
};

class LogFile
{
	FILE* fp;
	LogType logType;		// enum LogType
	wstring prefix;			// 로그 접두사 예) "Debug" 설정 -> Debug20161017.log	
	bool isWriteTime;		// 시간을 넣을지 여부
	bool isWriteFuncName;	// 함수이름과 라인번호를 넣을지 여부
	bool isWritePerHour;	// 한시간마다 로그를 새로 만들지 여부 (false이면 기본 하루단위, true이면 Debug20161017-06.log 식으로 -시간이 붙음)

public:
	LogFile(LogType type, wstring prefix, bool writeTime, bool writeFuncName, bool perHour);
	~LogFile();

	void OpenFile(const wstring& logPath);
	void Write(const wstring& logPath, const wchar_t* funcName, int lineNo, const wchar_t* message);	
};

class Log
{
	vector<LogFile> logFiles;
	wstring path;			// 로그를 저장할 디렉토리

public:
	Log();
	static Log& GetInstance();

	void WriteToFile(LogType type, const wchar_t* funcName, int lineNo, const wchar_t* message);
	void SetLogPath(wstring dir);
};

class LogHelper
{
private:
	LogType logType;
	const wchar_t* funcName;
	int lineNo;

public:
	LogHelper(LogType type, const wchar_t* funcName, int lineNo);

	void operator()(const wchar_t* message, ...);
};

#define LOG_DEBUG	LogHelper(LogType::DEBUG, __FUNCTIONW__, __LINE__)
#define LOG_CORE	LogHelper(LogType::CORE, __FUNCTIONW__, __LINE__)
#define LOG_INFO	LogHelper(LogType::INFO, __FUNCTIONW__, __LINE__)
#define LOG_CRI		LogHelper(LogType::CRI, __FUNCTIONW__, __LINE__)