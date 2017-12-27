#include "Precompiled.h"
#include "MySQLConnector.h"
#include "StringUtil.h"
#include "Log.h"
#include <errmsg.h>

MySQL::MySQL()
	: mysql(nullptr)
	, result_set(nullptr)
	, query_len(0)
	, num_of_rows(0)
	, num_of_fields(0)
	, db_errno(0)
	, fields_lengths(nullptr)
	, curr_read_field(0)
{
}

MySQL::~MySQL()
{
	Close();
}

bool MySQL::Initialize()
{
	if (mysql)
		mysql_close(mysql);

	mysql = mysql_init(nullptr);

	if (!mysql)
	{
		return false;
	}

	// Use the compressed client/server protocol. 
	mysql_options(mysql, MYSQL_OPT_COMPRESS, 0);
	// Read options from the named group from my.cnf or the file specified with MYSQL_READ_DEFAULT_FILE.
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "UseSQL");
	// The name of the character set to use as the default character set.
	mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	// Enable or disable automatic reconnection to the server if the connection is found to have been lost.Reconnect is off by default; this option provides a way to set reconnection behavior explicitly.
	bool reconnect = true;
	mysql_options(mysql, MYSQL_OPT_RECONNECT, (char*)&reconnect);

	return true;
}

bool MySQL::Open(const wchar_t* ip, unsigned int port, const wchar_t* dbname, const wchar_t* id, const wchar_t* passwd)
{
	if (!Initialize())
	{
		return false;
	}

	if (!mysql_real_connect(mysql, WcsToMbcs(ip), WcsToMbcs(id), WcsToMbcs(passwd), WcsToMbcs(dbname), port, nullptr, CLIENT_INTERACTIVE))
	{
		return false;
	}

	return true;
}

void MySQL::Close()
{
	ClearResultSet();

	mysql_close(mysql);
	mysql = nullptr;
}

bool MySQL::ExecuteQuery(const wchar_t* format, ...)
{
	// 가변인수 쿼리를 완성하고
	va_list args;
	va_start(args, format);
	wvsprintf(query, format, args);
	va_end(args);

	// 쿼리 사이즈를 넘어가는지 확인한다
	query_len = (unsigned long)wcslen(query);
	if (query_len >= MAX_QUERY_LEN)
	{
		LOG_CORE(L"Query length over. check the query (%u:%s)", query_len, query);
		return true;
	}

	// 쿼리 결과를 담을 결과셋을 정리하고
	ClearResultSet();

	bool query_result = false;
	// 쿼리를 실행한다. 실패했을 경우 몇번 재시도 해본다
	for (int i = 0; i < RETRY_COUNT; ++i)
	{
		query_result = DoQuery();
		if (query_result == true)
		{
			break;
		}
		else
		{
			// DB작업이 재시도 해도 성공할 수 없었다. 로그를 찍고, 추후에는 서버를 내리도록 하는게 좋겠다.
			if (i == RETRY_COUNT - 1)
			{
				LOG_CORE(L"db job failed");
				return false;
			}
		}
	}

	// 쿼리결과를 MySQL로부터 가져와 결과셋에 저장한다
	result_set = mysql_store_result(mysql);

	num_of_rows = 0;
	num_of_fields = 0;

	if (result_set)
	{
		num_of_rows = mysql_num_rows(result_set);
		num_of_fields = mysql_num_fields(result_set);
	}

	return true;
}

bool MySQL::DoQuery()
{
	// 쿼리를 실행하고, 실패시 에러번호를 설정하고 리턴한다
	int ret = mysql_real_query(mysql, WcsToMbcs(query), query_len);
	if (ret != 0)
	{
		db_errno = mysql_errno(mysql);

		// 아래의 경우에는 서버와 재연결을 시도해본다
		if (db_errno == CR_SERVER_GONE_ERROR || db_errno == CR_SERVER_LOST
			|| db_errno == CR_UNKNOWN_HOST || CR_CONN_HOST_ERROR)
		{
			LOG_CORE(L"mysql connection lost (error: %d), try reconnect", db_errno);

			// MYSQL_OPT_RECONNECT 옵션이 켜져있으므로 mysql_ping으로 재연결을 시도해보고
			if (mysql_ping(mysql) != 0)
			{
				// 실패하면 다시 연결해 본다
				if (!Open(MbcsToWcs(mysql->host), mysql->port, MbcsToWcs(mysql->db), MbcsToWcs(mysql->user), MbcsToWcs(mysql->passwd)))
					return false;
			}
		}

		return false;
	}

	return true;
}

bool MySQL::Fetch()
{
	// 결과셋의 한 행을 fetch 해온다

	if (!result_set)
		return false;

	// 결과셋의 다음 row를 리턴한다. 없으면 null리턴
	fetched_row = mysql_fetch_row(result_set);
	if (!fetched_row)
		return false;

	// 현재 row의 컬럼(데이터)의 길이를 저장한다. 
	fields_lengths = mysql_fetch_lengths(result_set);

	// 읽을 필드번호를 초기화해준다
	curr_read_field = 0;

	return true;
}

void MySQL::ClearResultSet()
{
	if (result_set)
	{
		mysql_free_result(result_set);
		result_set = nullptr;
	}

	fields_lengths = nullptr;
	num_of_rows = 0;
	num_of_fields = 0;
	curr_read_field = 0;
}

unsigned long long MySQL::GetAffectedRowCount()
{
	return mysql_affected_rows(mysql);
}

unsigned long long MySQL::GetNumOfRows()
{
	return num_of_rows;
}

unsigned long MySQL::GetNumOfFields()
{
	return num_of_fields;
}

bool MySQL::GetByte(char& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetByte(unsigned char& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetShort(short& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetShort(unsigned short& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetInt(int& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetInt(unsigned int& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atoi(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetInt64(long long& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = _atoi64(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetInt64(unsigned long long& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = _atoi64(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetFloat(float& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = (float)atof(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetDouble(double& o)
{
	if (!result_set || curr_read_field >= num_of_fields)
		return false;

	o = atof(fetched_row[curr_read_field++]);

	return true;
}

bool MySQL::GetBinary(char* buffer, int size)
{
	memset(buffer, 0, size);
	memcpy(buffer, fetched_row[curr_read_field], fields_lengths[curr_read_field]);
	curr_read_field++;

	return true;
}

bool MySQL::GetString(char* buffer, int size)
{
	strncpy(buffer, fetched_row[curr_read_field++], size);

	return true;
}

bool MySQL::GetString(wchar_t* buffer, int size)
{
	char strbuf[8192] = { 0, };
	strncpy(strbuf, fetched_row[curr_read_field++], sizeof(strbuf));
	wcsncpy(buffer, MbcsToWcs(strbuf), size);

	return true;
}