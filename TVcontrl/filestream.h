#pragma once
#include <string>
class filestream
{
public:
	static std::string date , usedtimestr;
	static long long usedtime;
	void readfile();
	void outToFile();
};

