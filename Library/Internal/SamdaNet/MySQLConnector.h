#pragma once

#include <mysql.h>
#include <boost/lexical_cast.hpp>

// reference: http://dev.mysql.com/doc/refman/5.7/en/c-api.html

#define MAX_QUERY_LEN 16384
#define RETRY_COUNT 5

class MySQL
{
private:
	MYSQL* mysql;						// MySQL 핸들
	MYSQL_RES* result_set;				// 결과셋이 저장된 포인터
	MYSQL_ROW fetched_row;				// 결과셋의 하나의 row를 가르키는 포인터
	wchar_t query[MAX_QUERY_LEN];		// 실행 할 쿼리 버퍼
	unsigned long query_len;			// 실행 할 쿼리 길이
	unsigned long long num_of_rows;		// 결과 행 수
	unsigned int num_of_fields;			// 결과의 컬럼 수
	unsigned int db_errno;				// 발생한 에러
	unsigned long* fields_lengths;		// 결과데이터의 각 필드의 길이를 가르키는 포인터
	unsigned int curr_read_field;		// 결과 ROW에서 읽어 올 컬럼의 인덱스

public:
	MySQL();
	~MySQL();

	bool Open(const wchar_t* ip, unsigned int port, const wchar_t* dbname, const wchar_t* id, const wchar_t* passwd);
	void Close();
	bool ExecuteQuery(const wchar_t* format, ...);
	bool Fetch();
	void ClearResultSet();	

	unsigned long long GetAffectedRowCount();
	unsigned long long GetNumOfRows();
	unsigned long GetNumOfFields();

	bool GetByte(char& o);
	bool GetByte(unsigned char& o);
	bool GetShort(short& o);
	bool GetShort(unsigned short& o);
	bool GetInt(int& o);
	bool GetInt(unsigned int& o);
	bool GetInt64(long long& o);
	bool GetInt64(unsigned long long& o);
	bool GetFloat(float& o);
	bool GetDouble(double& o);
	bool GetBinary(char* buffer, int size);
	bool GetString(char* buffer, int size);
	bool GetString(wchar_t* buffer, int size);

	template <typename T>
	bool Get(T& out)
	{
		if (!result_set || curr_read_field >= num_of_fields)
			return false;

		try
		{
			o = boost::lexical_cast<T>(fetched_row[curr_read_field++]);
		}
		catch (bad_lexical_cast e)
		{
			std::cout << e.what() << std::endl;
		}		
	}

private:
	bool Initialize();
	bool DoQuery();

};