#include "json/json.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

std::string str;

void write_json()
{
	Json::Value root;
	root["id"] = "Luna";
	root["name"] = "Silver";
	root["age"] = 19;
	root["hasCar"] = false;

	Json::Value items;
	items.append("notebook");
	items.append("ipad");
	items.append("iphone7");
	root["items"] = items;

	Json::Value friends;
	Json::Value tom;
	tom["name"] = "Tom";
	tom["age"] = 21;
	Json::Value jane;
	jane["name"] = "Jane";
	jane["age"] = 23;
	friends.append(tom);
	friends.append(jane);
	root["friends"] = friends;

	Json::StyledStreamWriter writer;

	std::stringstream strstream;
	std::ofstream fout;
	fout.open(L"test.json");

	writer.write(strstream, root);
	str = strstream.str();
	printf("%s\n", str.c_str());

	fout << strstream.str() << std::endl;

	fout.close();
}

void read_json()
{
	Json::Reader reader;
	Json::Value root;

	bool ret = reader.parse(str, root);
	if (!ret)
	{
		std::cout << reader.getFormattedErrorMessages();
		return;
	}

	std::cout << root["hasCar"] << " : " << root["age"] << std::endl << std::endl;

	const Json::Value items = root["items"];
	for (int i = 0; i < items.size(); ++i)
		std::cout << items[i].asString() << std::endl;

	std::cout << std::endl;

	auto member = root.getMemberNames();
	for (std::string s : member)
	{
		std::cout << s << std::endl;
	}
	std::cout << std::endl;

	for (std::string s : member)
	{
		if (root[s].isString())
		{
			std::cout << s << std::endl;
		}		
	}
	std::cout << std::endl;

	Json::Value friends = root["friends"];
	for (auto it = friends.begin(); it != friends.end(); ++it)
	{
		if (it->isObject())
		{
			std::cout << (*it)["name"] << " : " << (*it)["age"] << std::endl;
		}
	}
	std::cout << std::endl;
}

int main()
{
	write_json();

	read_json();

	return 0;
}