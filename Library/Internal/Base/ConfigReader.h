#pragma once

#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include "StringUtil.h"
#include "Log.h"

class ConfigReader
{
private:
	boost::property_tree::wptree pt;

public:
	void Read(wstring filename);

	template <typename T>
	T Get(wstring section_key, T default_value)
	{
		try
		{
			return pt.get<T>(section_key, default_value);
		}
		catch (std::exception& e)
		{
			LOG_CORE(L"Fail to get %s key (%s)", section_key.c_str(), e.what());
			return default_value;
		}		
	}
};