#include "Precompiled.h"
#include "ConfigReader.h"
#include "StringUtil.h"
#include "Log.h"
#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;

void ConfigReader::Read(wstring filename)
{
	try
	{
		boost::property_tree::read_ini(W2S(filename), pt);
	}
	catch (std::exception& e)
	{
		LOG_CORE(L"Fail to read %s config file (%s)", filename.c_str(), e.what());
	}	
}