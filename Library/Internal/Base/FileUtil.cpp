#include "Precompiled.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include "FileUtil.h"

namespace fs = boost::filesystem;

bool FileUtil::IsFile(wstring path)
{
	return fs::is_regular_file(path);
}

bool FileUtil::IsDirectory(wstring path)
{
	return fs::is_directory(path);
}

bool FileUtil::Exists(wstring path)
{
	return fs::exists(path);
}

uintmax_t FileUtil::FileSize(wstring path)
{
	return fs::file_size(path);
}

bool FileUtil::CreateDirectory(wstring path)
{
	fs::path dir(path);

	if (fs::exists(dir))
		return false;

	return fs::create_directories(dir);
}

wchar_t FileUtil::GetSeparator()
{
	return fs::path::separator;
}
